#include "hidcontrol.h"
#include "hiddevice.h"
#include "exceptions.h"

HidControl::HidControl(QObject *parent)
: QObject(parent)
, device(0)
{
	if (device == 0) {
		device = new QHidDevice(0x0451, 0x16a9, this);
		connect(device, SIGNAL(readInterrupt(QByteArray)),
				this, SLOT(dataReceived(QByteArray)));
	}
}

HidControl::~HidControl()
{
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
        if (!device->isOpen())
        {
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
    data[4] = 0x01;
    device->writeData(data, 5);
}

/**
 * @brief stop a specific remote by its id.
 * 		the default remote id is a broadcast id.
 */
void HidControl::stop(quint32 id/* = 0xffffffff*/)
{
	// send stop signal to remote
	char data[5];
	memcpy(data, PCId2usbId(id).data(), 4);
	data[4] = 0x00;
	device->writeData(data, 5);

    // if broadcast, means stop all device.
    if (0xffffffff == id) {
        device->close();
    }
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
    //stop(id);
}

/// the usb and PC id translate function.
quint32 HidControl::usbId2PCId(QByteArray ba)
{
    quint16 id[2];
    memcpy (id, ba.data(), 4);
    return quint32((id[0] << 16) | id[1]);
}

QByteArray HidControl::PCId2usbId(quint32 id)
{
	quint16 usbId[2];
	usbId[0] = quint16(id >> 16);
    usbId[1] = quint16(id & 0xffff);

	return QByteArray((char*)usbId, 4);
}
