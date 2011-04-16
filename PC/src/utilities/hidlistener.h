/**
* @file hidlistener.h
* @brief the class QHidListener definition file.
* @author Tankery Chen @ Dian Group
* @version 1.0.0
* @date 2011-04-15
*/
#ifndef __HISLISTENER_H_
#define __HISLISTENER_H_

extern "C" {
#define HAVE_STDBOOL_H
#include <hid.h>
}
#include <QThread>

class QByteArray;

class QHidListener : public QThread
{
    Q_OBJECT

public:
    QHidListener(HIDInterface *hid, unsigned short endpoint,
        unsigned int length, QObject *parent = 0);
    ~QHidListener();

    /**
    * @brief stop stop the listener.
    */
    void stop();

signals:
    void hidDataReceived(QByteArray inData);

protected:
    /**
    * @brief run start the listener.
    */
    void run();
    void exec();

private:
    static const int INT_WAIT_TIME = 100;
    HIDInterface* hidInterface;
    unsigned short endpointNum;
    unsigned int dataLength;        // the received data packet lenght.
    bool needStop;
};

#endif // __HISLISTENER_H_

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

