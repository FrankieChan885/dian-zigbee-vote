#ifndef DIANVOTEDRAWER_H
#define DIANVOTEDRAWER_H

#include <QWidget>
#include "dianvotedebug.h"

#define DEFAULT_UPDATE_TIMER 200  // million second

class DrawBase;
class DrawPie;
class DrawHistgram;
class QTimer;
class QPainter;
class QToolButton;

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
    void SetRepliedVoters(uint num);    // ���õ�����Ļ�ȡ���豸����
    void SwithChart();      // �ڱ�ͼ��ֱ��ͼ֮���л�

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

    //----buttons----//
    QToolButton *previous;
    QToolButton *next;
    QToolButton *showPie;
    QToolButton *showHistgram;
    QToolButton *correctAnswer;
    QToolButton *option;
    QToolButton *close;
    //----buttons----//
};

#endif // DIANVOTEDRAWER_H
