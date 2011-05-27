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

signals:
    void ShowPreviousQuestion();
    void ShowNextQuestion();

public slots:
    void DoShowPreviousQuestion();      // ��ʾ��һ������
    void DoShowNextQuestion();          // ��ʾ��һ������
    void SetNextPreviousDisabled();     // ����ͶƱ�Ĺ����н�ֹ�л�
    void SetNextPreviousEnabled();      // ����ͶƱ�����лָ��л���Ŀ
    void SetQuestionNum(int num);       // ������Ŀ���
    void SetRepliedVoters(uint num);    // ���õ�����Ļ�ȡ���豸����
    void SwithChart();      // �ڱ�ͼ��ֱ��ͼ֮���л�

protected:
    void paintEvent(QPaintEvent *event);    // ����paintEvent

private:
    void DoWithCoordinate();    // ��������

public:
    Ui::DianVoteDrawer *ui;
    DrawBase *histgram;
    DrawBase *pie;

private:
    QTimer *timer;

    int questionNum;            // ��Ŀ���
    int replidDeviceNum;        // �����󷵻ص��豸����

    //---- font ----//
    int FontSizeRatio;
    //---- font ----//

    //----config values----//
    int RepliedDeviceNumX;      // �������ֵ�X����
    float RatioRepliedDeviceX; // ��������λ���봰�ڵĿ�ȱ���
    int RepliedDeviceNumY;      // �������ֵ�Y����
    float RatioRepliedDeviceY; // ��������λ���봰�ڵĸ߶ȱ���

    int QuestionNumX;       // ��Ŀ�����ʾ��X����
    float RatioQuestionNumX;  // ��Ŀ�����ʾ�봰�ڿ�ȵı���
    int QuestionNumY;       // ��Ŀ�����ʾ��Y����
    float RatioQuestionNumY;  // ��Ŀ�����ʾ�봰�ڸ߶ȵı���


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
    QToolButton *details;
    QToolButton *close;
    //----buttons----//
};

#endif // DIANVOTEDRAWER_H
