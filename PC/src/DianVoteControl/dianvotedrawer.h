#ifndef DIANVOTEDRAWER_H
#define DIANVOTEDRAWER_H

#include <QWidget>
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

    void draw(QPainter *painter);           // ��ͼ

protected:
    void paintEvent(QPaintEvent *event);    // ����paintEvent

public:

    Ui::DianVoteDrawer *ui;
    DrawBase *histgram;
    DrawBase *pie;

private:

    void DoWithCoordinate();    // ��������

private:
    QTimer *timer;

    //---- font ----//
    int FontSizeRatio;
    //---- font ----//
    //----config values----//
    int QuestionNameX;      // �������ֵ�X����
    float RatioQuestionNameX; // ��������λ���봰�ڵĿ�ȱ���
    int QuestionNameY;      // �������ֵ�Y����
    float RatioQuestionNameY; // ��������λ���봰�ڵĸ߶ȱ���

    int TotalNumX;          // ������ʾ��X����
    float RatioTotalNumX;   // ����λ���봰�ڵĿ�ȱ���
    int TotalNumY;          // ������ʾ��Y����
    float RatioTotalNumY;   // ����λ���봰�ڵĸ߶ȶȱ���
    //----config values----//
};

#endif // DIANVOTEDRAWER_H
