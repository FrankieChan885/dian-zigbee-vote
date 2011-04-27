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
class QSlideTextItem;
class QGraphicsPixmapItem;
class QSlideModel;
class QPixmap;

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
    QSlideScene(QWidget * parent = 0, QSlideModel *sm = 0);
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
     *
     * @param toFace the variable to specific the direction of data transfor.
     */
    void updateContent(bool toFace = true);

    /**
    * @brief makeup will beautify the scene. For example, ordering the
    *   topic title and selections, resize the texts, or adding some effects.
    */
    void makeup();

    /**
    * @brief setBackground set the background pixmap in scene area.
    *   this is not the same with QGraphicsScene::setBackgroundBrush, cauz
    *   I using a max lower item to draw the background,
    *   this make the background only showing in scene area.
    *
    * @param pixmap the background pixmap.
    */
    void setBackgroundPixmap(const QPixmap& pixmap);

    /**
    * @brief clearItems delete all items in current scene.
    */
    void clearItems();

    QGraphicsTextItem *addTextItem(const QString &content);

    /**
     * @brief get slide content.
     */
    QString getContent();

    /**
     * @brief get slide title.
     */
    QString getTitle();

public slots:
    virtual void textItemLostFocus(QGraphicsTextItem *item) = 0;
    virtual void textItemSelectedChange(QGraphicsTextItem *item,
                                bool isSelected) = 0;

private:
    /**
     * @brief index2Option translate the selections index to
     *      it's option.
     */
    char index2Option(int index);
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
    QList<QGraphicsTextItem*> selectionStrings;

    // the display been desided by those properties below.
    /**
    * @brief 
    */
    int sceneWidth;
    int sceneHeight;
    /**
    * @brief the background pixmap item.
    */
    QGraphicsPixmapItem *backgroundPixmap;

    /**
    * @brief those value showing the frame of content.
    */
    int titleLeft;
    int titleTop;
    int titleWidth;
    int selectionsLeft;
    int selectionsTop;
    int selectionsWidth;
    int selectionsSpace;
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

