#ifndef DRAWPIE_H
#define DRAWPIE_H

#include "drawbase.h"

#define DEFAULT_START_ANGLE 60
#define PI 3.141592653589793

class DrawPie : public DrawBase
{
public:
    DrawPie();
    ~DrawPie();

    void draw(QPainter *painter);           // 画图

protected:
    void paintEvent(QPaintEvent *event);    // 覆盖paintEvent

private:
    void DoWithCoodinate();     // 计算画图时需要的坐标

private:
    //---- configure values----//
    int FontSize;           // 字体大小
    int RatioText;          // 字体大小与饼图的比例
    int CoverTextSpace;     // 相邻切投票数目为0的情况，字的距离
    float RatioLeftTextSpace; // 左边字离饼的距离与半径的比例
    float RatioRightTextSpace; // 右边字离饼的距离与半径的比例
    int Precision;          // 显示比例时的精确度
    int CentreOfPieX;       // 圆心X坐标
    int CentreOfPieY;       // 圆心Y坐标
    int PieRadius;          // 园半径
    float RatioPieRadius;   // 圆半径与窗口大小的比例
    QRect *DrawRegion;        // 扇形只能绘制在这一个矩形区域
    //---- configure values----//
};

#endif // DRAWPIE_H
