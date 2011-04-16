/**
 * main.cpp
 *
 * DianVote Editor 应用程序主文件。
 * 		DianVote Editor 是用于题库播放的软件。
 * 	该软件将加载编辑器生成的文件，像 ppt 一样播放出来，并可以在播放过程中
 * 	投票和显示结果。
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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QDir dir(QCoreApplication::applicationDirPath());

    try {
        DianVoteEditor *w = new DianVoteEditor();
        w->setupUi(dir.absoluteFilePath("res/dianvoteeditor.ui"), 0);
    } catch (DianVoteException *uie) {
        QMessageBox::critical(0, "error", uie->what());
        return 0;
    } catch (XmlStreamException *xse) {
        QMessageBox::critical(0, "error", xse->what());
        return 0;
    }
    return a.exec();
}
