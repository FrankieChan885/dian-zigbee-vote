#ifndef DIANVOTECONTROL_H
#define DIANVOTECONTROL_H

#include <QWidget>
#include <QByteArray>
#include "dianvotedrawer.h"

class QSize;
class QFile;
class QInputDialog;
class QSpacerItem;
class QErrorMessage;
class QSequentialAnimationGroup;
class QPropertyAnimation;
class QPushButton;
class HidControl;
class StopWatch;
class QSplashScreen;

#define DEFAULT_DOCK_SPACE 20       // 吸附产生时最大反应距离
                                    // 当窗口里桌面四周的距离小于或者等于这个距离时
                                    // 窗口就吸附到桌面边框上
#define MAP_VALUE   4               // 手持端按键到选项名索引的映射关系
#define DEFAULT_OPTION_NUM 4        // 默认的投票的选项数目
#define MAXIMUN_OPTION_NUM 5        // 投票题目选项的最大值
#define DEFAULT_DEADLINE 60         // 秒表倒计时时的默认值
#define MAXIMUN_DEADLINE 600        // 秒表倒计时时的最大值

enum ControlState
{
    RUNNING,
    PAUSE,
    STOP
};

enum ReceivedDataType
{
    NORMAL,
    DUPLICATE,
    ERROR_ID,
    ERROR_OPTION,
    ERROR_DATA,
    UNKNOWN
};

typedef struct _RevData
{
    quint32 id;   // 手持端ID
    quint8 key;   // 手持端按键的编号
    QString *revTime;
    enum ReceivedDataType type;
}RevData;

namespace Ui {
    class DianVoteControl;
}

class DianVoteControl : public QWidget
{
    Q_OBJECT

public:
    explicit DianVoteControl(QWidget *parent = 0);
    ~DianVoteControl();
    static void DianVoteMsgHandler(QtMsgType type, const char *msg);

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

private slots:
//    void DoShowStopWatch();     // 显示秒表widget，在下拉动画完毕后调用
    void DoHideStopWatch();     // 删除秒表widget，在上拉动画完毕后调用

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    QList< RevData* > *log;     // 数据接收的log

    Ui::DianVoteControl *ui;
    DianVoteDrawer *drawer;
    HidControl *hidControl;
    StopWatch *stopWatch;
    QSplashScreen *splash;      // 欢迎界面

    static QFile *VoteLog;             // 日志文件

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

    QPropertyAnimation *resizeAnimation;
//    QPropertyAnimation *showStopWatchAnimation;

    // 主控界面位置及大小
    QPoint prePoint;        // 记录动画开始时的位置
    QSize initSize;         // 窗口初始化时的大小
    // 控制状态
    enum ControlState curState; // 当前状态

private:
    int GetOptionNum();    // 获取选项个数，并传给histgram
    int GetLastTime();      // 获取秒表倒计时时间
    bool StartHid();        // 开启接收设备
    bool StopHid();         // 关闭接收设备
    bool PrepareHid();     // 准备好接收设备
    void LoadStyleSheet(const QString &sheetname);
    void ShowStopWatch();   // 显示秒表
    void HideStopWatch();   // 删掉秒表
    void WriteRevDataLog(RevData *rd);  // 在每一次接收到数据时，都将这次投票的所有记录写入文件，并清空log列表
    void ClearLogList();        // 清空log链表，在此之前保存到log文件中
};

#endif // DIANVOTECONTROL_H
