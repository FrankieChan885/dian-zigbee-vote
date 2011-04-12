/**
* @file hiddevice.cpp
* @brief class `QHidDevice' implementation file.
* @author Tankery Chen @ Dian Group
* @version 1.0.0
* @date 2011-04-05
*/
#include "hiddevice.h"

#include <QTimer>
#include <QByteArray>
#include <string>
#include "../utilities/exceptions.h"

/**
 * @brief QHidDevice constructor with vid and pid.
 *
 * @param vid the device vendor ID.
 * @param pid the device product ID.
 */
QHidDevice::QHidDevice(unsigned short vid, unsigned short pid,
        unsigned short interface, unsigned short endpoint,
        QObject *parent/* = 0*/)
: QIODevice(parent)
, vendorID(vid)
, productID(pid) 
, interfaceNum(interface)
, endpointNum(endpoint)
, scanTimer(new QTimer(this))
{
    // debug configuration.
    hid_set_debug(HID_DEBUG_NOTRACES);
    hid_set_debug_stream(stderr);
    hid_set_usb_debug(0);

    // init hid.
    hid_return ret = hid_init();
    if (ret != HID_RET_SUCCESS) {
        throw new DianVoteStdException(
            std::string(hid_strerror(ret)));
    }

    // create an new hid inerface for device.
    hid = hid_new_HIDInterface();
    if (hid == 0) {
        throw new DianVoteStdException(
            std::string("hid_new_HIDInterface() failed, out of memory?"));
    }

    // Connecting the timer timeout() signal to scanUSBPort() slot.
    QObject::connect( scanTimer, SIGNAL(timeout()),
            this, SLOT(scanUSBPort()) );
}

QHidDevice::~QHidDevice()
{
    // delete interface and clean up the hids.
    hid_delete_HIDInterface(&hid);

    hid_return ret = hid_cleanup();
    if (ret != HID_RET_SUCCESS) {
        throw new DianVoteStdException(
            std::string(hid_strerror(ret)));
    }
}

/**
 * @brief open will open a hid device.
 *
 * @param mode open mode.
 *
 * @return if find and openned, return true, or return false.
 */
bool QHidDevice::open(OpenMode mode) {
    // construct the matcher struct.
    HIDInterfaceMatcher matcher = 
        { vendorID, productID, NULL, NULL, 0 };

    // force open it.
    hid_return ret = hid_force_open(hid, endpointNum, &matcher,
            FORCE_OPEN_TIMES);
    if (ret != HID_RET_SUCCESS) {
        throw new DianVoteStdException(
                std::string(hid_strerror(ret)));

        return false;
    }

    // when open succed, start the timer.
    scanTimer->start(SCAN_TIME_SPACE);

    return QIODevice::open(mode);
}

/**
 * @brief close close the hid device.
 *
 * @return 
 */
void QHidDevice::close() {
    hid_return ret = hid_close(hid);
    if (ret != HID_RET_SUCCESS) {
        throw new DianVoteStdException(
                std::string(hid_strerror(ret)));
        return;
    }

    // when close succed, stop the timer.
    scanTimer->stop();

    return;
}

/**
 * @brief readData will read the data from the opened hid device.
 *
 * @param data the data array to read to.
 * @param len the data length.
 *
 * @return the number of bytes that were actually written,
 *   or -1 if an error occurred.
 */
qint64 QHidDevice::readData(char* data, qint64 len) {
    return -1;
}

/**
 * @brief writeData will write the data to the opened hid device.
 *
 * @param data the data array to write.
 * @param len the data length.
 *
 * @return the number of bytes that were actually written,
 *   or -1 if an error occurred.
 */
qint64 QHidDevice::writeData(const char* data, qint64 len) {
    return -1;
}

/**
 * @brief readAll read all date from hid device.
 *
 * @return the data from hid device.
 */
QByteArray QHidDevice::readAll() {
    return QByteArray("");
}

/**
 * @brief scanUSBPort scanning the usb port and store the data.
 */
void QHidDevice::scanUSBPort() {
    char packet[PACKET_LENGHT];
    hid_return ret = hid_interrupt_read(hid, endpointNum,
            packet, PACKET_LENGHT, SCAN_WAIT_TIMES);
    if (ret != HID_RET_SUCCESS) {
        throw new DianVoteStdException(
                std::string(hid_strerror(ret)));
        return;
    }

    emit readInterrupt(QByteArray(packet, PACKET_LENGHT));
}

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

