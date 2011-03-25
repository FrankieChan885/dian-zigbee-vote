/**
 * @file slideview.cpp
 * @brief 
 *  this is the QSlideView class implement file.
 *
 * @author Tankery tankery.chen@gmail.com
 * @version 1.0.0
 * @date 2011-03-18
 */
#include <QWidget>
#include <QPaintEvent>
#include "slideview.h"
#include "slidemodel.h"

/**
 * @brief QSlideView initial the view with some attribute.
 *
 * @param parent
 *  the QSlideView's container.
 * @param f
 *  the window style of QSlideWiget.
 */
QSlideView::QSlideView(QWidget * parent/* = 0*/, Qt::WindowFlags f/* = 0*/)
: QWidget(parent, f)
, slideModel(0)
, topicTitle(0)
{
    selectionStrings.clear();
    editArea = new QWidget(this);
}

QSlideView::~QSlideView() {
}

/**
 * @brief paintEvent doing all the painting jobs
 *  it will drawing a title and drawing the selections with it's content and point.
 *
 * @param event
 */
void QSlideView::paintEvent(QPaintEvent * /*event*/) {
    // do nothing, all the styles are setting in the qss file
}

/**
 * @brief loadNewSlide load a new slide with model specific.
 *   this function will recreating the labels of toptic title and
 *   selections according by the number of selections.
 *
 * @param sm the slide model
 */
void QSlideView::loadNewSlide(QSlideModel *sm) {
    // clear the original layouts.
    deleteLayouts();

    setModel(sm);

    // set new layouts.
    topicTitle = new QLabel(slideModel->getTopic(), editArea);
    // set toptic title's property, type, to "title"
    // this property will deside the style of it set by qss file.
    topicTitle->setProperty("type", QString("title"));
    foreach (QString sel, slideModel->getSelections()) {
        selectionStrings.push_back(new QLabel(sel, editArea));
        // set this label's type to "selection"
        selectionStrings.back()->setProperty("type", QString("selection"));
    }
    
}

/**
 * @brief deleteLayouts will delete all the layout in current view.
 */
void QSlideView::deleteLayouts() {
    if (slideModel) {
        delete slideModel;
    }
    if (topicTitle) {
        delete topicTitle;
    }
    foreach (QLabel *sel, selectionStrings) {
        if (sel) {
            delete sel;
        }
    }
    selectionStrings.clear();
}


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
