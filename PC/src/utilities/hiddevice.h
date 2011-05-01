/**
* @file hiddevice.h
* @brief class `QHidDevice' definition file.
*   using libhid or hidapi to communicate with hid device.
* @author Tankery Chen @ Dian Group
* @version 1.0.0
* @date 2011-04-04
*/
#ifndef __HIDDEVICE_H_
#define __HIDDEVICE_H_

#include <QIODevice>
#include <queue>

extern "C" {
#ifdef USE_LIBHID
#define HAVE_STDBOOL_H
#include "hid.h"
#else // #ifdef USE_LIBHID
#include "hidapi.h"
#endif // #ifdef USE_LIBHID
}

typedef const unsigned int cuint;

class QByteArray;
class QHidListener;

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
    * @param interface the interface number.
    */
    QHidDevice(unsigned short vid, unsigned short pid,
#ifdef USE_LIBHID
            unsigned short interface,
#endif // #ifdef USE_LIBHID
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
    * @brief startListening start listen the specific ep with data length.
    *
    * @param endpoint the endpoint number.
    * @param dataLength is the specific length of received data.
    */
    void startListening(unsigned short endpoint, unsigned int dataLength);

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

private:
    static cuint FORCE_OPEN_TIMES = 3;

    unsigned short vendorID;
    unsigned short productID;
#ifdef USE_LIBHID
    unsigned short interfaceNum;

    HIDInterface *hid;
#else
    hid_device *hid;
#endif
    /**
    * @brief listen the hid data receiving.
    */
    QHidListener *hidListener;

}; // class QHidDevice

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

