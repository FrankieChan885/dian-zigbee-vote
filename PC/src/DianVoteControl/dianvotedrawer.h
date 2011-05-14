#ifndef DIANVOTEDRAWER_H
#define DIANVOTEDRAWER_H

#include <QWidget>
#include "dianvotedebug.h"
#include "drawhistgram.h"

#define DEFAULT_UPDATE_TIMER 200  // million second

class QTimer;
class QPainter;

namespace Ui {
    class DianVoteDrawer;
}

class DianVoteDrawer : public QWidget
{

    Q_OBJECT

public:
    DianVoteDrawer(QWidget *parent = 0);
    ~DianVoteDrawer();

    void draw(QPainter *painter);           // draw

public:
    Ui::DianVoteDrawer *ui;
    DrawBase *histgram;
    DrawBase *pie;

public slots:
#ifdef DO_ROLL_CALL
    void SetRepliedDeviceNum(uint replyNum); // �����豸����
#endif  // end ifdef

protected:
    void paintEvent(QPaintEvent *event);    // ����paintEvent

private:

    void DoWithCoordinate();    // ��������

private:
    QTimer *timer;

    int replidDeviceNum;        // �����󷵻ص��豸����

    //---- font ----//
    int FontSizeRatio;
    //---- font ----//

    //----config values----//
    int RepliedDeviceNumX;      // �������ֵ�X����
    float RatioRepliedDeviceX; // ��������λ���봰�ڵĿ�ȱ���
    int RepliedDeviceNumY;      // �������ֵ�Y����
    float RatioRepliedDeviceY; // ��������λ���봰�ڵĸ߶ȱ���

    int TotalNumX;          // ������ʾ��X����
    float RatioTotalNumX;   // ����λ���봰�ڵĿ�ȱ���
    int TotalNumY;          // ������ʾ��Y����
    float RatioTotalNumY;   // ����λ���봰�ڵĸ߶ȶȱ���
    //----config values----//
};

#endif // DIANVOTEDRAWER_H
