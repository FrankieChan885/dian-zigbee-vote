/**
* @file slideeditor.h
* @brief this is the QSlideEditor definition file.
* @author Tankery Chen @ Dian Group
* @version 1.0.0
* @date 2011-04-23
*/
#ifndef SLIDEEDITOR_H
#define SLIDEEDITOR_H

#include <QWidget>

class QString;
class QSlideScene;

class QSlideEditor : public QWidget
{
    Q_OBJECT
public:
    QSlideEditor(QString &modelStr, QWidget *parent = 0);

    void setBackgroundPixmap(const QString& imgPath);

signals:

public slots:
    /**
     * @brief save the slide content to file.
     */
    void save(const QString& filePath);

private:
    QSlideScene *slideScene;
};

#endif // SLIDEEDITOR_H

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

