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

#define DEFAULT_DOCK_SPACE 20       // ��������ʱ���Ӧ����
                                    // ���������������ܵľ���С�ڻ��ߵ����������ʱ
                                    // ���ھ�����������߿���
#define MAP_VALUE   4               // �ֳֶ˰�����ѡ����������ӳ���ϵ
#define DEFAULT_OPTION_NUM 4        // Ĭ�ϵ�ͶƱ��ѡ����Ŀ
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

typedef struct _RevData
{
    quint32 id;   // �ֳֶ�ID
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
    void ParseData(quint32 id, quint8 option);
    //-----data pre-process part-----//

private slots:
//    void DoShowStopWatch();     // ��ʾ���widget��������������Ϻ����
    void DoHideStopWatch();     // ɾ�����widget��������������Ϻ����

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

    static QFile *VoteLog;             // ��־�ļ�

    // buttons
    QPushButton *pbStart;
    QPushButton *pbAuto;
    QPushButton *pbResult;
    QPushButton *pbPause;
    QPushButton *pbStop;
    QPushButton *pbOption;
    QPushButton *pbClose;

    QInputDialog *getOptionNum;      // �������ڣ�����ѡ�����
    QInputDialog *getLastTime;      // ����Autoģʽʱ��Ҫ����

    QPoint dragPosition;

    QPropertyAnimation *resizeAnimation;
//    QPropertyAnimation *showStopWatchAnimation;

    // ���ؽ���λ�ü���С
    QPoint prePoint;        // ��¼������ʼʱ��λ��
    QSize initSize;         // ���ڳ�ʼ��ʱ�Ĵ�С
    // ����״̬
    enum ControlState curState; // ��ǰ״̬

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
