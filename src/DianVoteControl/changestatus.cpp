#include <QtGui>
#include "drawbase.h"
#include "dianvotecontrol.h"
#include "exceptions.h"
#include "hidcontrol.h"
#include "stopwatch.h"
#include "dianvotedebug.h"

void DianVoteControl::VoteStart()
{
    StartNewSession();

    if(curState == RUNNING)
    {
        QMessageBox::critical(0, "warn", "Already Started");
    }else if(curState == PAUSE)
    {
        // 修改主界面
        pbStart->hide();
        pbPause->show();

        // 启动秒表
        stopWatch->start();

        StartHid();     // 开启接收设备
    }
    else if(curState == STOP)
    {
        // 这里会把hid打开
        if(!PrepareHid())
        {
            // 把错误信息写入log文件
            return;
        }

        int num = GetOptionNum();
        if(num)
        {
            emit setOptionNum(num);     // 获取选项个数
            optionNumList.append(num);
        }
        else
        {
            return;
        }
        if(!StartHid())     // 开启接收设备
        {
            return;
        }

        // 修改主界面
        pbStart->hide();
        pbAuto->hide();
        pbPause->show();
        pbStop->show();

        // 画出秒表, 递增模式
        ShowStopWatch();
        Q_ASSERT(stopWatch != NULL);
        stopWatch->setMode(STOP_WATCH_INCREASE_MODE);
        stopWatch->start();
        resizeAnimation->start();
    }

    // 改变状态
    curState = RUNNING;
}

void DianVoteControl::VoteAuto()
{
    StartNewSession();

    if(curState == RUNNING)
    {
        QMessageBox::critical(0, "warn", "Already Started");
    }
    else if(curState == PAUSE)
    {
        // 修改主界面
        pbStart->hide();
        pbPause->show();

        // 启动秒表
        stopWatch->start();

        StartHid();     // 开启接收设备
    }
    else if(curState == STOP)
    {
        // 这里会把hid打开
        if(!PrepareHid())
        {
            // 把错误信息写入log文件
            return;
        }

        int num1 = GetLastTime();     // 获取持续时间

        if(!num1)
        {
            return;
        }

        int num0 = GetOptionNum();     // 获取选项个数
        if(!num0)
        {
            return;
        }

        // 画出秒表, 递减模式
        ShowStopWatch();
        Q_ASSERT(stopWatch != NULL);
        stopWatch->setMode(STOP_WATCH_DECREASE_MODE);

        emit setLastTime(num1);
        emit setOptionNum(num0);
        optionNumList.append(num0);
        if(!StartHid())     // 开启接收设备
        {
            return;
        }

        // 修改主界面
        pbStart->hide();
        pbAuto->hide();
        pbPause->show();
        pbStop->show();

        stopWatch->start();
        resizeAnimation->start();
    }

    // 改变状态
    curState = RUNNING;
}

void DianVoteControl::VotePause()
{
    if(curState == RUNNING)
    {
        Q_ASSERT(stopWatch != NULL);
        stopWatch->pause();

        // 修改主界面
        pbPause->hide();
        pbStart->show();

        // 停止USB设备
        if(hidControl == NULL)
        {
            QMessageBox::critical(0, "Internal Error", "Can't Stop HidDevice");
            return;
        }
        StopHid();  // 停止
    }

    // 修改状态
    curState = PAUSE;
}

void DianVoteControl::VoteStop()
{
    emit displayResult(true);   // 显示结果
    SavePreviousSession();
    ClearLogList(); // 不管如何，清空日志链表是必须的。

    if(curState == RUNNING)
    {
        Q_ASSERT(stopWatch != NULL);
        stopWatch->stop();

        // 修改主界面
        HideStopWatch();
        pbPause->hide();
        pbStop->hide();
        pbStart->show();
        pbAuto->show();

        // 停止USB设备
        if(hidControl == NULL)
        {
            QMessageBox::critical(0, "Internal Error", "Can't Stop HidDevice");
            return;
        }
        StopHid();  // 停止
    }else if(curState == PAUSE)
    {
        Q_ASSERT(stopWatch != NULL);
        stopWatch->stop();

        // 修改主界面
        HideStopWatch();
        pbPause->hide();
        pbStop->hide();
        pbStart->show();
        pbAuto->show();

        // 因为pause状态hidCntrol已经停止，所以不需要停止
    }

    resizeAnimation->start();
    // 修改状态
    curState = STOP;
}

