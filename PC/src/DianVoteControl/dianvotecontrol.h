#ifndef DIANVOTECONTROL_H
#define DIANVOTECONTROL_H

#include <QWidget>
#include <QByteArray>
#include "dianvotedrawer.h"

class QInputDialog;
class QSpacerItem;
class QErrorMessage;
class QSequentialAnimationGroup;
class QPropertyAnimation;
class QPushButton;
class HidControl;
class StopWatch;

#define MAP_VALUE   4               // 手持端按键到选项名索引的映射关系

typedef struct _RevData
{
    quint32 id;   // 手持端ID
    char key;   // 手持端按键的编号
    QString *revTime;
}RevData;

enum ControlState
{
    RUNNING,
    PAUSE,
    STOP
};

namespace Ui {
    class DianVoteControl;
}

class DianVoteControl : public QWidget
{
    Q_OBJECT

public:
    explicit DianVoteControl(QWidget *parent = 0);
    ~DianVoteControl();

signals:
    void clearDrawData();           // 信号--清空数据
    void setLastTime(int sec);      // 信号--设置持续时间
    void setOptionNum(int num);     // 信号--给histgram设置选项数目
    void updateGraph(int index);

public slots:

    //-----control part-----//
    void VoteStart();
    void VotePause();
    void VoteStop();
    void VoteAuto();
    //-----control part-----//

    //-----display part-----//
    void DoShowResults();
    void DoShowStatics();
    //-----display part-----//

    //-----data pre-process part-----//
    void ParseData(quint32 id, quint8 option);
    //-----data pre-process part-----//

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    QList< RevData* > *log;     // 数据接收的log

    Ui::DianVoteControl *ui;
    DianVoteDrawer *drawer;
    HidControl *hidControl;
    StopWatch *stopWatch;

    // buttons
    QPushButton *pbStart;
    QPushButton *pbAuto;
    QPushButton *pbResult;
    QPushButton *pbPause;
    QPushButton *pbStop;
    QPushButton *pbOption;
    QPushButton *pbClose;

    QInputDialog *getOptionNum;      // 弹出窗口，设置选项个数
    QInputDialog *getLastTime;      // 当用Auto模式时需要设置

    QPoint dragPosition;

    QSequentialAnimationGroup *animationGroup;
    QPropertyAnimation *resizeAnimation;
//    QPropertyAnimation *showStopWatchAnimation;

    // 控制状态
    enum ControlState curState; // 当前状态

private slots:
//    void DoShowStopWatch();     // 显示秒表widget，在下拉动画完毕后调用
    void DoHideStopWatch();     // 删除秒表widget，在上拉动画完毕后调用

private:
    int GetOptionNum();    // 获取选项个数，并传给histgram
    int GetLastTime();      // 获取秒表倒计时时间
    bool StartHid();        // 开启接收设备
    bool StopHid();         // 关闭接收设备
    bool PrepareHid();     // 准备好接收设备
    void LoadStyleSheet(const QString &sheetname);
    void ShowStopWatch();   // 显示秒表
    void HideStopWatch();   // 删掉秒表
};

#endif // DIANVOTECONTROL_H
