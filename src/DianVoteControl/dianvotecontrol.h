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

typedef struct _VoteData
{
    QString name;       // ͶƱ������������
    quint16 id;         // id
    quint8 key;         // ����ѡ��
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
    quint16 id;   // �ֳֶ�ID
    quint8 key;   // �ֳֶ˰����ı��
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
    void RegistGlobalShortCut();        // ע��ȫ�ֿ�ݼ�

signals:
    void displayResult(bool flag);  // �ź�--��ʾ���
    void clearDrawData();           // �ź�--�������
    void setQuestionNum(int num);   // �ź�--����������
    void setLastTime(int sec);      // �ź�--���ó���ʱ��
    void setOptionNum(int num);     // �ź�--��histgram����ѡ����Ŀ
    void updateGraph(int index);

public slots:
    void VoteStart();
    void VotePause();
    void VoteStop();
    void VoteAuto();
    void StartNewSession();                 // ��ʼ�µĻỰ
    void SavePreviousSession();             // ÿ��ֹͣͶƱ��ʱ�򣬱����ϴ�һ�Ự��ͶƱ�����
    void DoShowPreviousQuestion();          // ����һ�μ�¼�������ٴη��͸�drawer��ʾ
    void DoShowNextQuestion();              // ����һ�μ�¼�������ٴη��͸�drawer��ʾ
    void DoShowOptions();
    void DoShowResults();
    void DoShowStatics();
    void HandlerControlMessage(quint16 id, quint8 option);  // �ӿ��ƶ˷����Ŀ�����Ϣ
    void ParseData(quint16 id, quint8 option);
    void StartOrStop();                     // ��ȫ�ֿ�ݼ��������ı�ͶƱ��״̬
    void ShowOrHideControler();             // ��ȫ�ֿ�ݼ��������������ƴ��ڴ�һ����Ļ�ƶ�����һ������
    void ShowOrHideResult();                // ��ȫ�ֿ�ݼ�����������ʾ����Ĵ��ڴ�һ����Ļ�ƶ�����һ������

private slots:
    void DoHideStopWatch();                 // ɾ�����widget��������������Ϻ����
    void GetIDList();                       // ���ͻ�ȡID�б���Ϣ
    void GetIDListLength();                 // ��ȡ�ֳֶ��豸�ĸ���

#if 0
    void DoRaceVoteMode();                  // ����ģʽ�µĴ���
    void ShowRaceVoteWinner(quint16 id, quint8 key);  // ����ģʽʱ��ʾ�����ߵ�ID
    void CloseRaceVoteWinner();             // �ر����𴰿�ʱҪ�ر��豸
    void DoSingleMode();                    // ��ѡģʽ��
    void DoMutipleMode();                   // ��ѡģʽ��
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
    QSplashScreen *splash;                          // ��ӭ����
    QIcon *windowIcon;                              // windowIcon
    QPoint dragPosition;                            // �϶�λ��
    QPoint prePoint;                                // ��¼������ʼʱ��λ��
    QSize initSize;                                 // ���ڳ�ʼ��ʱ�Ĵ�С
    enum ControlState curState;                     // ��ǰ״̬
    QList< VoteData* > *voted;                      // ��ͶƱ�ļ�¼
    int resultIndex;                                // ͶƱ��¼����
    QList< int > optionNumList;                     // ��¼���������ѡ�����
    QList< QList < VoteData* >* > *resultList;      // ����ͶƱ����ļ�¼
    static QFile *VoteLog;                          // ��־�ļ�
    int previousOptionNum;                          // ��һ��ʹ�õ�ѡ�����
    int previousLastTime;                           // ��һ��ʹ�õĳ���ʱ��
    bool straightStart;                             // ֱ�ӿ�ʼ��������ѡ�����

    // mode menu
#if 0
    QMenu *qmMode;
    QAction *qaSingleMode;                  // ��ѡģʽ
    QAction *qaMutipleMode;                 // ��ѡ
    QAction *qaRaceMode;                    // ����
    enum VoteMode voteMode;                 // ͶƱģʽ��Ĭ����SINGLE_VOTE
    QDialog *raceWinner;                    // ��ʾ������
#endif

    // buttons
    QPushButton *pbStart;
    QPushButton *pbAuto;
    QPushButton *pbResult;
    QPushButton *pbPause;
    QPushButton *pbStop;
    QPushButton *pbOption;
    QPushButton *pbClose;
    QInputDialog *getOptionNum;         // �������ڣ�����ѡ�����
    QInputDialog *getLastTime;          // ����Autoģʽʱ��Ҫ����
    QPropertyAnimation *resizeAnimation;

    // global shortcut
    QxtGlobalShortcut *startOrStop;         // start or stop
    QxtGlobalShortcut *showOrHideControler; // move controler from one screen to another
    QxtGlobalShortcut *showOrHideResult;    // move result from one screen to another
    QxtGlobalShortcut *previousSlide;       // show previous powerpoint
    QxtGlobalShortcut *nextSlide;           // show next power point

private:
    int GetOptionNum();                 // ��ȡѡ�������������histgram
    int GetLastTime();                  // ��ȡ�����ʱʱ��
    bool StartHid();                    // ���������豸
    bool StopHid();                     // �رս����豸
    bool PrepareHid();                  // ׼���ý����豸
    void LoadStyleSheet(const QString &sheetname);
    void ShowStopWatch();               // ��ʾ���
    void HideStopWatch();               // ɾ�����
    void ClearLogList();                // ���log�����ڴ�֮ǰ���浽log�ļ���
};

#endif // DIANVOTECONTROL_H
