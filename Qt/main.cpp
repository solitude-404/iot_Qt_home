#include "mainwindow.h"
#include "dbhelper.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DBHelper::initDB();

    MainWindow w;
    w.show();
    return QApplication::exec();
}
