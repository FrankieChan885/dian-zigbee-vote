/********************************************************************************
** Form generated from reading UI file 'dianvotedrawer.ui'
**
** Created: Tue Apr 26 06:09:01 2011
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
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DianVoteDrawer
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *DrawConvas;
    QHBoxLayout *OptionalBar;
    QSpacerItem *horizontalSpacer_3;
    QToolButton *SwitchDisplayMode;
    QToolButton *Compare;
    QToolButton *CorrectAnswer;
    QToolButton *Options;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer_17;
    QSpacerItem *horizontalSpacer_15;
    QSpacerItem *horizontalSpacer_16;
    QToolButton *Close;
    QSpacerItem *horizontalSpacer_10;
    QHBoxLayout *ControlBar;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *horizontalSpacer_8;
    QSpacerItem *horizontalSpacer_11;
    QSpacerItem *horizontalSpacer_13;
    QSpacerItem *horizontalSpacer_14;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_7;
    QToolButton *Previous;
    QSpacerItem *horizontalSpacer_9;
    QToolButton *Next;
    QSpacerItem *horizontalSpacer_12;
    QSpacerItem *horizontalSpacer_5;

    void setupUi(QWidget *DianVoteDrawer)
    {
        if (DianVoteDrawer->objectName().isEmpty())
            DianVoteDrawer->setObjectName(QString::fromUtf8("DianVoteDrawer"));
        DianVoteDrawer->resize(491, 464);
        gridLayout = new QGridLayout(DianVoteDrawer);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        DrawConvas = new QVBoxLayout();
        DrawConvas->setSpacing(6);
        DrawConvas->setObjectName(QString::fromUtf8("DrawConvas"));

        gridLayout->addLayout(DrawConvas, 1, 0, 1, 1);

        OptionalBar = new QHBoxLayout();
        OptionalBar->setSpacing(6);
        OptionalBar->setContentsMargins(0, 0, 0, 0);
        OptionalBar->setObjectName(QString::fromUtf8("OptionalBar"));
        OptionalBar->setSizeConstraint(QLayout::SetFixedSize);
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        OptionalBar->addItem(horizontalSpacer_3);

        SwitchDisplayMode = new QToolButton(DianVoteDrawer);
        SwitchDisplayMode->setObjectName(QString::fromUtf8("SwitchDisplayMode"));
        SwitchDisplayMode->setEnabled(true);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/pics/res/pie.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QString::fromUtf8(":/new/pics/res/histgram-32.png"), QSize(), QIcon::Normal, QIcon::On);
        SwitchDisplayMode->setIcon(icon);
        SwitchDisplayMode->setIconSize(QSize(32, 32));
        SwitchDisplayMode->setCheckable(false);

        OptionalBar->addWidget(SwitchDisplayMode);

        Compare = new QToolButton(DianVoteDrawer);
        Compare->setObjectName(QString::fromUtf8("Compare"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/new/pics/res/compare-32.png"), QSize(), QIcon::Normal, QIcon::Off);
        Compare->setIcon(icon1);
        Compare->setIconSize(QSize(32, 32));

        OptionalBar->addWidget(Compare);

        CorrectAnswer = new QToolButton(DianVoteDrawer);
        CorrectAnswer->setObjectName(QString::fromUtf8("CorrectAnswer"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/new/pics/res/correct-answer-32.png"), QSize(), QIcon::Normal, QIcon::Off);
        CorrectAnswer->setIcon(icon2);
        CorrectAnswer->setIconSize(QSize(32, 32));
        CorrectAnswer->setAutoRepeatDelay(300);
        CorrectAnswer->setAutoRepeatInterval(50);
        CorrectAnswer->setPopupMode(QToolButton::MenuButtonPopup);

        OptionalBar->addWidget(CorrectAnswer);

        Options = new QToolButton(DianVoteDrawer);
        Options->setObjectName(QString::fromUtf8("Options"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/new/pics/res/options.png"), QSize(), QIcon::Normal, QIcon::Off);
        Options->setIcon(icon3);
        Options->setIconSize(QSize(32, 32));
        Options->setPopupMode(QToolButton::InstantPopup);

        OptionalBar->addWidget(Options);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        OptionalBar->addItem(horizontalSpacer);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        OptionalBar->addItem(horizontalSpacer_4);

        horizontalSpacer_17 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        OptionalBar->addItem(horizontalSpacer_17);

        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        OptionalBar->addItem(horizontalSpacer_15);

        horizontalSpacer_16 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        OptionalBar->addItem(horizontalSpacer_16);

        Close = new QToolButton(DianVoteDrawer);
        Close->setObjectName(QString::fromUtf8("Close"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/new/pics/res/error-answer-32.png"), QSize(), QIcon::Normal, QIcon::Off);
        Close->setIcon(icon4);
        Close->setIconSize(QSize(32, 32));

        OptionalBar->addWidget(Close);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        OptionalBar->addItem(horizontalSpacer_10);


        gridLayout->addLayout(OptionalBar, 2, 0, 1, 1);

        ControlBar = new QHBoxLayout();
        ControlBar->setSpacing(6);
        ControlBar->setObjectName(QString::fromUtf8("ControlBar"));
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        ControlBar->addItem(horizontalSpacer_6);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        ControlBar->addItem(horizontalSpacer_8);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        ControlBar->addItem(horizontalSpacer_11);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        ControlBar->addItem(horizontalSpacer_13);

        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        ControlBar->addItem(horizontalSpacer_14);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        ControlBar->addItem(horizontalSpacer_2);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        ControlBar->addItem(horizontalSpacer_7);

        Previous = new QToolButton(DianVoteDrawer);
        Previous->setObjectName(QString::fromUtf8("Previous"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/new/pics/res/previous-32.png"), QSize(), QIcon::Normal, QIcon::Off);
        Previous->setIcon(icon5);
        Previous->setIconSize(QSize(32, 32));

        ControlBar->addWidget(Previous);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        ControlBar->addItem(horizontalSpacer_9);

        Next = new QToolButton(DianVoteDrawer);
        Next->setObjectName(QString::fromUtf8("Next"));
        Next->setCursor(QCursor(Qt::ArrowCursor));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/new/pics/res/next-32.png"), QSize(), QIcon::Normal, QIcon::Off);
        Next->setIcon(icon6);
        Next->setIconSize(QSize(32, 32));

        ControlBar->addWidget(Next);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        ControlBar->addItem(horizontalSpacer_12);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        ControlBar->addItem(horizontalSpacer_5);


        gridLayout->addLayout(ControlBar, 0, 0, 1, 1);


        retranslateUi(DianVoteDrawer);

        QMetaObject::connectSlotsByName(DianVoteDrawer);
    } // setupUi

    void retranslateUi(QWidget *DianVoteDrawer)
    {
        DianVoteDrawer->setWindowTitle(QApplication::translate("DianVoteDrawer", "DianVoteDrawer", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        DianVoteDrawer->setToolTip(QApplication::translate("DianVoteDrawer", "Close", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        SwitchDisplayMode->setToolTip(QApplication::translate("DianVoteDrawer", "pie&hist", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        SwitchDisplayMode->setText(QString());
#ifndef QT_NO_TOOLTIP
        Compare->setToolTip(QApplication::translate("DianVoteDrawer", "Compare", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        Compare->setText(QString());
#ifndef QT_NO_TOOLTIP
        CorrectAnswer->setToolTip(QApplication::translate("DianVoteDrawer", "CorrectAnswer", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        CorrectAnswer->setText(QString());
#ifndef QT_NO_TOOLTIP
        Options->setToolTip(QApplication::translate("DianVoteDrawer", "Options", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        Options->setText(QString());
        Close->setText(QString());
#ifndef QT_NO_TOOLTIP
        Previous->setToolTip(QApplication::translate("DianVoteDrawer", "Previous", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        Previous->setText(QString());
#ifndef QT_NO_TOOLTIP
        Next->setToolTip(QApplication::translate("DianVoteDrawer", "Next", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        Next->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DianVoteDrawer: public Ui_DianVoteDrawer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIANVOTEDRAWER_H
