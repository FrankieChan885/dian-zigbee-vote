#ifndef DIANVOTECONTROL_H
#define DIANVOTECONTROL_H

#include <map>
#include <QWidget>
#include <QByteArray>
#include "dianvoteoption.h"
#include "dianvotedrawer.h"

class HidControl;
class StopWatch;
class QxtGlobalShortcut;
class QSize;
class QFile;
class QMenu;
class QAction;
class QDialog;
class QInputDialog;
class QSpacerItem;
class QErrorMessage;
class QDesktopWidget;
class QPropertyAnimation;
class QPushButton;
class QSplashScreen;

#define DEFAULT_DOCK_SPACE 20       // 吸附产生时最大反应距离
                                    // 当窗口里桌面四周的距离小于或者等于这个距离时
                                    // 窗口就吸附到桌面边框上
#define MAP_VALUE   4               // 手持端按键到选项名索引的映射关系
#define DEFAULT_OPTION_NUM 5        // 默认的投票的选项数目
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

typedef struct _VoteData
{
    QString name;       // 投票器持有者姓名
    quint16 id;         // id
    quint8 key;         // 按键选项
}VoteData;

#if 0
enum VoteMode
{
    SINGLE_VOTE,
    MULTIPLE_VOTE,
    RACE_VOTE
};

typedef struct _RevData
{
    quint16 id;   // 手持端ID
    quint8 key;   // 手持端按键的编号
    enum ReceivedDataType type;
}RevData;
#endif

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
    void RegistGlobalShortCut();        // 注册全局快捷键

signals:
    void displayResult(bool flag);  // 信号--显示结果
    void clearDrawData();           // 信号--清空数据
    void setQuestionNum(int num);   // 信号--设置问题编号
    void setLastTime(int sec);      // 信号--设置持续时间
    void setOptionNum(int num);     // 信号--给histgram设置选项数目
    void updateGraph(int index);

public slots:
    void VoteStart();
    void VotePause();
    void VoteStop();
    void VoteAuto();
    void StartNewSession();                 // 开始新的会话
    void SavePreviousSession();             // 每次停止投票的时候，保存上次一会话（投票结果）
    void DoShowPreviousQuestion();          // 将上一次记录的数据再次发送给drawer显示
    void DoShowNextQuestion();              // 将下一次记录的数据再次发送给drawer显示
    void DoShowOptions();
    void DoShowResults();
    void DoShowStatics();
    void HandlerControlMessage(quint16 id, quint8 option);  // 从控制端发来的控制消息
    void ParseData(quint16 id, quint8 option);
    void StartOrStop();                     // 由全局快捷键触发，改变投票的状态
    void ShowOrHideControler();             // 由全局快捷键触发，将主控制窗口从一个屏幕移动到另一个上面
    void ShowOrHideResult();                // 由全局快捷键触发，将显示结果的窗口从一个屏幕移动到另一个上面

private slots:
    void DoHideStopWatch();                 // 删除秒表widget，在上拉动画完毕后调用
    void GetIDList();                       // 发送获取ID列表消息
    void GetIDListLength();                 // 获取手持端设备的个数

#if 0
    void DoRaceVoteMode();                  // 抢答模式下的处理
    void ShowRaceVoteWinner(quint16 id, quint8 key);  // 抢答模式时显示抢到者的ID
    void CloseRaceVoteWinner();             // 关闭抢答窗口时要关闭设备
    void DoSingleMode();                    // 单选模式下
    void DoMutipleMode();                   // 多选模式下
#endif

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    Ui::DianVoteControl *ui;
    DianVoteOption *options;
    DianVoteDrawer *drawer;
    HidControl *hidControl;
    StopWatch *stopWatch;
    QDesktopWidget* desktopWidget;
    QSplashScreen *splash;                          // 欢迎界面
    QIcon *windowIcon;                              // windowIcon
    QPoint dragPosition;                            // 拖动位置
    QPoint prePoint;                                // 记录动画开始时的位置
    QSize initSize;                                 // 窗口初始化时的大小
    enum ControlState curState;                     // 当前状态
    QList< VoteData* > *voted;                      // 已投票的记录
    int resultIndex;                                // 投票记录索引
    QList< int > optionNumList;                     // 记录所有问题的选项个数
    QList< QList < VoteData* >* > *resultList;      // 所有投票结果的记录
    static QFile *VoteLog;                          // 日志文件
    int previousOptionNum;                          // 上一次使用的选项个数
    int previousLastTime;                           // 上一次使用的持续时间
    bool straightStart;                             // 直接开始，不设置选项个数

    // mode menu
#if 0
    QMenu *qmMode;
    QAction *qaSingleMode;                  // 单选模式
    QAction *qaMutipleMode;                 // 多选
    QAction *qaRaceMode;                    // 抢答
    enum VoteMode voteMode;                 // 投票模式，默认是SINGLE_VOTE
    QDialog *raceWinner;                    // 显示抢答结果
#endif

    // buttons
    QPushButton *pbStart;
    QPushButton *pbAuto;
    QPushButton *pbResult;
    QPushButton *pbPause;
    QPushButton *pbStop;
    QPushButton *pbOption;
    QPushButton *pbClose;
    QInputDialog *getOptionNum;         // 弹出窗口，设置选项个数
    QInputDialog *getLastTime;          // 当用Auto模式时需要设置
    QPropertyAnimation *resizeAnimation;

    // global shortcut
    QxtGlobalShortcut *startOrStop;         // start or stop
    QxtGlobalShortcut *showOrHideControler; // move controler from one screen to another
    QxtGlobalShortcut *showOrHideResult;    // move result from one screen to another
    QxtGlobalShortcut *previousSlide;       // show previous powerpoint
    QxtGlobalShortcut *nextSlide;           // show next power point

private:
    int GetOptionNum();                 // 获取选项个数，并传给histgram
    int GetLastTime();                  // 获取秒表倒计时时间
    bool StartHid();                    // 开启接收设备
    bool StopHid();                     // 关闭接收设备
    bool PrepareHid();                  // 准备好接收设备
    void LoadStyleSheet(const QString &sheetname);
    void ShowStopWatch();               // 显示秒表
    void HideStopWatch();               // 删掉秒表
    void ClearLogList();                // 清空log链表，在此之前保存到log文件中
};

#endif // DIANVOTECONTROL_H
