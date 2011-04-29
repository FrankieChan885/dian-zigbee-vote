/**
 * @file slidemodel.h
 * @brief 
 *  this is the QSlideModel class definition file.
 *
 * @author tankery.chen@gmail.com
 * @version 1.0.0.0
 * @date 2011-03-18
 */
#ifndef __SLIDEMODEL_H_
#define __SLIDEMODEL_H_

#include <QObject>
#include <QMap>
#include <QtXml/QDomDocument>

class QFont;
class QColor;
class QString;

/**
 * @brief QSlideModel holding the single topic content using XML stream, 
 * and provide a interface to access topic XML stream easy.
 */
class QSlideModel : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief QSlideModel initial the QSlideModel using XML stream.
     *
     */
    QSlideModel(const QString& xmlStream, QObject *parent = 0);
    ~QSlideModel();

    /**
     * @brief set the model's content.
     *
     * @param xmls the topic content.
     */
    void setContent(const QString& xmls);
    /**
     * @brief get the model's content.
     *
     * @return the topic content.
     */
    QString getContent();
    /**
     * @brief get the model's root node.
     */
    QDomNode *getNode();

    /**
     * @brief setTopic set the topic (title) of slide
     *
     * @param t the topic
     */
    void setTopic(const QString& t, const QFont& f, const QColor& c);
    /**
     * @brief getPlainTopic get the topic of this slide.
     *
     * @return the topic.
     */
    QString getPlainTopic();
    /**
     * @brief getTopic get the topic of this slide.
     *
     * @param t the topic content.
     */
    void getTopic(QString& t, QFont& f, QColor& c);

    /**
     * @brief addSelection add a selection with content and point.
     *
     * @param content selection content.
     * @param point selection point.
     */
    bool addSelection(const char option,
                      const QString& content, const QFont& f, const QColor& c, const float& point);
    /**
     * @brief removeSelection remove a specific selection.
     */
    void removeSelection(const int index);
    /**
     * @brief this is a overload function
     *      remove a specific selection.
     */
    void removeSelection(const char option);

    /**
     * @brief get the slection node by index.
     */
    QDomNode getSelectionNode(int index);
    /**
     * @brief get the slection node by optin.
     */
    QDomNode getSelectionNode(char option);
    /**
     * @brief setSelection set specific selection.
     *
     * @param index the index of this selection.
     * @param content selection content.
     * @param point selection point.
     */
    void setSelection(int index, const QString& content, const QFont& f, const QColor& c,
                      const float& point);
    /**
     * @brief this is a overload function.
     */
    void setSelection(int index, const QString& content, const QFont& f, const QColor& c);
    /**
     * @brief setSelectionContent set the selection content.
     */
    void setSelectionContent(QDomNode& selNode, const QString& content, const QFont& f, const QColor& c);
    /**
     * @brief setSelectionPoint set the selection point.
     */
    void setSelectionPoint(QDomNode& selNode, const float& point);
    /**
     * @brief setSelection set specific selection.
     *
     * @param option the option will be set as 'a, b, c, d, e' (case insensitive)
     * @param content selection content.
     * @param point selection point.
     */
    void setSelection(char option, const QString& content, const QFont& f, const QColor& c,
                      const float& point);
    /**
     * @brief this is a overload function.
     */
    void setSelection(char option, const QString& content, const QFont& f, const QColor& c);

    /**
     * @brief getSelection get specific selection.
     *
     * @param index the index of this selection.
     * @param content selection content.
     * @param point selection point.
     */
    void getSelection(int index, QString& content, QFont& f, QColor& c, float& point);
    /**
     * @brief getSelection get specific selection.
     *
     * @param option the option will be set as 'a, b, c, d, e' (case insensitive)
     *
     * @return the selection
     */
    void getSelection(char option, QString& content, QFont& f, QColor& c, float& point);
    /**
     * @brief getSelection get specific selection with NO point.
     */
    void getSelection(int index, QString& content, QFont& f, QColor& c);
    /**
     * @brief getSelection get specific selection with NO point.
     */
    void getSelection(char option, QString& content, QFont& f, QColor& c);

    /**
     * @brief index2Option translate index to option
     */
    char index2Option(int index);
    /**
     * @brief option2Index translate option to index
     */
    int option2Index(char option);
    /**
     * @brief randomOptions random the options in selections.
     */
    void randomOptions();
    /**
     * @brief selectionCount return the selections number.
     */
    int selectionCount();

/// static public functions
public:
    static QSlideModel *createModel(QObject *parent = 0);

private:
    /**
    * @brief store the content of current topic.
    */
    QDomDocument topicContent;
    /**
     * @brief the map of options and index.
     */
    QMap <int, char> map4IndexOption;
};

#endif // __SLIDEMODEL_H_

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

