#include <iostream>
#include <QtGui>
#include <QIcon>
#include <QTimer>
#include <Qdir>
#include "exceptions.h"
#include "testdianvote.h"

QList< QString >* TestDianvote::message = new QList< QString >();

TestDianvote::TestDianvote(QDialog *parent/* = 0*/)
    : QDialog(parent)
    , count(0)
    , countID(0)
    , hidControl(0)
    , ui(new Ui::TestDianVote)
    , timer(new QTimer())
{
    ui->setupUi(this);
    ui->retranslateUi(this);

    connect(ui->startTest, SIGNAL(clicked(bool)),
            this, SLOT(usbStartClicked(bool)));
    connect(ui->remoteState, SIGNAL(clicked(bool)),
            this, SLOT(remoteStateClicked(bool)));
    connect(ui->remoteID, SIGNAL(textChanged(QString)),
            this, SLOT(remoteIDChanged(QString)));
    connect(ui->getIDList, SIGNAL(clicked()),
            this, SLOT(getIDList()));
    connect(ui->getIDAmount, SIGNAL(clicked()),
            this, SLOT(getIDListLength()));
    connect(ui->clearData, SIGNAL(clicked()),
            ui->output, SLOT(clear()));
    connect(ui->clearData, SIGNAL(clicked()),
            ui->rollcall, SLOT(clear()));
    connect(ui->clearMessage, SIGNAL(clicked()),
            ui->message, SLOT(clear()));

    connect(timer, SIGNAL(timeout()), this, SLOT(showIntenalMessage()));
    timer->setInterval(200);
    timer->start();

    try {
        // create new device.
        if (hidControl) {
            delete hidControl;
            hidControl = 0;
        }
        hidControl = new HidControl(this);
        QObject::connect(hidControl, SIGNAL(voteComing(quint16, quint8)),
            this, SLOT(showInData(quint16, quint8)));
        connect(hidControl, SIGNAL(idComing(quint16)),
                this, SLOT(showID(quint16)));
        connect(hidControl, SIGNAL(idAmountComing(uint)),
                this, SLOT(showIDAmount(uint)));
        connect(hidControl, SIGNAL(idSentFinished()), this, SLOT(idReceiveFinished()));

    } catch (DianVoteStdException *e) {
        if (hidControl) {
            delete hidControl;
            hidControl = 0;
        }
        QMessageBox::critical(0, "error", e->what());
        return;
    } catch (...) {
        if (hidControl) {
            delete hidControl;
            hidControl = 0;
        }
        QMessageBox::critical(0, "error", "unknow exception.");
        return;
    }
    QDir dir;
    dir.setCurrent(QCoreApplication::applicationDirPath());

    QIcon *windowIcon = new QIcon(dir.absoluteFilePath("res/icons/test.png"));
    this->setWindowIcon(*windowIcon);
}


TestDianvote::~TestDianvote() {
    if (hidControl) {
        delete hidControl;
        hidControl = 0;
    }
}

void TestDianvote::showInData(quint16 id, quint8 option) {
    // print remote ID:
    QString msg;
    msg.append(QString("%L1: remote: ").arg(count++, 4, 16, QChar('0')));
    msg.append(QString("0x%L1").
               arg(id, 4, 16, QChar('0')));

    // print remote cmd
    msg.append(QString("\t%L1").arg(option, 2, 16));

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
            if (!hidControl) {
                hidControl = new HidControl(this);
            }
            // open hid device.
            hidControl->open();
            qDebug("TestDianvote::usbStartClicked(): hidControl started...");

        } catch (DianVoteStdException *e) {
            ui->startTest->setChecked(false);
            QMessageBox::critical(0, "error", e->what());
            return;
        } catch (...) {
            ui->startTest->setChecked(false);
            QMessageBox::critical(0, "error", "unknow exception.");
            return;
        }
    } // if (isStart)
    else {
		// sleep remote.
		ui->remoteState->setChecked(false);
        if (hidControl) {
            hidControl->stop();
        }
    } // if (isStart) else
}

void TestDianvote::remoteStateClicked(bool isWork) {
    // don't write data unless usb is opened.
    if (ui->startTest->isChecked()) {
        quint16 id;
        bool ok;
        id = ui->remoteID->text().toULong(&ok, 16);
        if (!ok) {
            qDebug("TestDianvote::remoteStateClicked(): remote id should be based 16.");
            return;
        }
        isWork ?
            hidControl->start(id)
            : hidControl->stop(id);
    }
    else {
        QMessageBox::critical(0, "error", "test not started.");
        // release button.
        ui->remoteState->setChecked(false);
    }
}

void TestDianvote::getIDList()
{
    try {
        if (!hidControl) {
            hidControl = new HidControl(this);
        }
        // connect(hidControl, SIGNAL(idAmountComing(uint)), this, SLOT(showIDAmount(uint)));
        // get id list
        countID = 0;        // clear counter
        hidControl->GetIDList();
        ui->getIDList->setEnabled(false);

    } catch (DianVoteStdException *e) {
        ui->startTest->setChecked(false);
        QMessageBox::critical(0, "error", e->what());
        return;
    } catch (...) {
        ui->startTest->setChecked(false);
        QMessageBox::critical(0, "error", "unknow exception.");
        return;
    }
}

void TestDianvote::showID(quint16 id)
{
    QString msg;
    msg.append(QString("%L1: remote ID: ").arg(countID++, 4, 16, QChar('0')));
    msg.append(QString("0x%L1").
               arg(id, 4, 16, QChar('0')));

    ui->remoteCount->setText(QString("%L1").arg(countID));
    ui->rollcall->append(msg);
}

void TestDianvote::getIDListLength()
{
    try {
        if (!hidControl) {
            hidControl = new HidControl(this);
        }
        // connect(hidControl, SIGNAL(idComing(quint16)), this, SLOT(showID(quint16)));

        // get id amounts
        hidControl->GetIDListLength();
    } catch (DianVoteStdException *e) {
        ui->startTest->setChecked(false);
        QMessageBox::critical(0, "error", e->what());
        return;
    } catch (...) {
        ui->startTest->setChecked(false);
        QMessageBox::critical(0, "error", "unknow exception.");
        return;
    }
}

void TestDianvote::showIDAmount(uint amount)
{
    QString msg;
    msg.append(QString("The number of remote device: %L1").
               arg(amount, 3, 16, QChar('0')));
    ui->idAmount->setText(QString("%L1").arg(amount));
    ui->rollcall->append(msg);
}

void TestDianvote::idReceiveFinished()
{
    ui->getIDList->setEnabled(true);
}

void TestDianvote::DianVoteMsgHandler(QtMsgType type, const char *msg)
{
    QString log;
    switch (type) {
        case QtDebugMsg:
        {
            log = QString("Debug: %1").arg(msg);
            break;
        }

        case QtWarningMsg:
        {
            log = QString("Warning: %1").arg(msg);
            break;
        }

        case QtCriticalMsg:
        {
            log = QString("Critical: %1").arg(msg);
            break;
        }

        case QtFatalMsg:
        {
            log = QString("Fatal: %1").arg(msg);
            abort();
        }
    }
    message->append(log);
}

void TestDianvote::showIntenalMessage()
{
    if(message->length())
    {
        ui->message->append(message->first());
        message->removeFirst();
    }
}
