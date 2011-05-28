#include <QTimer>
#include "hidcontrol.h"
#include "hiddevice.h"
#include "exceptions.h"


HidControl::HidControl(QObject *parent)
: QObject(parent)
, device(0)
, bitmapSliceCounter(0)
{
    if (device == 0) {
		device = new QHidDevice(HID_VID, HID_PID, this);
		connect(device, SIGNAL(readInterrupt(QByteArray)),
				this, SLOT(dataReceived(QByteArray)));
	}
    connect(this, SIGNAL(idMapReceiveFinished()), this, SLOT(DoReceiveBitMapFinished()));

                    // try to open hid device
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
* @brief open control.
*/
void HidControl::open() {
    ensureDeviceOpened();
}

/**
 * @brief start a specific remote by its id.
 * 		the default remote id is a broadcast id.
 */
void HidControl::start(quint16 id/* = 0xffff*/)
{
    ensureDeviceOpened();

	// send start signal to remote
    char data[3];
    memcpy(data, PCId2usbId(id).data(), 2);
    data[2] = PC_START_HID_DEVICE;
    device->writeData(data, 3);
}

/**
 * @brief stop a specific remote by its id.
 * 		the default remote id is a broadcast id.
 */
void HidControl::stop(quint16 id/* = 0xffff*/)
{
    if (device->isOpen())
    {
        // send stop signal to remote
        char data[3];
        memcpy(data, PCId2usbId(id).data(), 2);
        data[2] = PC_STOP_HID_DEVICE;
        device->writeData(data, 3);

        // if broadcast, means stop all device.
        if (0xffff == id) {
//            device->close();
        }
    }
}

/**
 * @brief the data parsing function, called when data coming.
 */
void HidControl::dataReceived(QByteArray ba)
{
    quint16 data;
    quint8 flag = (quint8) ba.at(2);

    switch(flag) {
    case ID_MAP_FLAG: {
            // data is part a remote device id map, accurately 1/32
            remoteDeviceBitmap[bitmapSliceCounter++] = ba.at(0);
            remoteDeviceBitmap[bitmapSliceCounter++] = ba.at(1);
            if(bitmapSliceCounter == DEVICE_ID_BITMAP_SIZE_IN_BYTE)
            {
                bitmapSliceCounter = 0;
                emit idMapReceiveFinished();
            }
            break;
        }

    case ID_AMOUNT_FLAG: {
            memcpy(&data, ba.data(), 2);
            // data is the number of remote devices
            emit idAmountComing(uint(data));
            break;
        }

        // control message received

    case CONTROL_START_OR_STOP: {
        }

    case CONTROL_SHOW_OR_HIDE_CONTROLER: {
        }

    case CONTROL_SHOW_OR_HIDE_RESULT: {

        }

    case CONTROL_PREVIOUS_SLIDE: {

        }

    case CONTROL_NEXT_SLIDE: {
            data = usbId2PCId(ba);
            // data is remote device id, flag is the option
            emit controlMessageComming(data, flag);
            break;
        }

    default:
        {
            data = usbId2PCId(ba);
            // data is remote device id, flag is the option
            emit voteComing(data, flag);
            break;
        }
    }
}

/**
* @brief get the id list of remote devices.
*/
void HidControl::GetIDList()
{
    // reset remoteMap;
    remoteMap.clear();

    // device must opened
    ensureDeviceOpened();

    // send an message command to get id list.
    // PC_GET_ID_LIST is the command for getting id list.
    char cmd[3];
    memset(cmd, 0xff, 2);
    cmd[2] = PC_GET_ID_LIST;
    device->writeData(cmd, 3);
}

/**
 * @brief sent a message to HidDevice, tell it PC want the length of the id list,equal device amount
 */
void HidControl::GetIDListLength()
{
    // device must opened
    ensureDeviceOpened();

    // send an message command to get id list.
    // PC_GET_ID_LIST_LENGTH is the command for getting id list.
    char cmd[3];
    memset(cmd, 0xff, 2);
    cmd[2] = PC_GET_ID_LIST_LENGTH;
    device->writeData(cmd, 3);
}

/**
 * @brief when id bitmap receive finished, call this
 */
void HidControl::DoReceiveBitMapFinished()
{
    quint8 id = 0;
    while (id = findNextAddrFrom((quint8*)(remoteDeviceBitmap), id, 1)) {
        emit idComing(quint16(id));
        remoteMap[quint16(id)] = 0;
    }
    emit idSentFinished();
}

/**
 * @brief make sure that the USB device is open
 */
void HidControl::ensureDeviceOpened()
{
    // first create the hid.
    if (!device) {
        device =
            new QHidDevice(HidControl::HID_VID,
            HidControl::HID_PID,
            this);
    }

    // then open and listening it.
    if (!device->isOpen())
    {
        if (!device->open(QIODevice::ReadWrite)) {
            throw new DianVoteStdException("hid open failed...");
        }

        // start listening the endpoint 1 with data length 3
        // (in windows length should be 4, I don't know why).
        device->startListening(4);
    }
}

/// DianVoteControl need the device map
std::map <quint16, quint8>* HidControl::GetMap()
{
    return &remoteMap;
}

/// the USB and PC id translate function.
quint16 HidControl::usbId2PCId(const QByteArray& ba)
{
    quint16 id;
    memcpy(&id, ba.data(), 2);
    return id;
}

QByteArray HidControl::PCId2usbId(const quint16& id)
{
    quint16 usbId[1];
    usbId[0] = id;

    return QByteArray((char*)usbId, 2);
}

quint16 HidControl::findNextAddrFrom(quint8* bmp, quint8 addr, quint8 isSet)
{
    quint8 byte = (quint8)addr/8;
    quint8 bit = (addr % 8) + 1; // from next addr.
    for (; byte < DEVICE_ID_BITMAP_SIZE_IN_BYTE; byte++) {
        // only check when byte has 1.
        if (isSet? bmp[byte] : !bmp[byte]) {
            while(0x80 >> bit) {
                if (isSet) {
                    if (bmp[byte] & (0x80 >> bit)) {
                        return (byte*8 + bit);
                    }
                }
                else {
                    if (!(bmp[byte] & (0x80 >> bit))) {
                        return (byte*8 + bit);
                    }
                }
                bit++;
            }
        }
        bit = 0;
    }
    return 0;
}
