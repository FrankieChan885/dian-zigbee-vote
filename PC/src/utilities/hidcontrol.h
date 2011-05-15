#ifndef __HIDCONTROL_H_
#define __HIDCONTROL_H_

#include <QObject>
#include <map>

class QHidDevice;

class HidControl : public QObject
{
    Q_OBJECT
public:
    enum {
        CMD_SLEEP       = 0,
        CMD_WORK        = 1,
        CMD_ROLLCALL    = 2
    };
    static const quint16 HID_VID = 0x0451;
    static const quint16 HID_PID = 0x16a9;

    explicit HidControl(QObject *parent = 0);
    ~HidControl();

	/// the usb and PC id translate function.
	static quint32 usbId2PCId(const QByteArray&);
    static QByteArray PCId2usbId(const quint32&);

	void setStopOnReceive(bool needStop) {
		stopOnReceive = needStop;
	}
	bool needStopOnReceive() {
		return stopOnReceive;
	}

signals:
	/**
	* @brief when vote data coming, this signal will be emit.
	*/
	void voteComing(quint32 id, quint8 option);

    void rollCallFinished(uint count);

public slots:
	/**
	* @brief start a specific remote by its id.
	* 		the default remote id is a broadcast id.
	*/
	void start(quint32 id = 0xffffffff);
	/**
	* @brief stop a specific remote by its id.
	* 		the default remote id is a broadcast id.
	*/
	void stop(quint32 id = 0xffffffff);
	/**
	* @brief the data parsing function, called when data coming.
	*/
	void dataReceived(QByteArray ba);

    /**
    * @brief roll-call to get the number of remote.
    */
    void startRollCall(ulong time = 3000);
    void rollCallReplied(const QByteArray&);
    void rollCallTimeOut();

private:
    /**
     * @brief test whether the hid device is connect to computer or not
     */
    bool testDevice();

	QHidDevice *device;
    uint remoteCount;
	bool stopOnReceive;
    // this is a map to containing the remote ID an option.
    std::map <quint32, quint8> remoteMap;
};

#endif // #ifndef __HIDCONTROL_H_
