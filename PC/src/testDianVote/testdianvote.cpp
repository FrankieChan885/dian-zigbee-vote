#include <iostream>
#include <QtGui>
#include "exceptions.h"
#include "testdianvote.h"

TestDianvote::TestDianvote(QDialog *parent/* = 0*/)
    : QDialog(parent)
    , count(0)
    , hidDevice(0)
    , ui(new Ui::TestDianVote)
{
    ui->setupUi(this);
    ui->retranslateUi(this);

    connect(ui->startTest, SIGNAL(clicked(bool)),
            this, SLOT(usbStartClicked(bool)));
    connect(ui->remoteState, SIGNAL(clicked(bool)),
            this, SLOT(remoteStateClicked(bool)));
    connect(ui->remoteID1, SIGNAL(textChanged(QString)),
            this, SLOT(remoteIDChanged(QString)));
    connect(ui->remoteID2, SIGNAL(textChanged(QString)),
            this, SLOT(remoteIDChanged(QString)));
}


TestDianvote::~TestDianvote() {
    if (hidDevice) {
        hidDevice->close();
        delete hidDevice;
        hidDevice = 0;
    }
}

void TestDianvote::showInData(QByteArray ba) {
    if (ba.size() < 5) {
        return;
    }
    // print remote ID:
    QString msg;
    msg.append(QString("%L1: remote: ").arg(count++, 4, 16));
    quint16 id[2];
    memcpy(id, (quint16*) ba.data(), 2);
    msg.append(QString("0x%L1%L2").
               arg(id[0], 4, 16, QChar('0')).arg(id[1], 4, 16, QChar('0')));

    // print remote cmd
    msg.append(QString("\t%L1").arg((unsigned char) ba.at(4), 2, 16));

    ui->output->append(msg);
}

void TestDianvote::remoteIDChanged(const QString &) {
    ui->remoteState->setChecked(false);
}

void TestDianvote::usbStartClicked(bool isStart) {
    // get vid & pid.
    quint16 vid, pid;
    bool ok;
    vid = ui->usbVID->text().toUShort(&ok, 16);
    if (!ok) {
        qDebug("TestDianvote::usbStartClicked(): vid should be based 16.");
        return;
    }
    pid = ui->usbPID->text().toUShort(&ok, 16);
    if (!ok) {
        qDebug("TestDianvote::usbStartClicked(): pid should be based 16.");
        return;
    }

    if (isStart) {
        // clear count
        count = 0;

        try {
            // create new device.
            if (hidDevice) {
                delete hidDevice;
                hidDevice = 0;
            }
#ifdef USE_LIBHID
            hidDevice = new QHidDevice(vid, pid, 0, this);
#else
            hidDevice = new QHidDevice(vid, pid, this);
            qDebug("TestDianvote::usbStartClicked(): hid device created...");
#endif // #ifdef USE_LIBHID

            QObject::connect(hidDevice, SIGNAL(readInterrupt(QByteArray)),
                             this, SLOT(showInData(QByteArray)));

            // open hid device.
            if (!hidDevice->open(QIODevice::ReadOnly)) {
                // not started.
                QMessageBox::critical(0, "error", "hid device open failed.");
                ui->startTest->setChecked(false);
                if (hidDevice) {
                    delete hidDevice;
                    hidDevice = 0;
                    qDebug("TestDianvote::usbStartClicked(): hidDevice deleted...");
                }
                return;
            }
            qDebug("TestDianvote::usbStartClicked(): hidDevice opened...");

            // start listening the endpoint 1 with data length 5
            // (in windows length should be 6, I don't know why).
#ifdef USE_LIBHID
            hidDevice->startListening(1, 6);
#else
            hidDevice->startListening(6);
#endif // #ifdef USE_LIBHID

        } catch (DianVoteStdException *e) {
            QMessageBox::critical(0, "error", e->what());
            return;
        } catch (...) {
            QMessageBox::critical(0, "error", "unknow exception.");
        }
    }
    else {
        if (hidDevice) {
            hidDevice->close();
            delete hidDevice;
            hidDevice = 0;
            qDebug("TestDianvote::usbStartClicked(): hidDevice deleted...");
        }
    }
}

void TestDianvote::remoteStateClicked(bool isWork) {
    // don't write data unless usb is opened.
    if (ui->startTest->isChecked()) {
        quint16 id[2];
        bool ok;
        id[0] = ui->remoteID1->text().toUShort(&ok, 16);
        if (!ok) {
            qDebug("TestDianvote::remoteStateClicked(): remote id should be based 16.");
            return;
        }
        id[1] = ui->remoteID2->text().toUShort(&ok, 16);
        if (!ok) {
            qDebug("TestDianvote::remoteStateClicked(): remote id should be based 16.");
            return;
        }
        char remoteCtrl[5] = {0xff, 0xff, 0xff, 0xff, isWork? 1: 0};
        memcpy(remoteCtrl, (char*) id, 4);
        hidDevice->writeData(remoteCtrl, 5);
    }
    else {
        QMessageBox::critical(0, "error", "test not started.");
        // release button.
        ui->remoteState->setChecked(false);
    }
}

