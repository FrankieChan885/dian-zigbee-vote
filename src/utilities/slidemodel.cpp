/**
 * @file slidemodel.cpp
 * @brief 
 *  this is the QSlideModel class definition file.
 *
 * @author tankery.chen@gmail.com
 * @version 1.0.0.0
 * @date 2011-03-25
 */
#include <QFont>
#include <QColor>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QtXml/QDomDocument>
#include "slidemodel.h"
#include "exceptions.h"

/**
 * @brief QSlideModel initial the QSlideModel using XML stream.
 *
 */
QSlideModel::QSlideModel(const QString& xmlStream, QObject *parent)
        : QObject(parent)
{
    setContent(xmlStream);

    // initialize the map for index and options;

    // first, clear the map.
    map4IndexOption.clear();
    // second, get the selections.
    QDomNodeList selectionNodes = topicContent.elementsByTagName(
        tr("selection"));
    // third, get the options by index.
    for (int i = 0; i < selectionNodes.size(); ++i) {
        QDomElement element = selectionNodes.item(i).toElement();
        QString optionStr = element.attributeNode(tr("option")).value();
        char option = optionStr.at(0).toUpper().toAscii();

        map4IndexOption.insert(i, option);
    }

}

QSlideModel::~QSlideModel() {
    qDebug("slide model deleted");
}

/**
* @brief this function create the slide model with a default value.
*/
QSlideModel *QSlideModel::createModel(QObject *parent/* = 0*/) {
    // create an empty dom document with root node <slide/>
    QDomDocument newSlide;
    QDomNode rootNode = newSlide.createElement(tr("slide"));
    newSlide.appendChild(rootNode);
    QDomNode topicNode = newSlide.createElement(tr("topic"));
    rootNode.appendChild(topicNode);

    return (new QSlideModel(newSlide.toString(), parent));
}

/**
 * @brief set the model's content.
 *
 * @param xmls the topic content.
 */
void QSlideModel::setContent(const QString &xmls)
{
    QString errorStr("");
    int line, column;
    if (!topicContent.setContent(xmls, false,
                                 &errorStr, &line, &column)) {
        throw new XmlStreamException(errorStr.toStdString(), line, column);
    }
}

/**
 * @brief get the model's content.
 *
 * @return the topic content.
 */
QString QSlideModel::getContent()
{
    return topicContent.toString();
}

/**
 * @brief get the model's root node.
 */
QDomNode *QSlideModel::getNode() {
    return &topicContent;
}

/**
 * @brief setTopic set the topic (title) of slide
 *
 * @param t the topic with xml font string.
 */
void QSlideModel::setTopic(const QString& t, const QFont& f,
                           const QColor& c) {
    // create an topic text node
    QDomText topicText = topicContent.createTextNode(t);
    // create an new topic element node.
    QDomElement newTopic = topicContent.createElement(tr("topic"));
    newTopic.setAttribute(tr("font"), f.toString());
    newTopic.setAttribute(tr("color"), c.name());
    newTopic.appendChild(topicText);

    // replace old node.
    QDomElement oldTopic =
            topicContent.documentElement().firstChildElement(tr("topic"));
    if (oldTopic.isNull()) {
        qDebug("QSlideModel::setTopic(): topic node empty...");
        return;
    }
    topicContent.documentElement().replaceChild(newTopic, oldTopic);
}
/**
 * @brief getTopic get the topic of this slide.
 *
 * @param t the topic content.
 */
void QSlideModel::getTopic(QString& t, QFont& f, QColor& c) {
    // read topic node from xml file
    QDomElement topicNode = topicContent.documentElement().firstChildElement(tr("topic"));
    // get text
    t = topicNode.text();

    // get font
    f.fromString(topicNode.attribute(tr("font")));

    // get color
    c.setNamedColor(topicNode.attribute(tr("color")));
}
/**
 * @brief getPlainTopic get the topic of this slide.
 *
 * @return the topic.
 */
QString QSlideModel::getPlainTopic() {
    // read topic node from xml file
    QDomElement topicNode = topicContent.documentElement().firstChildElement(tr("topic"));
    // get text
    return topicNode.text();
}




/**
 * @brief get the slection node by index.
 */
QDomNode QSlideModel::getSelectionNode(int index) {
    QDomNodeList selNodeList = topicContent.elementsByTagName(
        tr("selection"));
    if (index >= selNodeList.size()) {
        qDebug("QSlideModel::setSelection(): index too big...");
        return QDomNode();
    }
    QDomNode node = selNodeList.item(index);
    if (node.isNull()) {
        qDebug("QSlideModel::setSelection(): selection node empty");
        return QDomNode();
    }

    return node;
}
/**
 * @brief get the slection node by optin.
 */
QDomNode QSlideModel::getSelectionNode(char option) {
    return getSelectionNode(option2Index(option));
}

/**
 * @brief addSelection add a selection with content and point.
 *
 * @param content selection content.
 * @param point selection point.
 */
