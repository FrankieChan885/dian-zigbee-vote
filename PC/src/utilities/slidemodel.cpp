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
 * @brief setTopic set the topic (title) of slide
 *
 * @param t the topic with xml font string.
 */
void QSlideModel::setTopic(const QString& t) {
    // create a cdata section to hold the html text.
    QString xml = simplifyXml(t);
    QDomCDATASection nodeStr = topicContent.createCDATASection(
            QString::fromAscii(xml.toUtf8().data()));

    // append cdata to topic node.
    QDomElement newTopic = topicContent.createElement(tr("topic"));
    newTopic.appendChild(nodeStr);

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
 * @return the topic.
 */
QString QSlideModel::getTopic() {
    // read topic node from xml file
    QDomElement topicNode = topicContent.documentElement().firstChildElement(tr("topic"));
    // get text
    return QString::fromUtf8(topicNode.text().toLatin1().data());
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
                               const QString& content, const float& point) {
    // if already have option, return.
    if (map4IndexOption.key(option, -1) != -1) {
        qDebug("QSlideModel::addSelection(): already have option: %c", option);
        return false;
    }

    // create a selection.
    QDomElement selection = topicContent.createElement(tr("selection"));

    // set the attribute.
    selection.setAttribute(tr("option"), option);
    selection.setAttribute(tr("point"), point);

    // get the simplified cdata section.
    QString xml = simplifyXml(content);
    QDomCDATASection nodeStr = topicContent.createCDATASection(
            QString::fromAscii(xml.toUtf8().data()));

    // add the content to selection.
    selection.appendChild(nodeStr);
    topicContent.appendChild(selection);

    // add and map for index and option
    map4IndexOption.insert(selectionCount(), option);

    return true;
}

/**
 * @brief removeSelection remove a specific selection.
 */
void QSlideModel::removeSelection(const int index) {
    topicContent.removeChild(getSelectionNode(index));
}

/**
 * @brief this is a overload function
 *      remove a specific selection.
 */
void QSlideModel::removeSelection(const char option) {
    topicContent.removeChild(getSelectionNode(option));
}



/**
 * @brief setSelections using a string list to set all selections.
 *
 * @param sels the selections list
 */
void QSlideModel::setSelections(const QStringList& sels) {
    // set selections frome string list to xml stream.
    int index= 0;
    for (index = 0; index < sels.size(); ++index) {
        setSelection(index, sels.at(index));
    }
}

/**
 * @brief setSelection set specific selection with point.
 *
 * @param index the index of this selection
 * @param sel the content of this selection
 * @param point the selections point.
 */
void QSlideModel::setSelection(int index, const QString& content,
                               const float& point) {
    // first, get selection nodes.
    QDomNode node = getSelectionNode(index);

    setSelectionContent(node, content);
    setSelectionPoint(node, point);
}

/**
 * @brief this is a overload function.
 *      set the content with NO point.
 */
void QSlideModel::setSelection(int index, const QString& content) {
    // first, get selection nodes.
    QDomNode node = getSelectionNode(index);

    // second, operate the selection node.
    setSelectionContent(node, content);
}

/**
 * @brief setSelectionContent set the selection content.
 */
void QSlideModel::setSelectionContent(QDomNode &selNode, const QString& content) {
    if (selNode.isNull()) {
        qDebug("QSlideModel::setSelection(): selection node empty");
        return;
    }

    // first, get the simplified cdata section.
    QString xml = simplifyXml(content);
    QDomCDATASection nodeStr = topicContent.createCDATASection(
            QString::fromAscii(xml.toUtf8().data()));

    // second, remove the first child (cdata).
    selNode.removeChild(selNode.firstChild());
    // last, find first <vote/> and insert new section before it.
    selNode.insertBefore(nodeStr, selNode.firstChildElement(tr("vote")));
}

/**
 * @brief setSelectionPoint set the selection point.
 */
void QSlideModel::setSelectionPoint(QDomNode &selNode, const float& point) {
    if (selNode.isNull()) {
        qDebug("QSlideModel::setSelection(): selection node empty");
        return;
    }

    // new a point attribute.
    QDomAttr newAttr = topicContent.createAttribute(tr("point"));
    newAttr.setValue(QString::number(point));
    // find old attribute.
    QDomAttr oldAttr = selNode.toElement().attributeNode(tr("point"));
    if (oldAttr.isNull()) {
        qDebug("QSlideModel::setSelection(): point attribute empty");
        return;
    }

    // replace them.
    selNode.replaceChild(newAttr, oldAttr);
}

/**
 * @brief setSelection is a overload function.
 *      set specific selection content and point by option.
 */
void QSlideModel::setSelection(char option, const QString& content,
                               const float& point) {
    int index = option2Index(option);
    return setSelection(index, content, point);
}

/**
 * @brief setSelection is a overload function.
 *      set specific selection content with NO point by option.
 */
void QSlideModel::setSelection(char option, const QString& content) {
    int index = option2Index(option);
    return setSelection(index, content);
}

/**
* @brief getSelections get all the selections of this slide.
*
* @param sels [out] the selections list.
*
* @return the selections count.
*/
int QSlideModel::getSelections(QStringList& sels) {
    // read selections frome xml file to string list.
    int index= 0;
    sels.clear();
    QDomNodeList selectionNodes = topicContent.elementsByTagName(
        tr("selection"));
    for (index = 0; index < selectionNodes.size(); ++index) {
        sels.push_back(QString::fromUtf8(selectionNodes.item(
                index).toElement().text().toLatin1().data()));
    }

    return index;
}

/**
* @brief getSelection get specific selection.
*
* @param index the index of this selection.
* @param content selection content.
* @param point selection point.
*/
void QSlideModel::getSelection(int index, QString& content, float& point) {
    // get selection nodes.
    QDomNode node = getSelectionNode(index);
    // get content.
    content = QString::fromUtf8(node.toElement().text().toAscii().data());
    // get point.
    point = node.toElement().attributeNode(tr("point")).value().toFloat();
}
/**
 * @brief getSelection get specific selection.
 *
 * @param option the option will be set as 'a, b, c, d, e' (case insensitive)
 *
 * @return the selection
 */
void QSlideModel::getSelection(char option, QString& content, float& point) {
    // read selection frome xml file to string list.
    int index = option2Index(option);
    return getSelection(index, content, point);
}
/**
* @brief getSelection get specific selection.
*/
void QSlideModel::getSelection(int index, QString& content) {
    // get selection nodes.
    QDomNode node = getSelectionNode(index);
    // get content.
    content = QString::fromUtf8(node.toElement().text().toAscii().data());
}
/**
 * @brief getSelection get specific selection.
 */
void QSlideModel::getSelection(char option, QString& content) {
    // read selection frome xml file to string list.
    int index = option2Index(option);
    return getSelection(index, content);
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

/**
* @brief simplify the xml stream.
*/
QString QSlideModel::simplifyXml(const QString& fullXml) {
    QString xml = fullXml;
    // remove the content before <html>.
    xml.remove(0, xml.indexOf(tr("<html>")));

    QDomDocument doc;
    if (!doc.setContent(xml, false)) {
        qDebug("QSlideModel::simplifyXml(): xml document error..");
        return QString();
    }

    if (doc.isNull()) {
        qDebug("QSlideModel::simplifyXml(): doc is null...");
        return QString();
    }

    // only the body node is usefull.
    QDomElement body = doc.documentElement().firstChildElement(
            tr("body"));
    if (body.isNull()) {
        qDebug("QSlideModel::simplifyXml(): body is null...");
        return QString();
    }
    // using the text stream to get the content.
    QString str;
    QTextStream stream(&str);
    body.save(stream, 4);

    return str;
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

