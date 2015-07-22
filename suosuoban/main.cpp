#include "mainwindow.h"

#include <QApplication>

#include <cassert>
#include "geom.h"

using namespace std;

int main(int argc, char *argv[])
{

    //running
    QApplication a(argc, argv);
    MainWindow w;
    w.setGeometry(100,100,800,600);

    w.loadLanguage(w.defaultLocale);

    w.show();

    return a.exec();
}







