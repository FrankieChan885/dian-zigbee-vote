#include <QApplication>
#include <iostream>
#include "testhiddevice.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QHidDevice *device = new QHidDevice(0x0451, 0x16a9, 2, &a);
    TestHidDevice *test = new TestHidDevice(device, 0);
    test->start();

    test->showInData(QByteArray(""));

    return a.exec();
}

