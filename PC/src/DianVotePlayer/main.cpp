/**
 * main.cpp
 *
 * The `DianVote Player' application's main file.
 * 		DianVote Player is a software that showing the topic's slide.
 *  This software will loading the file created by Eidtor, and display
 *  it like the PPT app, the voting and displaying of vote result is
 *  also available.
 *
 * Author: tankery.chen@gmail.com
 * Modified:
 *      date        author      action
 *  ------------------------------------------------------------
 *      2011-03-11  tankery     created file
 *      2011-03-17  tankery     add exception branch
 *
 * Copyright (c) Tankery Chen 2011 @ Dian Group
 */
#include "../utilities/slidescene.h"
#include "../utilities/slidemodel.h"
#include "../utilities/exceptions.h"
#include "../utilities/hiddevice.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QGraphicsView>
#include <QImage>
#include <QMatrix>
#include <QFile>
#include <QDir>
#include <limits>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QDir::setCurrent(QCoreApplication::applicationDirPath());
    QDir dir(QDir::current());

    // set the slide model.
    QFile xmlf(dir.absoluteFilePath("userdata/one-topic.xml"));
    if (!xmlf.exists()) {
        QMessageBox::critical(0, "error", "no xml file...");
        return 0;
    }
    xmlf.open(QFile::ReadWrite);
    QSlideModel model(xmlf.readAll());
    xmlf.close();


    // set the slide scene
    QSlideScene *scene = new QSlideScene(0, &model);
    QPixmap pixmap(dir.absoluteFilePath("res/images/default-background.png"));
    scene->setBackgroundBrush(Qt::black);
    scene->setBackgroundPixmap(pixmap);

    //set slide view
    QGraphicsView *view = new QGraphicsView(scene);

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFrameShape(QFrame::NoFrame);
    // editing is not allowed.
    view->setInteractive(false);

    // scale
    double newScale = QApplication::desktop()->height() / scene->sceneRect().height();
    QMatrix oldMatrix = view->matrix();
    view->resetMatrix();
    view->translate(oldMatrix.dx(), oldMatrix.dy());
    view->scale(newScale, newScale);

    view->showFullScreen();

    QHidDevice * test = new QHidDevice(0x55, 0x32, 1, &a);

    return a.exec();
}
