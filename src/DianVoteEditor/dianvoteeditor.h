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
#include <QList>
#include <QRectF>

class QFile;
class QSlideScene;
class QStandardItemModel;
class QModelIndex;

class DianVoteEditor : public QMainWindow
{
    Q_OBJECT

public:
    DianVoteEditor();
    ~DianVoteEditor();

    // setup the user interface using uiFile.
    void setupUi(const QString& uiFile);

    // set application's style using style sheet.
    void setStyle(const QString& qssFile);

public slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void aboutSoftware();
    void documentWasModified();
    void sceneAreaChanged(const QList<QRectF> & region);

    // slide list clicked
    void slideListActivated(const QModelIndex &);

    // the slides may be save at close.
    void closeEvent(QCloseEvent *event);

private:
    // setup the main window actions, toolbar, statusbar.
    void setupActions();
    void setupToolBars();
    void setupStatusBar();
    // update the slide list by slideScenes.
    void updateSlideList();
    // read & write application settings for configuration.
    void readSettings();
    void writeSettings();

    bool maybeSave();
    // load & save the slide file from specific filename.
    bool loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);

    // new a single slide scene with default template.
    QSlideScene* newSlide();

    // clean the resources.
    void clean();

    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    /**
    * @brief manage the file processing.
    * @param isSplit split the file to many slide or merge
    *       merge many slides to a file.
    */
    void fileProcessing(QFile &xmlFile, bool isSplit);

private:
    QMainWindow *dianvoteWindow;
    QString curFile;
    bool isUntitled;

    // slideScenes store all slides.
    QList<QSlideScene *> slideScenes;
    QSlideScene *currentSlideScene;
    // slidesListModel store all slides' outline with list.
    QStandardItemModel *slidesListModel;
};

#endif // DIANVOTEPLAYER_H
