#include <QMessageBox>
#include <QByteArray>
#include "../utilities/hiddevice.h"

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
        return hidDevice->open(QIODevice::ReadOnly);
    }

public slots:
    void showInData(QByteArray ba) {
        QMessageBox::information(0, "test", ba);
    }

private:
    QHidDevice *hidDevice;
};

