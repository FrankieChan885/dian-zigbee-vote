/**
 * @file slideview.h
 * @brief 
 *  this is the QSlideView class definition file.
 *
 * @author Tankery tankery.chen@gmail.com
 * @version 1.0.0
 * @date 2011-03-18
 */
#ifndef __SLIDEVIEW_H_
#define __SLIDEVIEW_H_

#include <QFrame>
#include <QLabel>
#include <vector>

class QPaintEvent;
class QSlideModel;

/**
 * @brief 
 *  QSlideView is a view that shows slide with topics in content.
 *  You can put this class in any parent view to let the parent desplaying
 *  the content of the topics.
 */
class QSlideView : public QFrame
{
Q_OBJECT

public:
    /**
     * @brief QSlideView initial the view with some attribute.
     *
     * @param parent
     *  the QSlideView's container.
     * @param f
     *  the window style of QSlideWiget.
     */
    QSlideView(QWidget * parent = 0, Qt::WindowFlags f = 0);
    virtual ~QSlideView();

    /**
    * @brief loadNewSlide load a new slide with model specific.
    *   this function will recreating the labels of toptic title and
    *   selections according by the number of selections.
    *
    * @param sm the slide model
    */
    void loadNewSlide(QSlideModel *sm);

    /**
    * @brief deleteLayouts will delete all the layout in current view.
    */
    void deleteLayouts();

    /**
    * @brief setModel set the model of this view.
    *
    * @param sm the slide model that holding the topic content.
    */
    void setModel(QSlideModel *sm) {
        slideModel = sm;
    }
    /**
    * @brief getModel get the slide model
    *
    * @return slide model
    */
    QSlideModel *getModel() {
        return slideModel;
    }

protected:
    /**
    * @brief paintEvent doing all the painting jobs
    *  it will drawing a title and drawing the selections with it's
    *  content and point.
    *
    * @param event
    */
    virtual void paintEvent(QPaintEvent * event);

private:
    /**
    * @brief the slide model that holding the topic content.
    */
    QSlideModel *slideModel;
    /**
    * @brief the topic content string.
    */
    QLabel *topicTitle;
    /**
    * @brief the selection content strings.
    * this strings are in ordered matching the Model's options.
    */
    std::vector <QLabel *> selectionStrings;
    /**
    * @brief editArea specific the space of showing things.
    */
    QFrame *editArea;
};

#endif // __SLIDEVIEW_H_

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
