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

class QSlideEditor;

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

    // set application's style using style sheet.
    void setStyle(const QString& qssFile);

    void show();

public slots:
    void onDestroy();

private:
    QMainWindow *dianvoteWindow;
    QSlideEditor *slideEditor;
};

#endif // DIANVOTEPLAYER_H
