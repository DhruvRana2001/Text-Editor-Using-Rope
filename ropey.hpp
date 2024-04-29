#pragma once

#include <QMainWindow>
#include "rope.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class Ropey;
}
class QAction;
class QMenu;
class QPlainTextEdit;
class QSessionManager;
QT_END_NAMESPACE

class Ropey : public QMainWindow
{
    Q_OBJECT

public:
    Ropey(QWidget *parent = nullptr);
    ~Ropey();

    void loadFile(const QString &fileName);

    struct DiffChunk{
        string text;
        uint32_t pos;
        bool isInsertion;
    };

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void documentWasModified();
    void handleModificationChanged(bool modified);
    void handleTextChanged();
    void close();
#ifndef QT_NO_SESSIONMANAGER
    void commitData(QSessionManager &);
#endif

private:
    Ui::Ropey *ui;
    void setupStatusTips();
    void setupShortcuts();
    void setupConnections();

    void readSettings();
    void writeSettings();
    bool maybeSave();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    vector<vector<uint32_t>> LevistanDistance (const string& str1, const string& str2);
    vector<DiffChunk> diff(string& str1, string& str2);

    Rope *rope;

    QString curFile;
};
