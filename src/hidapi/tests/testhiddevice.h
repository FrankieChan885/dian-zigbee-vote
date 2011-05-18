#include <QMessageBox>
#include <QByteArray>
#include <cstdio>
#include "hiddevice.h"

class TestHidDevice : public QObject
{
    Q_OBJECT

public:
    TestHidDevice(QHidDevice *hiddev, QObject *parent = 0)
        : QObject(parent)
        , hidDevice(hiddev)
    {
        QObject::connect(hidDevice, SIGNAL(readInterrupt(QByteArray)),
                this, SLOT(showInData(QByteArray)));
    }
    ~TestHidDevice() {
        hidDevice->close();
    }

    bool start() {
        if (!hidDevice->open(QIODevice::ReadOnly)) {
            return false;
        }
        qDebug("TestHidDevice::start(): hidDevice opened...");

        char remoteCtrl[5] = {0xff, 0xff, 0xff, 0xff, 0x00};
        printf("remote control code: ");
        scanf("%d", remoteCtrl + 4);
        hidDevice->writeData(remoteCtrl, 5);

        // start listening the endpoint 1 with data length 5
        // (in windows length should be 6, I don't know why).
#ifdef USE_LIBHID
        hidDevice->startListening(1, 6);
#else
        hidDevice->startListening(6);
#endif // #ifdef USE_LIBHID

        return true;
    }

public slots:
    void showInData(QByteArray ba) {
        static int count = 0;
        printf("%04d: remote: ", count++);
        if (ba.size() < 5) {
            printf("\n");
            return;
        }
        // print remote ID:
        quint16 id1 = *((quint16*) ba.data());
        quint16 id2 = *((quint16*) ba.data() + 1);
        printf("%04x-", id1);
        printf("%04x", id2);

        // print remote cmd
        printf("\t%02x\n", (unsigned char) ba.at(4));
    }

private:
    QHidDevice *hidDevice;
};
