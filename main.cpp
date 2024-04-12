#include "ropey.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Ropey w;
    w.show();
    return a.exec();
}
