#include "mainwindow.h"
#include "ccrashstack.h"
#include <QApplication>

#include <cassert>
#include "geom.h"
#include <iostream>
using namespace std;


long __stdcall   callback(_EXCEPTION_POINTERS*   excp)
{
    CCrashStack crashStack(excp);
    QString sCrashInfo = crashStack.GetExceptionInfo();
    QString sFileName = "testcrash.log";

    QFile file(sFileName);
    if (file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        file.write(sCrashInfo.toUtf8());
        file.close();
    }

    qDebug()<<"Error:\n"<<sCrashInfo;

    QMessageBox msgBox;
    msgBox.setText(QString::fromUtf8("亲，我死了，重新启动下吧！"));
    msgBox.exec();

    return   EXCEPTION_EXECUTE_HANDLER;
}


int main(int argc, char *argv[])
{
    SetUnhandledExceptionFilter(callback);

    //running
    QApplication a(argc, argv);
    MainWindow w;
    w.setGeometry(100,100,800,600);
    w.show();

    return a.exec();
}







