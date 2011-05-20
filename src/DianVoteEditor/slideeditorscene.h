/**
* @file slideeditorscene.h
* @brief the QSlideEditorScene shows the slide's content and provid an
*       ability to edit the scene.
*
* @author Tankery Chen @ Dian Group
* @email tankery.chen@gmail.com
* @version 1.0.0
* @date 2011-04-27
*/
#ifndef __SLIDEEDITORSCENE_H_
#define __SLIDEEDITORSCENE_H_

#include "../utilities/slidescene.h"

class QGraphicsTextItem;
class QToolBar;

/**
 * @brief 
 *  QSlideEditorScene is a view that shows slide with topics in content.\n
 *  You can put this class in any parent graphics view to let the parent
 *  desplaying the content of the topics.
 *  What's more, the Editor scene provid an ability to edit the scene.
 */
class QSlideEditorScene : public QSlideScene
{
    Q_OBJECT

public:
    /**
    * @brief QSlideEditorScene initial the view with parent
    */
    QSlideEditorScene(QSlideModel *sm = 0, QWidget * parent = 0);

    ~QSlideEditorScene();

    QGraphicsTextItem *addTextItem(const QString &content);

public slots:
    void textItemLostFocus(QGraphicsTextItem *item);
    /**
    * @brief if the item is selected, isSelected will be true,
    *   else if the item is lost select,
    *   isSelected will be false.
    */
    void textItemSelectedChange(QGraphicsTextItem *item,
                                bool isSelected);

private:
    QToolBar *fontBar;
};

#endif // __SLIDEEDITORSCENE_H_

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

