#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
//    //это для запуска на "других компах"
//    QStringList paths = QCoreApplication::libraryPaths();
//    paths.append(".");
//    paths.append("imageformats");
//    paths.append("platforms");
//    paths.append("sqldrivers");
//    QCoreApplication::setLibraryPaths(paths);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

