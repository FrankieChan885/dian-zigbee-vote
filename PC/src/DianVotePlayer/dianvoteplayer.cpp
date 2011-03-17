/**
 * dianvoteplayer.h
 *
 * DianVotePlayer main window definition file.
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
#include "dianvoteplayer.h"
#include "../utilities/exceptions.h"

#include <QDir>
#include <QtUiTools/QUiLoader>
#include <QFile>

DianVotePlayer::DianVotePlayer(QWidget *parent)
    : QWidget(parent)
{
    setupUi(QDir::toNativeSeparators("res/dianvoteplayer.ui"), this);
}

DianVotePlayer::~DianVotePlayer()
{

}

// setup the user interface using uiFile for the parent.
void DianVotePlayer::setupUi(const QString& uiFile, QWidget *parent)
{
    QUiLoader uiLoader(this);
    QFile file(uiFile);
    dianvoteWindow = (QMainWindow*) uiLoader.load(&file);
    if (dianvoteWindow == 0) {
        throw new UiException(UiException::UI_FILE_NOTFOUND);
    }

    // set parent to the specific one.
    dianvoteWindow->setParent(parent);
}
