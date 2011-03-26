/**
 * dianvoteplayer.cpp
 *
 * DianVotePlayer main window implementation file.
 * DianVotePlayer 类是该软件的播放主窗口。
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

class DianVotePlayer : public QObject
{
    Q_OBJECT

public:
    DianVotePlayer();
    ~DianVotePlayer();

    // setup the user interface using uiFile for the parent.
    void setupUi(const QString& uiFile, QWidget *parent);

private:
    QMainWindow *dianvoteWindow;
};

#endif // DIANVOTEPLAYER_H
