#include <QDesktopWidget>
#include "dianvotecontrol.h"
#include "shortcut.h"
#include "qxtglobalshortcut.h"

// 注册全局快捷键
void DianVoteControl::RegistGlobalShortCut()
{
    startOrStop = new QxtGlobalShortcut(QKeySequence(START_OR_STOP), this);
    connect(startOrStop, SIGNAL(activated()),
            this, SLOT(StartOrStop()));

    showOrHideControler = new QxtGlobalShortcut(QKeySequence(SHOW_HIDE_CONTROLER), this);
    connect(showOrHideControler, SIGNAL(activated()),
            this, SLOT(ShowOrHideControler()));

    showOrHideResult = new QxtGlobalShortcut(QKeySequence(SHOW_HIDE_RESULT), this);
    connect(showOrHideResult, SIGNAL(activated()),
            this, SLOT(ShowOrHideResult()));
}

void DianVoteControl::StartOrStop()
{
    if(curState == STOP || curState == PAUSE)
    {
        VoteStart();
    }
    else
    {
        VoteStop();
    }
}

// 借助Application::desktop()获取到的桌面信息
// 可以自由的将窗口从主屏幕移动到扩展屏幕或者反过来
void DianVoteControl::ShowOrHideControler()
{
    int screenCount = desktopWidget->screenCount();
    if( screenCount <= 1)
    {
        return;
    }

    int atWitchWindow = desktopWidget->screenNumber(this);
    QRect ag = desktopWidget->availableGeometry((atWitchWindow + 1) % screenCount);
    this->move(ag.topLeft());
}

void DianVoteControl::ShowOrHideResult()
{
    if(!this->drawer->isVisible())
    {
        DoShowResults();
        return;
    }
    int screenCount = desktopWidget->screenCount();
    if(desktopWidget->screenCount() <= 1)
    {
        return;
    }

    int atWitchWindow = desktopWidget->screenNumber(this->drawer);
    QRect ag = desktopWidget->availableGeometry((atWitchWindow + 1) % screenCount);
    this->drawer->move(ag.topRight().x() - this->drawer->width() - 20,
                       ag.topRight().y());
}
