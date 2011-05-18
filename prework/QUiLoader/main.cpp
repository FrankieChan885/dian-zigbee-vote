/**
 * main.cpp
 * 
 * TaskMgr application main file.
 * TaskMgr dialog contains a list widget to list all process in system.
 * update every 1 second.
 *
 * Author: tankery.chen@gmail.com
 * Modified:
 *      date        author      action
 *  ------------------------------------------------------------
 *      2011-03-11  tankery     created file
 *
 * Copyright (c) Tankery Chen 2011 @ Dian Group
 */
#include <QApplication>
#include <QMessageBox>
#include "taskmgr.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    try {
        TaskMgr *mgr = new TaskMgr();
        mgr->show();
    } catch (int n) {
        if (n == 0) {
            QMessageBox::critical(0, "error", "can not find ui file...");
            exit(0);
        }
        else {
            ;
        }
    } catch (...) {
        QMessageBox::critical(0, "error", "unknow problem...");
        exit(0);
    }

    return a.exec();
}
