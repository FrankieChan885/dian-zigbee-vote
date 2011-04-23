/**
* @file slideeditor.cpp
* @brief this is the QSlideEditor implementation file.
* @author Tankery Chen @ Dian Group
*   mailto:tankery.chen@gmail.com
* @version 1.0.0
* @date 2011-04-23
*/
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QString>
#include <QFile>
#include <QSize>

#include "slideeditor.h"
#include "../utilities/slidemodel.h"
#include "../utilities/slidescene.h"

QSlideEditor::QSlideEditor(QString &modelStr, QWidget *parent)
        : QWidget(parent)
        , slideScene(0)
{
    QSlideModel *model = new QSlideModel(modelStr, this);

    // set the slide scene with gray surround the white.
    slideScene = new QSlideScene(this, model);
    QPixmap pixmap(QSize(800, 600));
    pixmap.fill(Qt::white);
    slideScene->setBackgroundPixmap(pixmap);
    slideScene->setBackgroundBrush(Qt::darkGray);

    //set slide view
    QGraphicsView *view = new QGraphicsView(slideScene, this);
    view->setFrameShape(QFrame::NoFrame);
    QHBoxLayout *viewLayout = new QHBoxLayout;
    viewLayout->addWidget(view);
    setLayout(viewLayout);
}

/**
 * @brief save the slide content to file.
 */
void QSlideEditor::save(const QString &filePath) {
    QFile file(filePath);
    file.open(QIODevice::WriteOnly);
    if (file.isOpen()) {
        file.write(slideScene->getContent().toAscii());
    }
    else {
        qDebug("QSlideScene::save(): file open failed...");
    }
}

void QSlideEditor::setBackgroundPixmap(const QString &imgPath) {
    QPixmap pixmap(imgPath);
    slideScene->setBackgroundPixmap(pixmap);
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

