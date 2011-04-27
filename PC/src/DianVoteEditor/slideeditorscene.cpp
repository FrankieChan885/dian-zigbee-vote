/**
* @file slideeditorscene.cpp
* @brief the QSlideEditorScene class implementation file.
*
* @author Tankery Chen @ Dian Group
* @email tankery.chen@gmail.com
* @version 1.0.0
* @date 2011-04-27
*/

#include <QGraphicsTextItem>
#include <QMainWindow>
#include <QTextCursor>
#include <QToolBar>

#include "slideeditorscene.h"
#include "slidetextitem.h"
#include "slidefontbar.h"
#include "exceptions.h"

/**
 * @brief QSlideEditorScene initial the view with parent
 */
QSlideEditorScene::QSlideEditorScene(QWidget * parent/* = 0*/, QSlideModel *sm/* = 0*/)
: QSlideScene(parent, sm)
, fontBar(0)
{
}

void QSlideEditorScene::textItemSelectedChange(QGraphicsTextItem *item,
                                         bool isSelected) {
    QMainWindow * mainwindow = qobject_cast<QMainWindow *>(parent());
    QSlideTextItem * textItem = qgraphicsitem_cast<QSlideTextItem *>(item);
    if (isSelected) {
        // if has text item selected, create an new fon editor.
        if (qgraphicsitem_cast<QSlideTextItem *>(item)) {
            // create and font editor around new item.
            if (fontBar == 0) {
                QSlideFontBar *bar = new QSlideFontBar(
                        qobject_cast<QWidget *>(parent()));
                bar->setFont(item->font());
                // connect the font and color change to text item.
                QObject::connect(bar, SIGNAL(colorChanged(QColor)),
                                 textItem, SLOT(changeColor(QColor)));
                QObject::connect(bar, SIGNAL(fontChanged(QFont)),
                                 textItem, SLOT(changeFont(QFont)));
                fontBar = bar->addToolBar();
            }

            mainwindow->addToolBar(fontBar);
        }
    } // if (isSelected)
    else {
        // destroy the old font editor first.
        if (fontBar) {
            mainwindow->removeToolBar(fontBar);
            delete fontBar;
            fontBar = 0;
        }

        // do makeup.
        updateContent(false);
        updateContent();
        makeup();
    } // if (isSelected) else
}

void QSlideEditorScene::textItemLostFocus(QGraphicsTextItem *item)
{
    // clear the cursor selection.
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);

    //    if (item->toPlainText().isEmpty()) {
    //        item->setHtml(defaultText);
    //    }

    // do makeup.
    updateContent(false);
    updateContent();
    makeup();
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

