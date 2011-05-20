#include <QApplication>
#include "testdianvote.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qInstallMsgHandler(TestDianvote::DianVoteMsgHandler);

    qDebug("main(): qt started...");

    TestDianvote test;
    test.setWindowFlags(Qt::WindowMinMaxButtonsHint);
    test.show();

    return a.exec();
}
