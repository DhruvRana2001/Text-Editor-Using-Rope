#pragma once

#include <QMainWindow>

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

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void close();


private:
    Ui::Ropey *ui;
    void setupStatusTips();
    void setupShortcuts();
    void setupConnections();

    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    QString curFile;
};
