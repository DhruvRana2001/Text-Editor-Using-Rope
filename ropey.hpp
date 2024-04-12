#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class Ropey;
}
QT_END_NAMESPACE

class Ropey : public QMainWindow
{
    Q_OBJECT

public:
    Ropey(QWidget *parent = nullptr);
    ~Ropey();

private:
    Ui::Ropey *ui;
};
