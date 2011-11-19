#include <QtGui/QApplication>
#include "mainwindow.h"
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    srand(time(0));
    QApplication a(argc, argv);
    MainWindow w (&a);
    w.show();

    return a.exec();
}
