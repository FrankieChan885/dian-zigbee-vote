/**
* @file slidescene.h
* @brief the slidescene shows the slide's content with mutiple
*   effects.
*
* @author Tankery Chen @ Dian Group
* @email tankery.chen@gmail.com
* @version 1.0.0
* @date 2011-03-31
*/
#ifndef __SLIDESCENE_H_
#define __SLIDESCENE_H_

#include <QGraphicsScene>

class QGraphicsTextItem;
class QGraphicsItemGroup;
class QSlideModel;

/**
 * @brief 
 *  QSlideScene is a view that shows slide with topics in content.\n
 *  You can put this class in any parent graphics view to let the parent
 *  desplaying the content of the topics.
 */
class QSlideScene : public QGraphicsScene
{
    Q_OBJECT

public:
    /**
    * @brief QSlideScene initial the view with parent
    */
    QSlideScene(QObject * parent = 0, QSlideModel *sm = 0);
    virtual ~QSlideScene();

    /**
     * @brief setModel set the slide model of this class and update the
     *   view.
     *
     * @param sm the slide model
     */
    void setModel(QSlideModel *sm);
    /**
    * @brief model get the slide model of this class.
    *
    * @return the slide model.
    */
    QSlideModel *model();

    /**
     * @brief updateContent will update the content of this with model.
     *   this function will creating new selections if need.
     */
    void updateContent();

    /**
    * @brief makeup will beautify the scene. For example, ordering the
    *   topic title and selections, resize the texts, or adding some effects.
    */
    void makeup();

    /**
    * @brief clearItems delete all items in current scene.
    */
    void clearItems();

private:
    /**
    * @brief the slide model that holding the topic content.
    */
    QSlideModel *slideModel;
    /**
    * @brief the topic content string.
    */
    QGraphicsTextItem *topicTitle;
    /**
    * @brief the selection content strings.
    * this strings are in ordered matching the Model's options.
    */
    QGraphicsItemGroup *selectionStrings;
};

#endif // __SLIDESCENE_H_

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

