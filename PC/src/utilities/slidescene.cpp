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
#include <QGraphicsItemGroup>

/**
 * @brief QSlideScene initial the view with parent
 */
QSlideScene::QSlideScene(QObject * parent/* = 0*/, QSlideModel *sm/* = 0*/)
: QGraphicsScene(parent)
, slideModel(sm)
, topicTitle(0)
, selectionStrings(0)
{
    // set size to 800x600
    setSceneRect(0, 0, 800, 600);

    // if specific the model, create items using this model.
    if (slideModel) {
        // set topic title.
        topicTitle = addText(slideModel->getTopic());

        // set selection strings.
        selectionStrings = createItemGroup(QList<QGraphicsItem*>());
        QStringList sels;
        slideModel->getSelections(sels);
        foreach (QString sel, sels) {
            selectionStrings->addToGroup(addText(sel));
        }

        // beauty this.
        makeup();
    }
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
    updateContent();
    makeup();
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
        if (topicTitle) {
            topicTitle->setHtml(slideModel->getTopic());
        }
        else {
            topicTitle = addText(slideModel->getTopic());
        }

        // set selection strings.
        QStringList sels;
        slideModel->getSelections(sels);

        // move iterator to first item.
        QList<QGraphicsItem *>::iterator i =
            selectionStrings->childItems().begin();
        // loop in selection strings add modifiled item.
        foreach (QString sel, sels) {
            // if has existing item, reset the text.
            if (i != selectionStrings->childItems().end()) {
                ((QGraphicsTextItem *)*i)->setHtml(sel);
            }
            // if no existing item, add one.
            else {
                selectionStrings->addToGroup(addText(sel));
            }
            // move to next item.
            ++i;
        } // foreach
        // if there are more existing item, remove it.
        while (i != selectionStrings->childItems().end()) {
            selectionStrings->removeFromGroup(*i);
        }
    }
}

/**
 * @brief makeup will beautify the scene. For example, ordering the
 *   topic title and selections, resize the texts, or adding some effects.
 */
void QSlideScene::makeup() {
    // @TODO place the topic title and selections at well place according
    // the content.
}

/**
 * @brief clearItems will delete all the items in current scene.
 */
void QSlideScene::clearItems() {
    if (topicTitle) {
        delete topicTitle;
        topicTitle = 0;
    }

    destroyItemGroup(selectionStrings);
}

