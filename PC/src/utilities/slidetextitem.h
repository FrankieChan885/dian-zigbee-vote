/**
* @file slidetextitem.h
* @brief this is the class QSlideTextItem definition file.
* @author Tankery Chen @ Dian Group
* @version 1.0.0
* @date 2011-04-23
*/
#ifndef SLIDETEXTITEM_H
#define SLIDETEXTITEM_H

#include <QGraphicsTextItem>

class QString;
class QFont;
class QColor;

/**
 * @brief this class inherit frome QGraphicsTextItem,
 *       overloading a event function: focusOutEvent(),
 *       and adding a signal: lostFocus();
 *   this class can emit lostFocus when focusOutEvent occur.
 */
class QSlideTextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    QSlideTextItem(QGraphicsItem *parent = 0);
    QSlideTextItem(const QString & text, QGraphicsItem *parent = 0);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

signals:
    void lostFocus(QGraphicsTextItem *item);
    void selectedChange(QGraphicsTextItem *item, bool isSelected);

public slots:
    void changeFont(const QFont &font);
    void changeColor(const QColor &color);
};

#endif // SLIDETEXTITEM_H

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

