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
#include <QtXml/QDomDocument>

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
    QSlideModel(const QString& xmlStream);
    ~QSlideModel();

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
    QString getTopic();
    /**
     * @brief getSelections get all the selections of this slide.
     *
     * @param sels [out] the selections list.
     *
     * @return the selections count.
     */
    int getSelections(QStringList& sels);
    /**
     * @brief getSelection get specific selection.
     *
     * @param index the index of this selection.
     * @param content selection content.
     * @param point selection point.
     */
    void getSelection(int index, QString& content, float& point);
    /**
     * @brief getSelection get specific selection.
     *
     * @param option the option will be set as 'a, b, c, d, e' (case insensitive)
     *
     * @return the selection
     */
    void getSelection(char option, QString& content, float& point);

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

private:
    /**
    * @brief store the content of current topic.
    */
    QDomDocument topicContent;
};

#endif // __SLIDEMODEL_H_

