#include "ropey.hpp"
#include "./ui_ropey.h"

#include <QtWidgets>

Ropey::Ropey(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Ropey)
{
    ui->setupUi(this);

    setCentralWidget(ui->textEdit);
    setWindowTitle("Ropey");
    ui->textEdit->setFocus();

    setupStatusTips();
    setupShortcuts();
    setupConnections();

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
    //ui->actionPrint->setShortcut(QKeySequence::Print);
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
    //ui->actionPrint->setStatusTip("Print the current file");
    ui->actionUndo->setStatusTip("Undo the last action");
    ui->actionRedo->setStatusTip("Redo the last undone action");
    ui->actionCut->setStatusTip("Cut the current selection's contents");
    ui->actionCopy->setStatusTip("Copy the current selection's contents");
    ui->actionPaste->setStatusTip("Paste the current clipboard's contents");
    ui->actionExit->setStatusTip("Exit the application");
}

void Ropey::setupConnections()
{
    connect(ui->actionUndo, &QAction::triggered, ui->textEdit, &QTextEdit::undo);
    connect(ui->actionRedo, &QAction::triggered, ui->textEdit, &QTextEdit::redo);
    connect(ui->actionCut, &QAction::triggered, ui->textEdit, &QTextEdit::cut);
    connect(ui->actionCopy, &QAction::triggered, ui->textEdit, &QTextEdit::copy);
    connect(ui->actionPaste, &QAction::triggered, ui->textEdit, &QTextEdit::paste);

    connect(ui->actionNew, &QAction::triggered, this, &Ropey::newFile);
    connect(ui->actionOpen, &QAction::triggered, this, &Ropey::open);
    connect(ui->actionSave, &QAction::triggered, this, &Ropey::save);
    connect(ui->actionSave_As, &QAction::triggered, this, &Ropey::saveAs);
    //connect(ui->actionPrint, &QAction::triggered, this, &Ropey::print);
    connect(ui->actionExit, &QAction::triggered, this, &Ropey::close);
    //connect(ui->actionAbout, &QAction::triggered, this, &Ropey::about);
}

void Ropey::newFile()
{
    qDebug() << "new file button";
    //TODO : Check if we had a previously opened file that was modified
    // if yes : give  a promt to save it
    // if no : clear the text area
    ui->textEdit->clear();
}

void Ropey::open()
{
    qDebug() << "open button clicked";
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

void Ropey::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                                 .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    ui->textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif

    //setCurrentFile(fileName);
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
    qDebug()  << "saveAs button clicked";

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
    //close();
}
