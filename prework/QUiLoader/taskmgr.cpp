/**
 * taskmgr.cpp
 * 
 * TaskMgr dialog implimentaion file.
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
#include "taskmgr.h"
#include <QUiLoader>
#include <QFile>
#include <QDir>
#include <QTableWidget>
#include <QStringList>

TaskMgr::TaskMgr(QWidget * parent/* = 0*/, Qt::WFlags flags/* = 0*/)
: QDialog(parent, flags)
, timespace(1000)
, taskMgrDlg(0)
{
    setupUi(QDir::toNativeSeparators("skin/taskmgr.ui"), this);

    timer4Update = new QTimer();
    connect(timer4Update, SIGNAL(timeout()), this, SLOT(updateProcList()));
    timer4Update->start(timespace);
}

TaskMgr::~TaskMgr()
{
    timer4Update->stop();
    delete timer4Update;
}

void TaskMgr::setupUi(const QString& uiFile, QWidget *parent)
{
    QUiLoader uiLoader;
    QFile file(uiFile);
    taskMgrDlg = uiLoader.load(&file);
    if (taskMgrDlg == 0) {
        throw 0;
    }
    QTableWidget *table = taskMgrDlg->findChild<QTableWidget*>("procList");
    if (table) {
        table->setRowCount(10);
        table->setColumnCount(5);
        table->setHorizontalHeaderLabels(QStringList() << "Name" << "ID");
    }
    taskMgrDlg->setParent(parent);
}

void TaskMgr::updateProcList()
{
    QTableWidget *table = taskMgrDlg->findChild<QTableWidget*>("procList");
    if (table) {
    }
}

