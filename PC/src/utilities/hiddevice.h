/**
* @file hiddevice.h
* @brief class `QHidDevice' definition file.
*   using libhid to communicate with hid device.
* @author Tankery Chen @ Dian Group
* @version 1.0.0
* @date 2011-04-04
*/
#ifndef __HIDDEVICE_H_
#define __HIDDEVICE_H_

extern "C" {
#define HAVE_STDBOOL_H
#include <hid.h>
}
#include <QIODevice>
#include <queue>

typedef const unsigned int cuint;

class QTimer;
class QByteArray;

/**
* @brief this class is inherited from QIODevice,
*   using libhid to communicate with hid device.\n
*   this class will create an timer to get the data from device.\n
*   this class contains two fifo buffer to hold the in & out data.
*/
class QHidDevice : public QIODevice
{
Q_OBJECT

public:
    /**
    * @brief QHidDevice constructor with vid and pid.
    *
    * @param vid the device vendor ID.
    * @param pid the device product ID.
    */
    QHidDevice(unsigned short vid, unsigned short pid,
                unsigned short interface, unsigned short endpoint,
                QObject *parent = 0);
    ~QHidDevice();
    /**
    * @brief open will open a hid device.
    *
    * @param mode open mode.
    *
    * @return if find and openned, return true, or return false.
    */
    bool open(OpenMode mode);
    /**
    * @brief close close the hid device.
    *
    * @return 
    */
    void close();
    /**
    * @brief readData will read the data from the opened hid device.
    *
    * @param data the data array to read to.
    * @param len the data length.
    *
    * @return the number of bytes that were actually written,
    *   or -1 if an error occurred.
    */
    qint64 readData(char* data, qint64 len);
    /**
    * @brief writeData will write the data to the opened hid device.
    *
    * @param data the data array to write.
    * @param len the data length.
    *
    * @return the number of bytes that were actually written,
    *   or -1 if an error occurred.
    */
    qint64 writeData(const char* data, qint64 len);
    /**
    * @brief readAll read all date from hid device.
    *
    * @return the data from hid device.
    */
    QByteArray readAll();

signals:
    /**
     * @brief readInterrupt is a public signal that occured when in report 
     *   data is received.
     *
     * @param QByteArray the received data.
     */
    void readInterrupt(QByteArray inData);

private slots:
    /**
    * @brief scanUSBPort scanning the usb port and store the data.
    */
    void scanUSBPort();

private:
    static cuint SCAN_WAIT_TIMES = 2;    // the usb scanning wait times (ms).
    static cuint SCAN_TIME_SPACE = 8;    // the usb scanning interval (ms).
    static cuint FORCE_OPEN_TIMES = 3;
    static cuint PACKET_LENGHT = 2;       // the received data packet lenght.

    unsigned short vendorID;
    unsigned short productID;
    unsigned short interfaceNum;
    unsigned short endpointNum;

    HIDInterface *hid;
    /**
    * @brief for usb scanning.
    */
    QTimer *scanTimer;

}; // class RemoteCmder

#endif // __HIDDEVICE_H_

/* Copyright (C) 
* 2011 - Tankery Chen @ Dian Group
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
* 
*/

