#include <QApplication>
#include "testdianvote.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug("main(): qt started...");

    TestDianvote test;
    test.show();

    return a.exec();
}
