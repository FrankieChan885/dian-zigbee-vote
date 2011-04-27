/**
 * dianvoteplayer.cpp
 *
 * DianVoteEditor main window implementation file.
 *
 * Author: tankery.chen@gmail.com
 * Modified:
 *      date        author      action
 *  ------------------------------------------------------------
 *      2011-03-15  tankery     created file
 *
 * Copyright (c) Tankery Chen 2011 @ Dian Group
 */
#ifndef DIANVOTEPLAYER_H
#define DIANVOTEPLAYER_H

#include <QtGui/QMainWindow>

class QFile;
class QSlideScene;
class QStandardItemModel;
class QModelIndex;

class DianVoteEditor : public QObject
{
    Q_OBJECT

public:
    DianVoteEditor();
    ~DianVoteEditor();

    // setup the user interface using uiFile for the parent.
    void setupUi(const QString& uiFile, QWidget *parent);

    // setup the main window actions.
    void setupActions();

    // update the slide list by slideScenes.
    void updateSlideList();

    // set application's style using style sheet.
    void setStyle(const QString& qssFile);

    void show();

    // clean the resources.
    void clean();

public slots:
    // save the slide.
    void saveSlide();
    // new a slide scene and append the title to list model.
    void newSlide();
    // open a slide.
    void openSlide();

    // slide list clicked
    void slideListClicked(const QModelIndex &);

private:
    /**
    * @brief manage the file processing.
    * @param isSplit split the file to many slide or merge
    *       merge many slides to a file.
    */
    void fileProcessing(QFile &xmlFile, bool isSplit);

    QMainWindow *dianvoteWindow;

    // slideScenes store all slides.
    QList<QSlideScene *> slideScenes;
    QSlideScene *currentSlideScene;
    // slidesListModel store all slides' outline with list.
    QStandardItemModel *slidesListModel;
};

#endif // DIANVOTEPLAYER_H
