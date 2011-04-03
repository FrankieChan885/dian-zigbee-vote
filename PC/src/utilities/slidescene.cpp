/**
* @file slidescene.cpp
* @brief the QSlideScene class implementation file.
*
* @author Tankery Chen @ Dian Group
* @email tankery.chen@gmail.com
* @version 1.0.0
* @date 2011-04-01
*/

#include "slidescene.h"
#include "slidemodel.h"
#include "exceptions.h"

#include <QGraphicsTextItem>
#include <QList>
#include <QBrush>

/**
 * @brief QSlideScene initial the view with parent
 */
QSlideScene::QSlideScene(QObject * parent/* = 0*/, QSlideModel *sm/* = 0*/)
: QGraphicsScene(parent)
, slideModel(0)
, topicTitle(0)
, sceneWidth(800)
, sceneHeight(600)
, titleLeft(100)
, titleTop(50)
, titleWidth(400)
, selectionsLeft(150)
, selectionsTop(150)
, selectionsWidth(500)
, selectionsSpace(20)
, background(Qt::white)
{
    // set size to 800x600
    setSceneRect(0, 0, 800, 600);

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
 */
void QSlideScene::updateContent() {
    // update until this class has model.
    if (slideModel) {
        // set topic title.
        if (topicTitle == 0) {
            topicTitle = addText("");
        }
        topicTitle->setHtml(slideModel->getTopic());

        // set selection strings.
        int i;      // done put in for, while also use this.
        // I'm using the iterator first, but the position of iterator
        // make me crazy, so I using int value instead.
        QStringList sels;
        slideModel->getSelections(sels);
        for (i = 0; i < sels.size(); ++i) {
            QString sel;
            sel = sels[i];
            // if no existing item, add one.
            if (i == selectionStrings.size()) {
                selectionStrings.push_back(addText(""));
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
    }
}

/**
 * @brief makeup will beautify the scene. For example, ordering the
 *   topic title and selections, resize the texts, or adding some effects.
 */
void QSlideScene::makeup() {
    // set background
    setBackgroundBrush(background);

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
}

