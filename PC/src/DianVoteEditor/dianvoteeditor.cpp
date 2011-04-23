/**
 * dianvoteeditor.h
 *
 * DianVoteEditor main window definition file.
 *
 * Author: tankery.chen@gmail.com
 * Modified:
 *      date        author      action
 *  ------------------------------------------------------------
 *      2011-03-15  tankery     created file
 *
 * Copyright (c) Tankery Chen 2011 @ Dian Group
 */
#include <QApplication>
#include <QAction>
#include <QLatin1String>
#include <QFile>
#include <QDir>
#include <QtUiTools/QUiLoader>
#include <QHBoxLayout>
#include <QGraphicsView>

#include "dianvoteeditor.h"
#include "slideeditor.h"
#include "../utilities/exceptions.h"
#include "../utilities/slidemodel.h"
#include "../utilities/slidescene.h"

DianVoteEditor::DianVoteEditor()
        : dianvoteWindow(0)
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

    setupActions();

    QDir::setCurrent(QCoreApplication::applicationDirPath());
    QDir dir(QDir::current());

    // set the slide model.
    QFile xmlf(dir.absoluteFilePath("userdata/one-topic.xml"));
    if (!xmlf.exists()) {
        throw new DianVoteException(DianVoteException::TOPICS_FILE_NOTFOUND);
    }
    xmlf.open(QFile::ReadOnly);
    QString xmls(xmlf.readAll());
    xmlf.close();

    // new a slide editor to main window.
    slideEditor = new QSlideEditor(xmls, dianvoteWindow);

    slideEditor->setBackgroundPixmap(
            dir.absoluteFilePath("res/images/default-background.png"));

    // set the view to this window's central widget.
    dianvoteWindow->setCentralWidget(slideEditor);

    // set parent to the specific one.
    dianvoteWindow->setParent(parent);
}

// setup the main window actions.
void DianVoteEditor::setupActions()
{
    QAction *action = dianvoteWindow->findChild<QAction *>(tr("action_Quit"));
    QObject::connect(action, SIGNAL(triggered()), dianvoteWindow, SLOT(close()));
    QObject::connect(action, SIGNAL(triggered()), this, SLOT(onDestroy()));
}

// set application's style using style sheet.
void DianVoteEditor::setStyle(const QString& qssFile)
{
    QFile qss(qssFile);
    if (qss.open(QFile::ReadOnly)) {
        qApp->setStyleSheet(QLatin1String(qss.readAll()));
        qss.close();
    }
    else {
        throw new DianVoteException(DianVoteException::QSS_FILE_NOTFOUND);
    }
}

void DianVoteEditor::show()
{
    // show it!
    return dianvoteWindow->show();
}

void DianVoteEditor::onDestroy() {
    qDebug("DianVoteEditor::onDestroy()");

    QDir dir(QDir::current());
    // set the slide model.
    slideEditor->save(dir.absoluteFilePath("userdata/one-topic.xml"));
}

