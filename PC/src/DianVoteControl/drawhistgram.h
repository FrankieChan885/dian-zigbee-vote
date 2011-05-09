#ifndef DRAWHISTGRAM_H
#define DRAWHISTGRAM_H

#include "drawbase.h"
#include <QPainter>

#define BASIC_SLICE 20
#define FONT_RATIO 70       // 用于动态计算字体大小
#define FONT_SPACE 45       // 用于动态计算字体离坐标轴的距离

class DrawHistgram : public DrawBase
{
    Q_OBJECT

public:
    DrawHistgram();
    ~DrawHistgram();

    void draw(QPainter *painter);           // 画图

protected:
    void paintEvent(QPaintEvent *event);    // 重载paintEvent

private:
    void DoWithCoodinate();     // 计算画图时需要的坐标

    bool YAxisDashLineFlag;

    //-----Important Coordinate-----//
    QPoint zeroPoint;       // 画图使用的0点坐标
    int XaxisLength;        // X轴的长度
    int YaxisLength;        // Y轴的长度
    //-----Important Coordinate-----//

    //-----Config Values-----//
    int WidthSlices;        // 将窗口宽分成20份
    int HeightSlices;       // 将窗口高分成20份
    float XaixsLengthRatio; // X坐标占窗口的比例
    float YaxisLengthRatio; // Y坐标占窗口的比例

    int ScaleSize;          // X和Y轴上刻度的宽带或者高度
    int YFontSpace;        // 字离坐Y标轴的距离
    int YFontSpaceRatio;    // 字离Y坐标的距离的比例
    int XFontSpace;        // 字离坐X标轴的距离
    int XFontSpaceRatio;    // 字离X坐标的距离的比例
    int YaxisFontRatio;
    int YFontSize;           // Y轴默认字体大小
    int XFontSize;          // X轴默认字体大小
    int XaxisFontRatio;
    int YaxisSlices;        // 以10为度量画Y轴上的刻度,当投票人数少于等于10的时候改为1

    int YaxisScaleNums;     // Y坐标刻度的数目
    int YaxisScaleSpace;    // Y坐标刻度间隔
    int XaxisScaleNums;      // X坐标刻度的数目
    int XaxisScaleSpace;    // X坐标刻度间隔

    int Precision;          // 显示比例时的精确度
    float RectWidthRatio;        // 决定柱状条的宽度
    float RectHeigthRatio;      // 决定柱状图的高度与纵轴的比例
    int RatioRectSize;      // 用于显示比例的矩形大小
    //-----Config Values-----//

    //-----for font style-----//
    QFont YFont;             // Y坐标上数字的样式
    QFont XFont;            // X坐标及柱状图上的字的样式
    //-----for font style-----//
};

#endif // DRAWHISTGRAM_H
