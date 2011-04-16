/**
* @file hidlistener.cpp
* @brief the QHidListener implementation file.
* @author Tankery Chen @ Dian Group
* @version 1.0.0
* @date 2011-04-15
*/
#include <iostream>
#include <string>
#include <QByteArray>
#include <QThread>

#include "hidlistener.h"
#include "exceptions.h"

QHidListener::QHidListener(HIDInterface *hid, unsigned short endpoint,
        unsigned int length, QObject *parent/* = 0*/)
: QThread(parent)
, hidInterface(hid)
, endpointNum(endpoint)
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

    while (!needStop) {
        hid_return ret = hid_interrupt_read(hidInterface, endpointNum,
                packet, dataLength, INT_WAIT_TIME);

        // if read success, emit read interrupt.
        if (ret == HID_RET_SUCCESS) {
            emit hidDataReceived(QByteArray(packet, dataLength));
        }
        else if (ret != HID_RET_TIMEOUT) {
            throw new DianVoteStdException(
                    std::string(hid_strerror(ret)));
        }
    }

    delete packet;
}

/**
 * @brief stop stop the listener.
 */
void QHidListener::stop()
{
    needStop = true;
}

