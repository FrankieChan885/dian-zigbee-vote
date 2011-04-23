/**
* @file slidescene.cpp
* @brief the QSlideScene class implementation file.
*
* @author Tankery Chen @ Dian Group
* @email tankery.chen@gmail.com
* @version 1.0.0
* @date 2011-04-01
*/

#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QTextCursor>
#include <QList>

#include <limits>

#include "slidescene.h"
#include "slidemodel.h"
#include "slidetextitem.h"
#include "exceptions.h"

/**
 * @brief QSlideScene initial the view with parent
 */
QSlideScene::QSlideScene(QObject * parent/* = 0*/, QSlideModel *sm/* = 0*/)
: QGraphicsScene(parent)
, slideModel(0)
, topicTitle(0)
, sceneWidth(800)   // set size to 800x600
, sceneHeight(600)
, backgroundPixmap(0)
, titleLeft(100)
, titleTop(50)
, titleWidth(600)
, selectionsLeft(150)
, selectionsTop(150)
, selectionsWidth(500)
, selectionsSpace(20)
{
    setSceneRect(0, 0, sceneWidth, sceneHeight);

    // set default background.
    setBackgroundBrush(Qt::black);

    // set model to update.
    setModel(sm);
}

QSlideScene::~QSlideScene() {
    // clear items.
    clearItems();

    // clear model.
    if (slideModel) {
        delete slideModel;
        slideModel = 0;
    }
}

/**
 * @brief setModel set the slide model of this class and update the
 *   view.
 *
 * @param sm the slide model
 */
void QSlideScene::setModel(QSlideModel *sm) {
    slideModel = sm;
    if (slideModel) {
        updateContent();
        makeup();
    }
}

/**
 * @brief model get the slide model of this class.
 *
 * @return the slide model.
 */
QSlideModel *QSlideScene::model() {
    return slideModel;
}

/**
 * @brief updateContent will update the content of this with model.
 *   this function will creating new selections if need.
 *
 * @param toFace the variable to specific the direction of data transfor.
 */
void QSlideScene::updateContent(bool toFace) {
    // update until this class has model.
    if (slideModel) {
        if (toFace) {
            // set topic title.
            if (topicTitle == 0) {
                topicTitle = addTextItem("");
            }
            QString str = slideModel->getTopic();
            topicTitle->setHtml(str);

            // set selection strings.
            // attention: the order of the selections can be random,
            //      so, we only using the option to specific the selection.

            int i;      // used in 'for', 'while' also use this.
            // first iterate in slide model selections
            for (i = 0; i < slideModel->selectionCount(); ++i) {
                QString sel;
                slideModel->getSelection(index2Option(i), sel);
                // if no existing item, add one.
                if (i == selectionStrings.size()) {
                    selectionStrings.push_back(addTextItem(""));
                }
                selectionStrings[i]->setHtml(sel);
            }
            // if there are more existing item, remove it.
            while (i < selectionStrings.size()) {
                if (selectionStrings[i]) {
                    delete selectionStrings[i];
                }
                selectionStrings.removeAt(i);
                ++i;
            }
        } // if (toFace)
        else {
            if (topicTitle == 0)
                return;

            slideModel->setTopic(topicTitle->toHtml());

            // set xml selection strings.
            // attention: the order of the selections can be random,
            //      so, we only using the option to specific the selection.

            int i;      // used in 'for', 'while' also use this.
            for (i = 0; i < selectionStrings.size(); ++i) {
                // if no enough xml selection, add one.
                if (i == selectionStrings.size()) {
                    slideModel->addSelection(index2Option(i),
                        selectionStrings.at(i)->toHtml(), 0.0f);
                }
                else {
                    slideModel->setSelection(index2Option(i),
                        selectionStrings.at(i)->toHtml());
                }
            }
            // if there are more existing xml selection, remove it.
            while (i < slideModel->selectionCount()) {
                slideModel->removeSelection(index2Option(i));
                ++i;
            }
        } // if (toFace) else
    } // if (slideModel)
} // void QSlideScene::updateContent(bool toFace)

/**
 * @brief makeup will beautify the scene. For example, ordering the
 *   topic title and selections, resize the texts, or adding some effects.
 */
void QSlideScene::makeup() {
    // place the topic title and selections at well place according
    // to the content.
    if (slideModel) {
        // set topic title.
        if (topicTitle) {
            topicTitle->setTextWidth(titleWidth);
            topicTitle->setPos(titleLeft, titleTop);
        }

        // set selection strings.
        int i;      // done put in for, while also use this.
        int selTop = selectionsTop;  // height of selection items.
        // I'm using the iterator first, but the position of iterator
        // make me crazy, so I using int value instead.
        for (i = 0; i < selectionStrings.size(); ++i) {
            // set width and content with font.
            selectionStrings[i]->setTextWidth(selectionsWidth);
            selectionStrings[i]->setPos(selectionsLeft, selTop);

            // move selTop to next selection position.
            selTop += selectionStrings[i]->sceneBoundingRect().height() +
                selectionsSpace;
        }
    }
}

/**
 * @brief setBackground set the background pixmap in scene area.
 *   this is not the same with QGraphicsScene::setBackgroundBrush, cauz
 *   I using a max lower item to draw the background,
 *   this make the background only showing in scene area.
 *
 * @param pixmap the background pixmap.
 */
void QSlideScene::setBackgroundPixmap(const QPixmap& pixmap) {
    // if no pixmap, return.
    if (pixmap.isNull()) {
        return;
    }
    // set background pixmap.
    if (backgroundPixmap == 0) {
        backgroundPixmap = addPixmap(pixmap);
    }
    else {
        backgroundPixmap->setPixmap(pixmap);
    }

    // set the background pixmap z value to the bottom of every items.
    backgroundPixmap->setZValue(std::numeric_limits<qreal>::max()*-1);

    backgroundPixmap->setPos(0, 0);

    // scale
    qreal newScale = this->height() / pixmap.height();
    backgroundPixmap->setScale(newScale);
}

/**
 * @brief clearItems will delete all the items in current scene.
 */
void QSlideScene::clearItems() {
    // delete topic title.
    if (topicTitle) {
        delete topicTitle;
        topicTitle = 0;
    }

    // delete selections.
    for (QList<QGraphicsTextItem*>::iterator i= selectionStrings.begin();
        i != selectionStrings.end(); ++i) {
        if (*i) {
            delete *i;
        }
    }
    selectionStrings.clear();

    // delete background pixmap item.
    if (backgroundPixmap) {
        delete backgroundPixmap;
        backgroundPixmap = 0;
    }
}

QGraphicsTextItem *QSlideScene::addTextItem(const QString &content)
{
    QGraphicsTextItem *item = new QSlideTextItem(content);
    item->setTextInteractionFlags(Qt::TextEditorInteraction);
    QObject::connect(item, SIGNAL(lostFocus(QGraphicsTextItem*)),
                     this, SLOT(textItemLostFocus(QGraphicsTextItem*)));
    addItem(item);
    return item;
}

/**
 * @brief get slide content.
 */
QString QSlideScene::getContent() {
    return slideModel->getContent();
}

void QSlideScene::textItemLostFocus(QGraphicsTextItem *item)
{
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);

    if (item->toPlainText().isEmpty()) {
        item->setHtml(defaultText);
    }

    // do makeup.
    updateContent(false);
    updateContent();
    makeup();
}

/**
 * @brief index2Option translate the selections index to
 *      it's option.
 */
char QSlideScene::index2Option(int index) {
    return ('A' + index);
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

