/**
* @file slidetextitem.cpp
* @brief this is the QSlideTextItem implementation file.
* @author Tankery Chen @ Dian Group
* @version 1.0.0
* @date 2011-04-23
*/

#include <QString>
//#include <QGraphicsSceneHoverEvent>
#include "slidetextitem.h"

QSlideTextItem::QSlideTextItem(QGraphicsItem *parent)
        : QGraphicsTextItem(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
}

QSlideTextItem::QSlideTextItem(const QString &text, QGraphicsItem *parent)
        : QGraphicsTextItem(text, parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
}

QVariant QSlideTextItem::itemChange(GraphicsItemChange change,
                     const QVariant &value)
{
    // if selected change and this item is selected, emit selected.
    if (change == QGraphicsItem::ItemSelectedHasChanged)
        emit selectedChange(this, value.toBool());
    return value;
}

void QSlideTextItem::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    emit lostFocus(this);
    QGraphicsTextItem::focusOutEvent(event);
}

/**
* @brief double click make the text item editable.
*/
void QSlideTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() == Qt::NoTextInteraction)
        setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void QSlideTextItem::changeColor(const QColor &color) {
    this->setDefaultTextColor(color);
}

void QSlideTextItem::changeFont(const QFont &font) {
    this->setFont(font);
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

