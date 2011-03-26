/**
 * @file slidemodel.cpp
 * @brief 
 *  this is the QSlideModel class definition file.
 *
 * @author tankery.chen@gmail.com
 * @version 1.0.0.0
 * @date 2011-03-25
 */
#include <QString>
#include <QStringList>
#include <QtXml/QXmlStreamReader>
#include "slidemodel.h"

/**
 * @brief QSlideModel initial the QSlideModel using XML stream.
 *
 */
QSlideModel::QSlideModel(const QString& xmlStream) {
    // @TODO add the QXMLStreamReader init code.
}

QSlideModel::~QSlideModel() {
}

/**
 * @brief setTopic set the topic (title) of slide
 *
 * @param t the topic
 */
void QSlideModel::setTopic(const QString& t) {
    // @TODO set topic to XML file
}

/**
 * @brief setSelections using a string list to set all selections.
 *
 * @param sels the selections list
 */
void QSlideModel::setSelections(const QStringList& sels) {
    // @TODO set selections to XML file
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
    // @TODO read topic frome xml file
    // now for test
    return QString::fromUtf8("中文标题, and English");
}

/**
 * @brief getSelections get all the selections of this slide.
 *
 * @return the selections list
 */
QStringList QSlideModel::getSelections() {
    // @TODO read selections frome xml file to string list.
    // now for test
    static QStringList sels;
    sels << QString::fromUtf8("selection 1") << QString::fromUtf8("选项 2") << QString::fromUtf8("选项 sel 3")
        << QString::fromUtf8("这个是一个很长的选项，我想看看它会显示什么结果呢？如果太长了应该会换行？"
                "不过我觉得它不会换行的。。。。到底是不是这样呢？");

    return sels;
}

/**
 * @brief getSelection get specific selection.
 *
 * @param index the index of this selection
 *
 * @return the selection
 */
QString QSlideModel::getSelection(int index) {
    // @TODO read selection frome xml file to string list.
    return QString::fromUtf8("");
}
/**
 * @brief getSelection get specific selection.
 *
 * @param option the option will be set as 'a, b, c, d, e' (case insensitive)
 *
 * @return the selection
 */
QString QSlideModel::getSelection(char option) {
    // @TODO read selection frome xml file to string list.
    return QString::fromUtf8("");
}


/////////////////////// Static functions ///////////////////////
/**
 * @brief index2Option translate index to option
 */
char QSlideModel::index2Option(int index) {
    // @TODO finish the translation
    return '\0';
}
/**
 * @brief option2Index translate option to index
 */
int QSlideModel::option2Index(char option) {
    // @TODO finish the translation
    return 0;
}

