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
#include "../utilities/slideview.h"
#include "../utilities/slidemodel.h"

#include <QApplication>
#include <QLatin1String>
#include <QFile>
#include <QtUiTools/QUiLoader>
#include <QVBoxLayout>

DianVotePlayer::DianVotePlayer()
{
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
        throw new DianVoteException(DianVoteException::UI_FILE_NOTFOUND);
    }

    // set the slide view and model.
    QSlideModel *slideModel = new QSlideModel("../userdata/one-topic.xml");
    QSlideView *slideView = new QSlideView;
    slideView->loadNewSlide(slideModel);
    // set the view to this window's central widget.
    dianvoteWindow->setCentralWidget(slideView);

    // set parent to the specific one.
    dianvoteWindow->setParent(parent);

    // show it!
    dianvoteWindow->show();

    QFile qss(":/skins/default.qss");
    if (qss.open(QFile::ReadOnly)) {
        qApp->setStyleSheet(QLatin1String(qss.readAll()));
        qss.close();
    }
    else {
        throw new DianVoteException(DianVoteException::QSS_FILE_NOTFOUND);
    }
}

