/********************************************************************************
** Form generated from reading UI file 'dianvotedrawer.ui'
**
** Created: Sat Apr 23 21:56:09 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIANVOTEDRAWER_H
#define UI_DIANVOTEDRAWER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DianVoteDrawer
{
public:

    void setupUi(QWidget *DianVoteDrawer)
    {
        if (DianVoteDrawer->objectName().isEmpty())
            DianVoteDrawer->setObjectName(QString::fromUtf8("DianVoteDrawer"));
        DianVoteDrawer->resize(400, 300);

        retranslateUi(DianVoteDrawer);

        QMetaObject::connectSlotsByName(DianVoteDrawer);
    } // setupUi

    void retranslateUi(QWidget *DianVoteDrawer)
    {
        DianVoteDrawer->setWindowTitle(QApplication::translate("DianVoteDrawer", "DianVoteDrawer", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DianVoteDrawer: public Ui_DianVoteDrawer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIANVOTEDRAWER_H
