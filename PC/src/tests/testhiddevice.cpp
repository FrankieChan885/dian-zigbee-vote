#include <QMessageBox>
#include <QApplication>
#include <QByteArray>
#include "../utilities/hiddevice.h"
#include "testhiddevice.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QHidDevice *device = new QHidDevice(0x0451, 0x16a9, 2, 3);
    TestHidDevice *test = new TestHidDevice(device);
    test->start();

    return a.exec();
}

