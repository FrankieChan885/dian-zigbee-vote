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
    void SetRepliedDeviceNum(uint replyNum); // 设置设备总数
#endif  // end ifdef

protected:
    void paintEvent(QPaintEvent *event);    // 覆盖paintEvent

private:

    void DoWithCoordinate();    // 计算坐标

private:
    QTimer *timer;

    int replidDeviceNum;        // 点名后返回的设备总数

    //---- font ----//
    int FontSizeRatio;
    //---- font ----//

    //----config values----//
    int RepliedDeviceNumX;      // 问题名字的X坐标
    float RatioRepliedDeviceX; // 问题名字位置与窗口的宽度比例
    int RepliedDeviceNumY;      // 问题名字的Y坐标
    float RatioRepliedDeviceY; // 问题名字位置与窗口的高度比例

    int TotalNumX;          // 总数显示的X坐标
    float RatioTotalNumX;   // 总数位置与窗口的宽度比例
    int TotalNumY;          // 总数显示的Y坐标
    float RatioTotalNumY;   // 总数位置与窗口的高度度比例
    //----config values----//
};

#endif // DIANVOTEDRAWER_H
