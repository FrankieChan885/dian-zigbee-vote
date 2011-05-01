#include <QApplication>
#include <iostream>
#include "testhiddevice.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef USE_LIBHID
    QHidDevice *device = new QHidDevice(0x0451, 0x16a9, 2, &a);
#else
    QHidDevice *device = new QHidDevice(0x0451, 0x16a9, &a);
#endif // #ifdef USE_LIBHID
    TestHidDevice *test = new TestHidDevice(device, 0);
    test->start();

    test->showInData(QByteArray(""));

    return a.exec();
}

