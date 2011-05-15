#include <QTimer>
#include "hidcontrol.h"
#include "hiddevice.h"
#include "exceptions.h"


HidControl::HidControl(QObject *parent)
: QObject(parent)
, device(0)
, stopOnReceive(false)
{
	if (device == 0) {
		device = new QHidDevice(HID_VID, HID_PID, this);
		connect(device, SIGNAL(readInterrupt(QByteArray)),
				this, SLOT(dataReceived(QByteArray)));
	}
    testDevice();   // try to open hid device
                    // throw exception when failed
}

HidControl::~HidControl()
{
    stop();

	if (device) {
		delete device;
		device = 0;
	}
}

/**
 * @brief start a specific remote by its id.
 * 		the default remote id is a broadcast id.
 */
void HidControl::start(quint32 id/* = 0xffffffff*/)
{
    if (0xffffffff == id) {
        if (!device->isOpen()) {
            if (!device->open(QIODevice::ReadWrite)) {
                throw new DianVoteStdException("hid open failed...");
            }

            // start listening the endpoint 1 with data length 5
            // (in windows length should be 6, I don't know why).
            device->startListening(6);
        }
    }

	// send start signal to remote
	char data[5];
	memcpy(data, PCId2usbId(id).data(), 4);
	data[4] = CMD_WORK;
	device->writeData(data, 5);
}

/**
 * @brief stop a specific remote by its id.
 * 		the default remote id is a broadcast id.
 */
void HidControl::stop(quint32 id/* = 0xffffffff*/)
{
    if (device->isOpen())
    {
        // send stop signal to remote
        char data[5];
        memcpy(data, PCId2usbId(id).data(), 4);
        data[4] = CMD_SLEEP;
        device->writeData(data, 5);

        // if broadcast, means stop all device.
        if (0xffffffff == id) {
            device->close();
        }
    }
}

/**
* @brief test whether the hid device is connect to computer or not
*/
bool HidControl::testDevice()
{
    if(device->isOpen()) {
        device->close();
        return true;
    }
    else {
        if(!device->open(QIODevice::ReadWrite)) {
            throw new DianVoteStdException("hid open failed...");
        }
        else {
            device->close();
            return true;
        }
    }
    return false;
}

/**
 * @brief the data parsing function, called when data coming.
 */
void HidControl::dataReceived(QByteArray ba)
{
    quint32 id = usbId2PCId(ba);
    quint8 option = (quint8) ba.at(4);

    emit voteComing(id, option);

	// when received succeed, stop this id.
	if (stopOnReceive) {
		stop(id);
	}
}

/**
* @brief roll-call to get the number of remote.
*/
void HidControl::startRollCall(ulong time /*= 3000*/)
{
    // reset remoteMap;
    remoteMap.clear();

    // first create the hid.
    if (!device) {
        device =
            new QHidDevice(HidControl::HID_VID,
            HidControl::HID_PID,
            this);
    }
    // reconnect signal.
    disconnect(device, 0, 0, 0);
    connect(device, SIGNAL(readInterrupt(QByteArray)),
        this, SLOT(rollCallReplied(QByteArray)));

    // then open and listening it.
    if (!device->isOpen())
    {
        if (!device->open(QIODevice::ReadWrite)) {
            throw new DianVoteStdException("hid open failed...");
        }

        // start listening the endpoint 1 with data length 5
        // (in windows length should be 6, I don't know why).
        device->startListening(6);
    }

    // send an broadcast command to roll-call.
    // 2 is the command for roll-call.
    char cmd[5];
    memset(cmd, 0xff, 4);
    cmd[4] = CMD_ROLLCALL;
    device->writeData(cmd, 5);

    // wait for roll-call reply.
    QTimer::singleShot(time, this, SLOT(rollCallTimeOut()));
}

/**
* @brief roll-call to get the number of remote.
*/
void HidControl::rollCallReplied(const QByteArray& ba)
{
	if ((quint8)ba.at(4) == 0xff) {
		remoteMap[usbId2PCId(ba)] = ba.at(4);
	}
}

void HidControl::rollCallTimeOut() {
    disconnect(device, 0, 0, 0);
    connect(device, SIGNAL(readInterrupt(QByteArray)),
        this, SLOT(dataReceived(QByteArray)));
    emit rollCallFinished(remoteMap.size());

//    // close device.
//    if (device->isOpen())
//    {
//        device->close();
//    }
}

/// the USB and PC id translate function.
quint32 HidControl::usbId2PCId(const QByteArray& ba)
{
    quint16 id[2];
    memcpy (id, ba.data(), 4);
    return quint32((id[0] << 16) | id[1]);
}

QByteArray HidControl::PCId2usbId(const quint32& id)
{
	quint16 usbId[2];
	usbId[0] = quint16(id >> 16);
    usbId[1] = quint16(id & 0xffff);

	return QByteArray((char*)usbId, 4);
}
