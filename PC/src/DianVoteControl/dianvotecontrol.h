#ifndef DIANVOTECONTROL_H
#define DIANVOTECONTROL_H

#include <QWidget>
#include <QByteArray>
#include "dianvotedrawer.h"
#include "hiddevice.h"

class QPushButton;

#define DEVICE_ID_LENGTH 4 + 1      // 手持设备ID号长度
#define MAP_VALUE   4               // 手持端按键到选项名索引的映射关系

typedef struct _RevData
{
    char devID[DEVICE_ID_LENGTH];   // 手持端ID
    char key;   // 手持端按键的编号
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
    QList< RevData* > *log;     // 数据接收的log

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
