#include "mainwin.h"
#include "opensavewindow.h"
#include <QApplication>
#include "functhread.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWin w;
    w.show();
    qRegisterMetaType<std::vector<cordsPair> >();
    return a.exec();
}
