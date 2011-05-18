/**
* @file hidlistener.cpp
* @brief the QHidListener implementation file.
* @author Tankery Chen @ Dian Group
* @version 1.0.0
* @date 2011-04-15
*/
#include <QByteArray>
#include <QThread>

#include "hidlistener.h"

QHidListener::QHidListener(
#ifdef USE_LIBHID
        HIDInterface *hid,
        unsigned short endpoint,
#else // #ifdef USE_LIBHID
        hid_device *hid,
#endif // #ifdef USE_LIBHID
        unsigned int length, QObject *parent/* = 0*/)
: QThread(parent)
, hidInterface(hid)
#ifdef USE_LIBHID
, endpointNum(endpoint)
#endif // #ifdef USE_LIBHID
, dataLength(length)
, needStop(false)
{
}

QHidListener::~QHidListener()
{
}

void QHidListener::run() {
    exec();
}
/**
 * @brief start start the listener with time set.
 */
void QHidListener::exec()
{
    char *packet = new char[dataLength];
    qDebug("QHidListener::exec(): started listening...");

#ifdef USE_LIBHID
    while (!needStop) {
        hid_return ret = hid_interrupt_read(hidInterface, endpointNum,
                packet, dataLength, INT_WAIT_TIME);

        // if read success, emit read interrupt.
        if (ret == HID_RET_SUCCESS) {
            emit hidDataReceived(QByteArray(packet, dataLength));
        }
        else if (ret != HID_RET_TIMEOUT) {
            qDebug(hid_strerror(ret));
        }
    }
#else // #ifdef USE_LIBHID
    // don't blocking, or the read can't stop unless data received.
	hid_set_nonblocking(hidInterface, 1);

    while (!needStop) {
        int ret = hid_read(hidInterface,
                           (unsigned char *) packet, dataLength,
                           INT_WAIT_TIME);
        if (ret > 0) {
			quint16 id[2];
			memcpy(id, packet, 4);
			qDebug("data received from: 0x%04x%04x with %d",
				   id[0], id[1], packet[dataLength-1]);
            emit hidDataReceived(QByteArray(packet, dataLength));
        }
        else if (ret < 0) {
            qDebug("QHidListener::exec(): hid read failed...");
            usleep(100);
			//break;
        }
    }
#endif // #ifdef USE_LIBHID

    delete [] packet;
}

/**
 * @brief stop stop the listener.
 */
void QHidListener::stop()
{
    needStop = true;
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
