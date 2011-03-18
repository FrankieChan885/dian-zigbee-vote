/**
 * @file slidewidget.h
 * @brief 
 *  this is the QSlideWidget class definition file.
 *
 * @author Tankery tankery.chen@gmail.com
 * @version 1.0.0.0
 * @date 2011-03-18
 */
/*
 * Copyright (C) 2011 - Tankery tankery.chen@gmail.com
 *
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
#ifndef __SLIDEWIDGET_H_
#define __SLIDEWIDGET_H_

#include <QWidget>
#include <QString>

/**
 * @brief 
 *  QSlideWidget is a widget that shows slide with topics in content.
 *  You can put this class in any parent widget to let the parent desplaying
 *  the content of the topics.
 */
class QSlideWidget : public QWidget
{
Q_OBJECT

public:
    /**
     * @brief QSlideWidget initial the widget with some attribute.
     *
     * @param editable
     *  set the state of QSlideWidget that limits the changes of it.
     * @param parent
     *  the QSlideWidget's container.
     * @param f
     *  the window style of QSlideWiget.
     */
    QSlideWidget(bool editable = false, QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~QSlideWidget();

    /**
    * @brief setDocument set the document of this widget view.
    *
    * @param sd the slide document that holding the topic content.
    */
    void setDocument(QSlideDoc *sd) {
        slideDoc = sd;
    }
    /**
    * @brief getDocument get the slide document
    *
    * @return slide document
    */
    QSlideDoc *getDocument() {
        return slideDoc;
    }

private:
    /**
    * @brief the slide document that holding the topic content.
    */
    QSlideDoc *slideDoc;
};

#endif // __SLIDEWIDGET_H_
