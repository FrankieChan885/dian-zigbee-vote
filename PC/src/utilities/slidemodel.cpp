/**
 * @file slidemodel.cpp
 * @brief 
 *  this is the QSlideModel class definition file.
 *
 * @author tankery.chen@gmail.com
 * @version 1.0.0.0
 * @date 2011-03-25
 */
#include "slidemodel.h"
#include "exceptions.h"
#include <QString>
#include <QStringList>
#include <QtXml/QDomDocument>

/**
 * @brief QSlideModel initial the QSlideModel using XML stream.
 *
 */
QSlideModel::QSlideModel(const QString& xmlStream)
{
    QString errorStr("");
    int line, column;
    if (!topicContent.setContent(xmlStream, false,
         &errorStr, &line, &column)) {
        throw new XmlStreamException(errorStr.toStdString(), line, column);
    }
}

QSlideModel::~QSlideModel() {
}

/**
 * @brief setTopic set the topic (title) of slide
 *
 * @param t the topic
 */
void QSlideModel::setTopic(const QString& t) {
    // set topic to XML file
    topicContent.documentElement().firstChildElement(
        tr("topic")).setNodeValue(t);
}

/**
 * @brief setSelections using a string list to set all selections.
 *
 * @param sels the selections list
 */
void QSlideModel::setSelections(const QStringList& sels) {
    // @TODO remove selections first
    // @TODO add selections to XML file
}

/**
 * @brief setSelection set specific selection.
 *
 * @param index the index of this selection
 * @param sel the content of this selection
 */
void QSlideModel::setSelection(int index, const QString& sel) {
}

/**
 * @brief setSelection set specific selection.
 *
 * @param option the option will be set as 'a, b, c, d, e' (case insensitive)
 * @param sel the content of this selection
 */
void QSlideModel::setSelection(char option, const QString& sel) {
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
    QDomNodeList selectionNodes = topicContent.elementsByTagName(
        tr("selection"));
    QDomNode node = selectionNodes.item(index);
    // get content.
    content = node.nodeValue();
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
 * @brief index2Option translate index to option
 */
char QSlideModel::index2Option(int index) {
    // translate the index to option
    QDomNodeList selectionNodes = topicContent.elementsByTagName(
        tr("selection"));
    QDomElement element = selectionNodes.item(index).toElement();
    QString optionStr = element.attributeNode(tr("option")).value();

    return (*optionStr.data()).toAscii();
}
/**
 * @brief option2Index translate option to index
 */
int QSlideModel::option2Index(char option) {
    // @TODO finish the translation
    return 0;
}

/**
* @brief randomOptions random the options in selections.
*/
void QSlideModel::randomOptions() {
}

