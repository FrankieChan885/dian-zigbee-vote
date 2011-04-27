/**
* @file slideplayerscene.cpp
* @brief the QSlidePlayerScene implementation file.
* @author Tankery Chen @ Dian Group
* @version 1.0.0
* @date 2011-04-27
*/
#include "slideplayerscene.h"

QSlidePlayerScene::QSlidePlayerScene(QWidget * parent/* = 0*/,
                                     QSlideModel *sm/* = 0*/)
    :QSlideScene(parent, sm)
{
}

void QSlidePlayerScene::textItemLostFocus(QGraphicsTextItem*) {
}

void QSlidePlayerScene::textItemSelectedChange(QGraphicsTextItem*, bool) {
}

