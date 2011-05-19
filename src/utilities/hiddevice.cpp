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
#include <iostream>
#include <queue>
#include "exceptions.h"
#include "hidlistener.h"

/**
 * @brief QHidDevice constructor with vid and pid.
 *
 * @param vid the device vendor ID.
 * @param pid the device product ID.
 * @param interface the interface number.
 */
QHidDevice::QHidDevice(unsigned short vid, unsigned short pid,
#ifdef USE_LIBHID
            unsigned short interface,
#endif // #ifdef USE_LIBHID
            QObject *parent/* = 0*/)
: QIODevice(parent)
, vendorID(vid)
, productID(pid) 
#ifdef USE_LIBHID
, interfaceNum(interface)
#endif // #ifdef USE_LIBHID
, hid(0)
, hidListener(0)
{
    if (!mutex.tryLock(500))
    {
        qDebug("QHidDevice::QHidDevice(): hid constructor locked...");
        throw new DianVoteStdException(
            std::string("QHidDevice::QHidDevice(): hid constructor locked..."));
    }
    
#ifdef USE_LIBHID
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
#endif // #ifdef USE_LIBHID
    mutex.unlock();
}

QHidDevice::~QHidDevice()
{
    // if device open, close it first.
    if (isOpen()) {
        qDebug("QHidDevice::~QHidDevice(): hid is open...");
        close();
    }

#ifdef USE_LIBHID
    // delete interface and clean up the hids.
    hid_delete_HIDInterface(&hid);

    hid_return ret = hid_cleanup();
    if (ret != HID_RET_SUCCESS) {
        throw new DianVoteStdException(
            std::string(hid_strerror(ret)));
    }
#endif // #ifdef USE_LIBHID
}

/**
 * @brief open will open a hid device.
 *
 * @param endpoint the endpoint number.
 * @param dataLength is the specific length of received data.
 * @param mode open mode.
 *
 * @return if find and openned, return true, or return false.
 */
bool QHidDevice::open(OpenMode mode) {
    if (!mutex.tryLock(500))
    {
        qDebug("QHidDevice::open(): hid open locked...");
        return false;
    }
    
#ifdef USE_LIBHID
    if (hid_is_opened(hid)) {
        std::cerr << "Error in QHidDevice::open: device already open"
            << std::endl;
        mutex.unlock();
        return false;
    }

    // construct the matcher struct.
    HIDInterfaceMatcher matcher = 
        { vendorID, productID, NULL, NULL, 0 };

    // force open it.
    hid_return ret = hid_force_open(hid, interfaceNum, &matcher,
            FORCE_OPEN_TIMES);
    if (ret != HID_RET_SUCCESS) {
        // close and throw an error.
        close();
        mutex.unlock();
        throw new DianVoteStdException(
                std::string(hid_strerror(ret)));

        return false;
    }
#else // #ifdef USE_LIBHID
    // Open the device using the VID, PID,
    // and optionally the Serial number.
    hid = hid_open(vendorID, productID, NULL);

    if (hid == NULL) {
        qDebug("QHidDevice::open: hid opend failed...");
        mutex.unlock();
        return false;
    }
#endif // #ifdef USE_LIBHID
    qDebug("QHidDevice::open: hid opened...");

    mutex.unlock();

    return QIODevice::open(mode);
}

/**
 * @brief close close the hid device.
 */
void QHidDevice::close() {
    if (!mutex.tryLock(500))
    {
        qDebug("QHidDevice::close(): hid close locked...");
        return;
    }
    
    // first clear the listener.
    if (hidListener) {
        // stop and wait listener exit.
        hidListener->stop();
        hidListener->wait();

        delete hidListener;
        hidListener = 0;
    }

    // then close the hid device.
#ifdef USE_LIBHID
    if (hid_is_opened(hid)) {
        hid_return ret = hid_close(hid);
        if (ret != HID_RET_SUCCESS) {
            mutex.unlock();
            throw new DianVoteStdException(
                    std::string(hid_strerror(ret)));
            return;
        }
    }
#else // #ifdef USE_LIBHID

    if (hid != 0) {
        hid_close(hid);
    }
#endif // #ifdef USE_LIBHID

    qDebug("QHidDevice::close(): hid closed...");
    setOpenMode(QIODevice::NotOpen);

    mutex.unlock();
    return;
}

/**
 * @brief startListening start listen the specific ep with data length.
 *
 * @param endpoint the endpoint number.
 * @param dataLength is the specific length of received data.
 */
void QHidDevice::startListening(
#ifdef USE_LIBHID
                                unsigned short endpoint,
#endif // #ifdef USE_LIBHID
                                unsigned int dataLength)
{
    if (!mutex.tryLock(500))
    {
        qDebug("QHidDevice::startListening(): hid listening locked...");
        return;
    }

    // start listening when listener not initialized.
    if (!hidListener) {
        // new a listener.
        hidListener = new QHidListener(hid,
#ifdef USE_LIBHID
            endpoint,
#endif // #ifdef USE_LIBHID
            dataLength, this);
        if (hidListener) {
            qDebug("QHidDevice::startListening(): hidListener created...");
        }

        // call readInterrupt if hid data received.
        QObject::connect(hidListener, SIGNAL(hidDataReceived(QByteArray)),
            this, SIGNAL(readInterrupt(QByteArray)));

        // start it.
        hidListener->start();
    }

    mutex.unlock();
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
qint64 QHidDevice::readData(char*, qint64) {
    return -1;
}

/**
 * @brief writeData will write the data to the opened hid device.
 *
 * @param data the data array to write.
 * @param len the data length.
 *
 * @return the number of bytes that were actually written,
 *   or -1 on error.
 */
qint64 QHidDevice::writeData(const char* data, qint64 len) {
    if (!mutex.tryLock(500))
    {
        qDebug("QHidDevice::writeData(): hid write locked...");
        return -1;
    }

#ifdef USE_LIBHID
    mutex.unlock();
    return -1;
#else // #ifdef USE_LIBHID
 	// send control command.
    unsigned char *buf = new unsigned char[len + 1];
    // must contain a report ID. if single report, set it 0.
    buf[0] = 0;
    memcpy(buf+1, data, len);
    // note!! don't using sizeof(buf) as buffer's length!
    // it will be the pointer length, 4.
 	int res = hid_write(hid, buf, len + 1);
 	if (res < 0) {
        qDebug("QHidDevice::writeData(): Unable to write()...");
 	}
    else {
        qDebug("QHidDevice::writeData(): %d data writed...", res);
    }
    delete [] buf;

    mutex.unlock();
    return res;
#endif // #ifdef USE_LIBHID
}

/**
 * @brief readAll read all date from hid device.
 *
 * @return the data from hid device.
 */
QByteArray QHidDevice::readAll() {
    return QByteArray("");
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

