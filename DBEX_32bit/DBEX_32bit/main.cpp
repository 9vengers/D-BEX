#include "DBEX_32bit.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DBEX_32bit w;
    w.show();
    return a.exec();
}

