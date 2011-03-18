/**
 * @file slidedoc.h
 * @brief 
 *  this is the QSlideDoc class definition file.
 *
 * @author tankery.chen@gmail.com
 * @version 1.0.0.0
 * @date 2011-03-18
 */
#ifndef __SLIDEDOC_H_
#define __SLIDEDOC_H_

#include <QString>
#include <QXMLStreamReader>

/**
 * @brief QSlideDoc holding the single topic content using XML stream, 
 * and provide a interface to access topic XML stream easy.
 */
class QSlideDoc
{
    Q_OBJECT

public:
    /**
     * @brief QSlideDoc initial the QSlideDoc using XML stream.
     *
     */
    QSlideDoc(const QString& xmlStream);
    ~QSlideDoc();

    /**
     * @brief setTopic set the topic (title) of slide
     *
     * @param t the topic
     */
    void setTopic(const QString& t);
    /**
     * @brief setSelections using a string list to set all selections.
     *
     * @param sels the selections list
     */
    void setSelections(const QStringList& sels);
    /**
     * @brief setSelection set specific selection.
     *
     * @param index the index of this selection
     * @param sel the content of this selection
     */
    void setSelection(int index, const QString& sel);
    /**
     * @brief setSelection set specific selection.
     *
     * @param option the option will be set as 'a, b, c, d, e' (case insensitive)
     * @param sel the content of this selection
     */
    void setSelection(char option, const QString& sel);

    /**
     * @brief getTopic get the topic of this slide.
     *
     * @return the topic.
     */
    QString& getTopic();
    /**
     * @brief getSelections get all the selections of this slide.
     *
     * @return the selections list
     */
    QStringList& getSelections();
    /**
     * @brief getSelection get specific selection.
     *
     * @param index the index of this selection
     *
     * @return the selection
     */
    QString& getSelection(int index);
    /**
     * @brief getSelection get specific selection.
     *
     * @param option the option will be set as 'a, b, c, d, e' (case insensitive)
     *
     * @return the selection
     */
    QString& getSelection(char option);

    /**
     * @brief index2Option translate index to option
     */
    char index2Option(int index);
    /**
     * @brief option2Index translate option to index
     */
    int option2Index(char option);

private:
    /**
    * @brief store the content of current topic.
    */
    QXmlStreamReader topicContent;
};

#endif // __SLIDEDOC_H_

