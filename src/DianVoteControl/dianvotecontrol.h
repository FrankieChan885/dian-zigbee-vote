#ifndef DIANVOTECONTROL_H
#define DIANVOTECONTROL_H

#include <QWidget>
#include <QByteArray>
#include "dianvotedrawer.h"

class HidControl;
class StopWatch;
class QSize;
class QFile;
class QMenu;
class QAction;
class QDialog;
class QInputDialog;
class QSpacerItem;
class QErrorMessage;
class QSequentialAnimationGroup;
class QPropertyAnimation;
class QPushButton;
class QSplashScreen;

#define DEFAULT_DOCK_SPACE 20       // ��������ʱ���Ӧ����
                                    // ���������������ܵľ���С�ڻ��ߵ����������ʱ
                                    // ���ھ�����������߿���
#define MAP_VALUE   4               // �ֳֶ˰�����ѡ����������ӳ���ϵ
#define DEFAULT_OPTION_NUM 5        // Ĭ�ϵ�ͶƱ��ѡ����Ŀ
#define MAXIMUN_OPTION_NUM 5        // ͶƱ��Ŀѡ������ֵ
#define DEFAULT_DEADLINE 60         // �����ʱʱ��Ĭ��ֵ
#define MAXIMUN_DEADLINE 600        // �����ʱʱ�����ֵ

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

#ifndef USELESS_CODE
enum VoteMode
{
    SINGLE_VOTE,
    MULTIPLE_VOTE,
    RACE_VOTE
};
#endif

typedef struct _RevData
{
    quint16 id;   // �ֳֶ�ID
    quint8 key;   // �ֳֶ˰����ı��
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
    void displayResult(bool flag);  // �ź�--��ʾ���
    void clearDrawData();           // �ź�--�������
    void setLastTime(int sec);      // �ź�--���ó���ʱ��
    void setOptionNum(int num);     // �ź�--��histgram����ѡ����Ŀ
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
    void ParseData(quint16 id, quint8 option);
    //-----data pre-process part-----//

private slots:
//    void DoShowStopWatch();     // ��ʾ���widget��������������Ϻ����
    void DoHideStopWatch();     // ɾ�����widget��������������Ϻ����
    void GetIDList();           // ���ͻ�ȡID�б���Ϣ
    void GetIDListLength();     // ��ȡ�ֳֶ��豸�ĸ���

#ifndef USELESS_CODE
    void DoRaceVoteMode();      // ����ģʽ�µĴ���
    void ShowRaceVoteWinner(quint16 id, quint8 key);  // ����ģʽʱ��ʾ�����ߵ�ID
    void CloseRaceVoteWinner();             // �ر����𴰿�ʱҪ�ر��豸
    void DoSingleMode();        // ��ѡģʽ��
    void DoMutipleMode();       // ��ѡģʽ��
#endif

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    QList< RevData* > *log;     // ���ݽ��յ�log

    Ui::DianVoteControl *ui;
    DianVoteDrawer *drawer;
    HidControl *hidControl;
    StopWatch *stopWatch;
    QSplashScreen *splash;      // ��ӭ����
    QDialog *raceWinner;        // ��ʾ������
    QIcon *windowIcon;          // windowIcon

    // mode menu
//    QMenu *qmMode;
//    QAction *qaSingleMode;  // ��ѡģʽ
//    QAction *qaMutipleMode; // ��ѡ
//    QAction *qaRaceMode;    // ����

    // buttons
    QPushButton *pbStart;
    QPushButton *pbAuto;
    QPushButton *pbResult;
    QPushButton *pbPause;
    QPushButton *pbStop;
    QToolButton *pbOption;
    QPushButton *pbClose;

    QInputDialog *getOptionNum;      // �������ڣ�����ѡ�����
    QInputDialog *getLastTime;      // ����Autoģʽʱ��Ҫ����

    QPropertyAnimation *resizeAnimation;
//    QPropertyAnimation *showStopWatchAnimation;

    // ���ؽ���λ�ü���С
    QPoint dragPosition;    // �϶�λ��
    QPoint prePoint;        // ��¼������ʼʱ��λ��
    QSize initSize;         // ���ڳ�ʼ��ʱ�Ĵ�С
    // ����״̬
//    enum VoteMode voteMode;     // ͶƱģʽ��Ĭ����SINGLE_VOTE
    enum ControlState curState; // ��ǰ״̬
    static QFile *VoteLog;             // ��־�ļ�

private:
    int GetOptionNum();    // ��ȡѡ�������������histgram
    int GetLastTime();      // ��ȡ�����ʱʱ��
    bool StartHid();        // ���������豸
    bool StopHid();         // �رս����豸
    bool PrepareHid();     // ׼���ý����豸
    void LoadStyleSheet(const QString &sheetname);
    void ShowStopWatch();   // ��ʾ���
    void HideStopWatch();   // ɾ�����
    void WriteRevDataLog(RevData *rd);  // ��ÿһ�ν��յ�����ʱ���������ͶƱ�����м�¼д���ļ��������log�б�
    void ClearLogList();        // ���log�����ڴ�֮ǰ���浽log�ļ���
};

#endif // DIANVOTECONTROL_H
