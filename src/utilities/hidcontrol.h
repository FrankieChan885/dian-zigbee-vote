#ifndef __HIDCONTROL_H_
#define __HIDCONTROL_H_

#include <QObject>
#include <map>

class QHidDevice;

/**
 * @brief message definitiond
 */
#define ID_MAP_FLAG                 0xff        // message which has this flag is a device ID
#define ID_AMOUNT_FLAG              0xf0        // message which has this flag is the amount of device
#define PC_GET_ID_LIST              0xf2        // get total voter id
#define PC_GET_ID_LIST_LENGTH       0xf1        // get total voter nums
#define PC_STOP_HID_DEVICE          0x01        // stop vote
#define PC_START_HID_DEVICE         0x02        // start vote

/**
 * @brief regular definition
 */
#define DEVICE_ID_BITMAP_SIZE_IN_WORD   16          // the device id bitmap size is 32 words
#define DEVICE_ID_BITMAP_SIZE_IN_BYTE   32          // the device id bitmap size is 32 bytes

class HidControl : public QObject
{
    Q_OBJECT
public:
    static const quint16 HID_VID = 0x0451;
    static const quint16 HID_PID = 0x16a9;

    explicit HidControl(QObject *parent = 0);
    ~HidControl();

	/// the usb and PC id translate function.
    static quint16 usbId2PCId(const QByteArray&);
    static QByteArray PCId2usbId(const quint16&);

signals:
	/**
	* @brief when vote data coming, this signal will be emit.
	*/
    void voteComing(quint16 id, quint8 option);

    /**
     * @brief when id bitmap slice coming, this signal will be emit.
     *        USB need sent 32 message to complete the transmition
     */
    void idComing(quint16 id);

    /**
     * @brief when id bitmap sent completed, this signal will be emit.
     */
    void idMapReceiveFinished();

    /**
     * @brief when id sent finished, this signal will be emit.
     */
    void idSentFinished();

    /**
     * @brief when (the number of id) data coming, this signal will be emit.
     */
    void idAmountComing(uint count);

//    void rollCallFinished(uint count);

public slots:
    /**
    * @brief open control.
    */
    void open();
	/**
	* @brief start a specific remote by its id.
	* 		the default remote id is a broadcast id.
	*/
    void start(quint16 id = 0xffff);
	/**
	* @brief stop a specific remote by its id.
	* 		the default remote id is a broadcast id.
	*/
    void stop(quint16 id = 0xffff);
	/**
	* @brief the data parsing function, called when data coming.
	*/
	void dataReceived(QByteArray ba);

    /**
    * @brief sent a message to HidDevice, tell it PC want the voter ID bitmap.
    */
    void GetIDList();

    /**
     * @brief sent a message to HidDevice, tell it PC want the length of the id list,equal device amount
     */
    void GetIDListLength();

    /**
     * @brief when id bitmap receive finished, call this
     */
    void DoReceiveBitMapFinished();

private:

    /**
     * @brief make sure that the USB device is open
     */
    void ensureDeviceOpened();

    /**
     * @brief find the next address, which mask value equal isSet from bmp
     */
    quint16 findNextAddrFrom(quint8* bmp, quint8 addr, quint8 isSet);

    QHidDevice *device;
    // bitmap of the remote device
    quint8 remoteDeviceBitmap[DEVICE_ID_BITMAP_SIZE_IN_BYTE];
    // bitmap slice counter, USB need send 32 messages until the bitmap transmit finished
    quint8 bitmapSliceCounter;
    // this is a map to containing the remote ID an option.
    std::map <quint16, quint8> remoteMap;
};

#endif // #ifndef __HIDCONTROL_H_
