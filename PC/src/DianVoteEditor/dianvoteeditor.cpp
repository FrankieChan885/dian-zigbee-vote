/**
 * dianvoteeditor.h
 *
 * DianVoteEditor main window definition file.
 * DianVoteEditor 类是该软件的播放主窗口。
 *
 * Author: tankery.chen@gmail.com
 * Modified:
 *      date        author      action
 *  ------------------------------------------------------------
 *      2011-03-15  tankery     created file
 *
 * Copyright (c) Tankery Chen 2011 @ Dian Group
 */
#include "dianvoteeditor.h"
#include "../utilities/exceptions.h"
#include "../utilities/slideview.h"
#include "../utilities/slidemodel.h"

#include <QApplication>
#include <QLatin1String>
#include <QFile>
#include <QDir>
#include <QtUiTools/QUiLoader>
#include <QVBoxLayout>

DianVoteEditor::DianVoteEditor()
{
}

DianVoteEditor::~DianVoteEditor()
{

}

// setup the user interface using uiFile for the parent.
void DianVoteEditor::setupUi(const QString& uiFile, QWidget *parent)
{
    QUiLoader uiLoader(this);
    QFile uif(uiFile);
    if (!uif.open(QIODevice::ReadOnly)) {
        throw new DianVoteException(DianVoteException::UI_FILE_NOTFOUND);
    }
    dianvoteWindow = (QMainWindow*) uiLoader.load(&uif);
    uif.close();

    if (dianvoteWindow == 0) {
        throw new DianVoteException(DianVoteException::UI_LOAD_FAILED);
    }

    QDir dir(QCoreApplication::applicationDirPath());

    // set the slide model.
    QFile xmlf(dir.absoluteFilePath("userdata/one-topic.xml"));
    if (!xmlf.exists()) {
        throw new DianVoteException(DianVoteException::TOPICS_FILE_NOTFOUND);
    }
    xmlf.open(QFile::ReadWrite);
    QSlideModel *slideModel = new QSlideModel(xmlf.readAll());
    xmlf.close();

    // set the slide view
    QSlideView *slideView = new QSlideView;
    slideView->loadNewSlide(slideModel);
    // set the view to this window's central widget.
    dianvoteWindow->setCentralWidget(slideView);

    // set parent to the specific one.
    dianvoteWindow->setParent(parent);

    // show it!
    dianvoteWindow->show();

    QFile qss(dir.absoluteFilePath("../../resources/skins/default.qss"));
    if (qss.open(QFile::ReadOnly)) {
        qApp->setStyleSheet(QLatin1String(qss.readAll()));
        qss.close();
    }
    else {
        throw new DianVoteException(DianVoteException::QSS_FILE_NOTFOUND);
    }
}

