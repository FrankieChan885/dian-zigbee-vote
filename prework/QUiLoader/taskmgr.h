/**
 * taskmgr.h
 * 
 * TaskMgr dialog definition file.
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
#include <QDialog>
#include <QTimer>
#include <QString>

class TaskMgr : public QDialog
{
    Q_OBJECT
private:
    // timer for processes list update
    QTimer *timer4Update;
    // the time between two update events.
    int timespace;
    // the ui dialog
    QWidget *taskMgrDlg;
public:
    TaskMgr(QWidget * parent = 0, Qt::WFlags flags = 0);
    ~TaskMgr();
    // setup the user interface using uiFile for the parent.
    void setupUi(const QString& uiFile, QWidget *parent);
public slots:
    void updateProcList();

}; // class TaskMgr