bool QSlideModel::addSelection(const char option,
                               const QString& content, const QFont& f,
                               const QColor& c, const float& point) {
    // if already have option, return.
    if (map4IndexOption.key(option, -1) != -1) {
        qDebug("QSlideModel::addSelection(): already have option: %c", option);
        return false;
    }

    // create a selection.
    QDomElement selection = topicContent.createElement(tr("selection"));

    topicContent.appendChild(selection);

    // add and map for index and option
    map4IndexOption.insert(selectionCount(), option);

    setSelection(option, content, f, c, point);

    return true;
}

/**
 * @brief removeSelection remove a specific selection.
 */
void QSlideModel::removeSelection(const int index) {
    topicContent.removeChild(getSelectionNode(index));
    map4IndexOption.remove(index);
}

/**
 * @brief this is a overload function
 *      remove a specific selection.
 */
void QSlideModel::removeSelection(const char option) {
    removeSelection(option2Index(option));
}


/**
 * @brief setSelection set specific selection with point.
 *
 * @param index the index of this selection
 * @param sel the content of this selection
 * @param point the selections point.
 */
void QSlideModel::setSelection(int index, const QString& content
                               , const QFont& f, const QColor& c,
                               const float& point) {
    // first, get selection nodes.
    QDomElement node = getSelectionNode(index).toElement();

    // create an text node
    QDomText text = topicContent.createTextNode(content);
    // replace the first child node (text node).
    node.replaceChild(text, node.firstChild());
    // set the attribute.
    node.setAttribute(tr("font"), f.toString());
    node.setAttribute(tr("color"), c.name());
    node.setAttribute(tr("point"), point);
}

/**
 * @brief this is a overload function.
 *      set the content with NO point.
 */
void QSlideModel::setSelection(int index, const QString& content
                               , const QFont& f, const QColor& c) {
    // first, get selection nodes.
    QDomElement node = getSelectionNode(index).toElement();

    // create an text node
    QDomText text = topicContent.createTextNode(content);
    // replace the first child node (text node).
    node.replaceChild(text, node.firstChild());
    // set the attribute.
    node.setAttribute(tr("font"), f.toString());
    node.setAttribute(tr("color"), c.name());
}

/**
 * @brief setSelection is a overload function.
 *      set specific selection content and point by option.
 */
void QSlideModel::setSelection(char option, const QString& content
                               , const QFont& f, const QColor& c,
                               const float& point) {
    int index = option2Index(option);
    return setSelection(index, content, f, c, point);
}

/**
 * @brief setSelection is a overload function.
 *      set specific selection content with NO point by option.
 */
void QSlideModel::setSelection(char option, const QString& content
                               , const QFont& f, const QColor& c) {
    int index = option2Index(option);
    return setSelection(index, content, f, c);
}

/**
* @brief getSelection get specific selection.
*
* @param index the index of this selection.
* @param content selection content.
* @param point selection point.
*/
void QSlideModel::getSelection(int index, QString& content
                               , QFont& f, QColor& c
                               , float& point) {
    // get selection nodes.
    QDomElement node = getSelectionNode(index).toElement();
    // get content.
    content = node.text();
    // get font
    f.fromString(node.attribute(tr("font")));
    // get color
    c.setNamedColor(node.attribute(tr("color")));

    // get point.
    point = node.attributeNode(tr("point")).value().toFloat();
}
/**
 * @brief getSelection get specific selection.
 *
 * @param option the option will be set as 'a, b, c, d, e' (case insensitive)
 *
 * @return the selection
 */
void QSlideModel::getSelection(char option, QString& content
                               , QFont& f, QColor& c, float& point) {
    // read selection frome xml file to string list.
    int index = option2Index(option);
    return getSelection(index, content, f, c, point);
}
/**
* @brief getSelection get specific selection.
*/
void QSlideModel::getSelection(int index, QString& content
                               , QFont& f, QColor& c) {
    // get selection nodes.
    QDomElement node = getSelectionNode(index).toElement();
    // get content.
    content = node.toElement().text();
    // get font
    f.fromString(node.attribute(tr("font")));
    // get color
    c.setNamedColor(node.attribute(tr("color")));
}
/**
 * @brief getSelection get specific selection.
 */
void QSlideModel::getSelection(char option, QString& content
                               , QFont& f, QColor& c) {
    // read selection frome xml file to string list.
    int index = option2Index(option);
    return getSelection(index, content, f, c);
}

/**
 * @brief index2Option translate index to option
 */
char QSlideModel::index2Option(int index) {
    // if no index found, return '\0'.
    return map4IndexOption.value(index, '\0');
}
/**
 * @brief option2Index translate option to index
 */
int QSlideModel::option2Index(char option) {
    // if no option found, return -1.
    return map4IndexOption.key(option, -1);
}

/**
* @brief randomOptions random the options in selections.
*/
void QSlideModel::randomOptions() {
}

/**
 * @brief selectionCount return the selections number.
 */
int QSlideModel::selectionCount() {
    return map4IndexOption.size();
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

