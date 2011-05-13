#ifndef __HIDCONTROL_H_
#define __HIDCONTROL_H_

#include <QObject>

class QHidDevice;

class HidControl : public QObject
{
    Q_OBJECT
public:
    explicit HidControl(QObject *parent = 0);
    ~HidControl();

signals:
	/**
	* @brief when vote data coming, this signal will be emit.
	*/
	void voteComing(quint32 id, quint8 option);

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

private:
    /**
     * @brief test whether the hid device is connect to computer or not
     */
    bool testDevice();

	/// the usb and PC id translate function.
	quint32 usbId2PCId(QByteArray);
    QByteArray PCId2usbId(quint32);

	QHidDevice *device;
};

#endif // #ifndef __HIDCONTROL_H_
