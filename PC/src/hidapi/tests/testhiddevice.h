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
        qDebug("TestHidDevice::start: hidDevice opened...");

        // start listening the ep 1 with data length 5.
        hidDevice->startListening(1, 5);

        return true;
    }

public slots:
    void showInData(QByteArray ba) {
//        QMessageBox::information(0, "test", ba);
        printf("remote: ");
        // print remote ID:
        for (int i = 0; i < 4; i++) {
            printf("%02x", (unsigned char) ba.at(i));
        }
        // print remote cmd
        printf("\t%02x\n", (unsigned char) ba.at(4));
    }

private:
    QHidDevice *hidDevice;
};

