#include <QtGui/QApplication>
#include <windows.h>
#include "dianvotecontrol.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DianVoteControl w;
    w.setWindowFlags(Qt::FramelessWindowHint);
    w.show();

    return a.exec();
}
