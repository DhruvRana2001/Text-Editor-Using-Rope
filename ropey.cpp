#include "ropey.hpp"
#include "./ui_ropey.h"

#include <QtWidgets>

Ropey::Ropey(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Ropey)
{
    ui->setupUi(this);

    setCentralWidget(ui->textEdit);
    setWindowTitle("Ropey [*]");
    ui->textEdit->setFocus();

    setupStatusTips();
    setupShortcuts();
    setupConnections();

    readSettings();

#ifndef QT_NO_SESSIONMANAGER
    //QGuiApplication::setFallbackSessionManagementEnabled(false);
    connect(qApp, &QGuiApplication::commitDataRequest,
            this, &Ropey::commitData);
#endif

    //Setup Backened
    rope = new Rope();

    setCurrentFile(QString());
    setUnifiedTitleAndToolBarOnMac(true);

    ui->statusbar->showMessage("Ready");
}

Ropey::~Ropey()
{
    delete ui;
}

void Ropey::setupShortcuts()
{
    ui->actionNew->setShortcut(QKeySequence::New);
    ui->actionOpen->setShortcut(QKeySequence::Open);
    ui->actionSave->setShortcut(QKeySequence::Save);
    ui->actionSave_As->setShortcut(QKeySequence::SaveAs);
    ui->actionUndo->setShortcut(QKeySequence::Undo);
    ui->actionRedo->setShortcut(QKeySequence::Redo);
    ui->actionCut->setShortcut(QKeySequence::Cut);
    ui->actionCopy->setShortcut(QKeySequence::Copy);
    ui->actionPaste->setShortcut(QKeySequence::Paste);
    ui->actionExit->setShortcut(QKeySequence::Quit);
}

void Ropey::setupStatusTips()
{
    ui->actionNew->setStatusTip("Create a new file");
    ui->actionOpen->setStatusTip("Open an existing file");
    ui->actionSave->setStatusTip("Save the current file");
    ui->actionSave_As->setStatusTip("Save the current file under a new name");
    ui->actionUndo->setStatusTip("Undo the last action");
    ui->actionRedo->setStatusTip("Redo the last undone action");
    ui->actionCut->setStatusTip("Cut the current selection's contents");
    ui->actionCopy->setStatusTip("Copy the current selection's contents");
    ui->actionPaste->setStatusTip("Paste the current clipboard's contents");
    ui->actionExit->setStatusTip("Exit the application");
}

void Ropey::setupConnections()
{   
    connect(ui->textEdit->document(), &QTextDocument::contentsChanged,
        this, &Ropey::documentWasModified);

    connect(ui->textEdit->document(), &QTextDocument::modificationChanged,
            this, &Ropey::handleModificationChanged);

    connect(ui->textEdit, &QTextEdit::textChanged, this, &Ropey::handleTextChanged);

    connect(ui->actionUndo, &QAction::triggered, ui->textEdit, &QTextEdit::undo);
    connect(ui->actionRedo, &QAction::triggered, ui->textEdit, &QTextEdit::redo);
    connect(ui->actionCut, &QAction::triggered, ui->textEdit, &QTextEdit::cut);
    connect(ui->actionCopy, &QAction::triggered, ui->textEdit, &QTextEdit::copy);
    connect(ui->actionPaste, &QAction::triggered, ui->textEdit, &QTextEdit::paste);

    connect(ui->actionNew, &QAction::triggered, this, &Ropey::newFile);
    connect(ui->actionOpen, &QAction::triggered, this, &Ropey::open);
    connect(ui->actionSave, &QAction::triggered, this, &Ropey::save);
    connect(ui->actionSave_As, &QAction::triggered, this, &Ropey::saveAs);
    connect(ui->actionExit, &QAction::triggered, this, &Ropey::close);
}

void Ropey::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = screen()->availableGeometry();
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
}

void Ropey::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
}

void Ropey::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

bool Ropey::maybeSave()
{
    qDebug() << "Save if file mdoified : " << ui->textEdit->document()->isModified();
    if (!ui->textEdit->document()->isModified())
        return true;
    
    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("Rope"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard
                               | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }

    qDebug() << "Returning : True";
    return true;
}

void Ropey::newFile()
{
    qDebug() << "new file button";
    if (maybeSave()) {
        qDebug() << "Deleting rope 1";
        rope = new Rope();
        qDebug() << "clear text area 1";
        ui->textEdit->clear();
        setCurrentFile(QString());
    }
    qDebug() << "Deleting rope";
    rope = new Rope();
    qDebug() << "clear text area";
    ui->textEdit->clear();
}

