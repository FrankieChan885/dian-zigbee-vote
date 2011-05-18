#include <QApplication>
#include <iostream>
#include "testhiddevice.h"
#include "exceptions.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug("main(): qt started...");

    try {
#ifdef USE_LIBHID
    QHidDevice *device = new QHidDevice(0x0451, 0x16a9, 0, &a);
#else
    QHidDevice *device = new QHidDevice(0x0451, 0x16a9, &a);
    qDebug("main(): hid device created...");
#endif // #ifdef USE_LIBHID
    TestHidDevice *test = new TestHidDevice(device, 0);
    test->start();

    test->showInData(QByteArray(""));
    } catch (DianVoteStdException *e) {
        QMessageBox::critical(0, "error", e->what());
        return 0;
    } catch (...) {
        QMessageBox::critical(0, "error", "unknow exception.");
    }

    return a.exec();
}

