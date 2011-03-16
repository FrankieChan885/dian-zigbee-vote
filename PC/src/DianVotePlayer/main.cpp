/**
 * main.cpp
 *
 * DianVote Player 应用程序主文件。
 * 		DianVote Player 是用于题库播放的软件。
 * 	该软件将加载编辑器生成的文件，像 ppt 一样播放出来，并可以在播放过程中
 * 	投票和显示结果。
 *
 * Author: tankery.chen@gmail.com
 * Modified:
 *      date        author      action
 *  ------------------------------------------------------------
 *      2011-03-11  tankery     created file
 *
 * Copyright (c) Tankery Chen 2011 @ Dian Group
 */
#include "dianvoteplayer.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DianVotePlayer w;
    w.show();
    return a.exec();
}
