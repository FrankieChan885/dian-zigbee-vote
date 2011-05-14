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

    void draw(QPainter *painter);           // 画图

protected:
    void paintEvent(QPaintEvent *event);    // 覆盖paintEvent

public:

    Ui::DianVoteDrawer *ui;
    DrawBase *histgram;
    DrawBase *pie;

private:

    void DoWithCoordinate();    // 计算坐标

private:
    QTimer *timer;

    //---- font ----//
    int FontSizeRatio;
    //---- font ----//
    //----config values----//
    int QuestionNameX;      // 问题名字的X坐标
    float RatioQuestionNameX; // 问题名字位置与窗口的宽度比例
    int QuestionNameY;      // 问题名字的Y坐标
    float RatioQuestionNameY; // 问题名字位置与窗口的高度比例

    int TotalNumX;          // 总数显示的X坐标
    float RatioTotalNumX;   // 总数位置与窗口的宽度比例
    int TotalNumY;          // 总数显示的Y坐标
    float RatioTotalNumY;   // 总数位置与窗口的高度度比例
    //----config values----//
};

#endif // DIANVOTEDRAWER_H