void Ropey::open()
{
    qDebug() << "open button clicked";

    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open File"),
                                                        "./",
                                                        tr("Text files (*.txt);;"
                                                        "XML files (*.xml);;"
                                                        "Code files (*.cpp *.hpp *.h *.c);;"
                                                        "All files (*.*)"));
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool Ropey::save()
{
    qDebug()  << "save button clicked";

    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool Ropey::saveAs()
{
    qDebug()  << "save As button clicked";

    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    return saveFile(dialog.selectedFiles().first());
}

void Ropey::close()
{
    qDebug() << "close button clicked";
}

void Ropey::documentWasModified()
{
    setWindowModified(ui->textEdit->document()->isModified());
}

void Ropey::handleModificationChanged(bool modified)
{
    setWindowModified(modified);
}

vector<vector<uint32_t>> Ropey::LevistanDistance (const string& str1, const string& str2) {
    uint32_t m = str1.size();
    uint32_t n = str2.size();

    vector<vector<uint32_t>> dp(m + 1, vector<uint32_t>(n + 1, 0));

    for (uint32_t i = 0; i <= m; i++)
        dp[i][0] = i;

    for (uint32_t j = 0; j <= n; j++)
        dp[0][j] = j;

    for (uint32_t i = 1; i <= m; i++) {
        for (uint32_t j = 1; j <= n; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1]; // Characters match
            } else {
                dp[i][j] = min( dp[i - 1][j]+1 ,  // Deletion of str1[i - 1]
                                dp[i][j - 1]+1 ); // Insertion of str2[j - 1]
            }
        }
    }

    return dp;
}

vector<Ropey::DiffChunk> Ropey::diff(string& str1, string& str2) {

    vector<Ropey::DiffChunk> diffrences;
    vector<vector<uint32_t>> dp = LevistanDistance(str1, str2);

    uint32_t i = str1.size();
    uint32_t j = str2.size();

    string insertChunk = "";
    string deleteChunk = "";

    while (i > 0 || j > 0) {
        if (i > 0 && dp[i][j] == dp[i - 1][j] + 1) { // Deletion
            if (!insertChunk.empty()) {
                // Insert
                reverse(insertChunk.begin(), insertChunk.end());
                diffrences.push_back({insertChunk, i, true});
                insertChunk = "";
            }
            deleteChunk = str1[i - 1] + deleteChunk;
            i--;
        }
        else if (j > 0 && dp[i][j] == dp[i][j - 1] + 1) { // Insertion
            if (!deleteChunk.empty()) {
                // Delete
                diffrences.push_back({deleteChunk, i, false});
                deleteChunk = "";
            }
            insertChunk += str2[j - 1];
            j--;
        }
        else {
            if (!insertChunk.empty()) {
                // Insert
                reverse(insertChunk.begin(), insertChunk.end());
                diffrences.push_back({insertChunk, i, true});
                insertChunk = "";
            }
            if (!deleteChunk.empty()) {
                // Delete
                diffrences.push_back({deleteChunk, i, false});
                deleteChunk = "";
            }
            i--;
            j--;
        }
    }

    if (!deleteChunk.empty()) {
        // Delete
        diffrences.push_back({deleteChunk, i, false});
    }

    if (!insertChunk.empty()) {
        // Insert
        reverse(insertChunk.begin(), insertChunk.end());
        diffrences.push_back({insertChunk, i, true});
    }
    

    //reverse(diffrences.begin(), diffrences.end());

    return diffrences;
}


void Ropey::handleTextChanged() {

    // Store the current cursor position
    QTextCursor cursor = ui->textEdit->textCursor();
    int cursorPosition = cursor.position();
    // Store the previous modified status
    bool wasModified = ui->textEdit->document()->isModified();

    // Disconnect the textChanged signal temporarily
    disconnect(ui->textEdit, &QTextEdit::textChanged, this, &Ropey::handleTextChanged);

    qDebug() << "Invoked handle text";
    string curStr = ui->textEdit->toPlainText().toStdString();
    string prevStr = rope->toString();
    auto operations = diff(prevStr, curStr);

    for (auto& op : operations) {
        if (op.isInsertion) {
            rope->insert(op.pos, op.text.c_str(), op.text.length());
        } else {
            rope->remove(op.pos, op.text.size());
        }
    }

    qDebug() << "Rope toString : " << rope->toString();

    ui->textEdit->setPlainText(curStr.c_str());

    cursor.setPosition(cursorPosition);
    ui->textEdit->setTextCursor(cursor);
    // Restore the modified status
    ui->textEdit->document()->setModified(wasModified);

    // Reconnect the textChanged signal
    connect(ui->textEdit, &QTextEdit::textChanged, this, &Ropey::handleTextChanged);
}

void Ropey::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                                 .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    rope = new Rope(fileName.toStdString().c_str());
    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    ui->textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool Ropey::saveFile(const QString &fileName)
//! [44] //! [45]
{
    QString errorMessage;

    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    QSaveFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        out << ui->textEdit->toPlainText();
        if (!file.commit()) {
            errorMessage = tr("Cannot write file %1:\n%2.")
                               .arg(QDir::toNativeSeparators(fileName), file.errorString());
        }
    } else {
        errorMessage = tr("Cannot open file %1 for writing:\n%2.")
                           .arg(QDir::toNativeSeparators(fileName), file.errorString());
    }
    QGuiApplication::restoreOverrideCursor();

    if (!errorMessage.isEmpty()) {
        QMessageBox::warning(this, tr("Application"), errorMessage);
        return false;
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void Ropey::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    ui->textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}

QString Ropey::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

#ifndef QT_NO_SESSIONMANAGER
void Ropey::commitData(QSessionManager &manager)
{
    if (manager.allowsInteraction()) {
        if (!maybeSave())
            manager.cancel();
    } else {
        // Non-interactive: save without asking
        if (ui->textEdit->document()->isModified())
            save();
    }
}
#endif

