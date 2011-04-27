/**
 * main.cpp
 *
 * DianVote Editor Application's main file.
 * 	    DianVote Editor is a software for topics editting.
 *  This software will edit the file, and save it in an xml like file.
 *  The xml like file can be load by DianVote Player for displaying.
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
#include "dianvoteeditor.h"
#include "../utilities/exceptions.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QDir>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // set encode translate codec.
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Utf8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Utf8"));

    QDir dir(QCoreApplication::applicationDirPath());

    try {
        DianVoteEditor *w = new DianVoteEditor();
        w->setupUi(dir.absoluteFilePath("res/dianvoteeditor.ui"), 0);
        w->setStyle(dir.absoluteFilePath("res/skins/default.qss"));
        w->show();
    } catch (DianVoteException *uie) {
        QMessageBox::critical(0, "error", uie->what());
        return 0;
    } catch (XmlStreamException *xse) {
        QMessageBox::critical(0, "error", xse->what());
        return 0;
    }
    return a.exec();
}
