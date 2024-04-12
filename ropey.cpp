#include "ropey.hpp"
#include "./ui_ropey.h"

Ropey::Ropey(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Ropey)
{
    ui->setupUi(this);
}

Ropey::~Ropey()
{
    delete ui;
}
