#ifndef DIANVOTECONTROL_H
#define DIANVOTECONTROL_H

#include <QWidget>
#include <QByteArray>
#include "dianvotedrawer.h"
#include "hiddevice.h"

class QPushButton;

#define DEVICE_ID_LENGTH 4 + 1      // �ֳ��豸ID�ų���
#define MAP_VALUE   4               // �ֳֶ˰�����ѡ����������ӳ���ϵ

typedef struct _RevData
{
    char devID[DEVICE_ID_LENGTH];   // �ֳֶ�ID
    char key;   // �ֳֶ˰����ı��
    QString *revTime;
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

signals:
    void updateGraph(int index);

public slots:

    //-----control part-----//
    void VoteStart();
    void VotePause();
    void VoteStop();
    void VoteAuto();
    void SetStopWatch(int time = 60);
    //-----control part-----//

    //-----display part-----//
    void DoShowResults();
    void DoShowStatics();
    //-----display part-----//

    //-----data pre-process part-----//
    void ParseData(QByteArray data);
    //-----data pre-process part-----//

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    Ui::DianVoteControl *ui;
    DianVoteDrawer *drawer;
    QHidDevice *device;
    QList< RevData* > *log;     // ���ݽ��յ�log

    QPushButton *pbStart;
    QPushButton *pbAuto;
    QPushButton *pbResult;
    QPushButton *pbPause;
    QPushButton *pbStop;
    QPushButton *pbOption;
    QPushButton *pbClose;

    QPoint dragPosition;

private:
    void LoadStyleSheet(const QString &sheetname);
};

#endif // DIANVOTECONTROL_H
