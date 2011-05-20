#include "drawhistgram.h"
#include <math.h>

DrawHistgram::DrawHistgram()
{
    YAxisDashLineFlag = true;  // 默认不显示虚线

    WidthSlices = 18;      // 将窗口宽分成20份
    HeightSlices = 20;     // 将窗口高分成20份
    XaixsLengthRatio = 16 / (float)WidthSlices;      // X坐标占窗口的比例
    YaxisLengthRatio = 16 / (float)HeightSlices;     // Y坐标占窗口的比例

    ScaleSize = 5;         // X和Y轴上刻度的宽带或者高度
    YFontSpace = 25;     // 字离坐Y标轴的距离
    YFontSpaceRatio = 15;    //
    XFontSpace = 25;     // 字离坐X标轴的距离
    XFontSpaceRatio = 30;
    YaxisFontRatio = 30;    // 字体大小与纵轴的比例
    YFontSize = 10;        // 默认字体大小
    XaxisFontRatio = 50;    // 字体大小与横轴的比例
    XFontSize = 15;         // 默认字体大小
    YaxisSlices = 10;     // 以10为度量画Y轴上的刻度,当投票人数少于等于10的时候改为1

    Precision = 4;         // 显示比例时的精确度
    RectWidthRatio = 0.5;    // 决定柱状条的宽度
    RectHeigthRatio = 0.8;      // 决定柱状条的高度
    RatioRectSize = 10;    // 用于显示比例的矩形大小

    // 初始化虚拟投票总人数
    fakeVoterNums = INIT_VOTER_NUM;
}

DrawHistgram::~DrawHistgram()
{
}

void DrawHistgram::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 设置视口为整个窗口大小
    painter.setViewport(0, 0, width(), height());
    // 设置可以绘图的大小
    painter.setWindow(0, 0, width(), height());

    draw(&painter);
}

void DrawHistgram::draw(QPainter *painter)
{
    if(voterNums == 0)
    {
        return;
    }

    //-----Do Calculate the Coodinate-----//
    DoWithCoodinate();

    QPen thinPen(palette().foreground(), 2);
    painter->setPen(thinPen);
    // 画出Y轴
//    painter->drawLine(zeroPoint.x(), zeroPoint.y(),
//                      zeroPoint.x(), zeroPoint.y() - YaxisLength);
                                                // 0_0 默认0点坐标在左上角...
    // 画出X轴
    painter->drawLine(zeroPoint.x(), zeroPoint.y(),
                          zeroPoint.x() + XaxisLength, zeroPoint.y());

    // 画Y轴刻度
    /*for (int i = 1; i <= YaxisScaleNums; i++)
    {
        painter->setPen(thinPen);
        painter->drawLine(zeroPoint.x() - ScaleSize, zeroPoint.y() - i * YaxisScaleSpace,
                          zeroPoint.x(), zeroPoint.y() - i * YaxisScaleSpace);
                                            // 是“-”...原坐标0点在左上角问题...

        painter->setFont(YFont);
        QString num = QString("%1").arg(i * BASIC_SLICE);
        painter->drawText(zeroPoint.x() - YFontSpace, zeroPoint.y() - i * YaxisScaleSpace, num);
        // 如果纵轴虚线被设置成显示状态
        if (YAxisDashLineFlag)
        {
            painter->setPen(QPen(Qt::black, 0.1, Qt::DashLine, Qt::SquareCap));
            painter->drawLine(zeroPoint.x(), zeroPoint.y() - i * YaxisScaleSpace,
                              zeroPoint.x() + XaxisLength, zeroPoint.y() - i * YaxisScaleSpace);
        }
    }*/

    // 画出柱状图和X轴

    painter->drawLine(zeroPoint.x(), zeroPoint.y(),
                      zeroPoint.x(), zeroPoint.y() - ScaleSize);

    for (int i = 1; i <= optionNums; i++)
    {
        // 画出X轴刻度
        painter->setPen(thinPen);
        painter->setFont(XFont);
        painter->drawLine(zeroPoint.x() + i * XaxisScaleSpace, zeroPoint.y(),
                          zeroPoint.x() + i * XaxisScaleSpace, zeroPoint.y() - ScaleSize);
        // 画出选项
        painter->setFont(XFont);
        painter->drawText(zeroPoint.x() + i * XaxisScaleSpace -(XaxisScaleSpace + XFontSize * drawData->at(i - 1)->optionName.length()) /2, // 文字的位置在每两个刻度的中间
                          zeroPoint.y() + XFontSpace,
                          drawData->at(i - 1)->optionName);

        // 画出直方图
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setPen(thinPen);
        painter->setBrush(QBrush((*drawData)[i - 1]->color, Qt::SolidPattern));   // 设置填充颜色
        // 如果被设置成显示正确答案
        if (correctAnswerFlag)
        {
            // 如果是正确答案，设置成绿色，否则为红色
            if (drawData->at(i - 1)->optionName == correctAnswer)
            {
                painter->setBrush(QBrush("green", Qt::SolidPattern));
            }
            else
            {
                painter->setBrush(QBrush("red", Qt::SolidPattern));
            }
        }
        painter->drawRect((*drawData)[i - 1]->histgramX, (*drawData)[i - 1]->histgramY,
                          (*drawData)[i - 1]->histgramWidth, (*drawData)[i - 1]->histgramHeight); // 画柱状图

        if (displayResultWhileVoting)
        {
            // 显示投票人的个数
            int numX = (*drawData)[i - 1]->histgramX +
                        ((*drawData)[i - 1]->histgramWidth - XFontSize * ((*drawData)[i - 1]->nums.length())) / 2;
            int numY = (*drawData)[i - 1]->histgramY - 2 * XFontSize;
            painter->drawText(numX, numY, (*drawData)[i - 1]->nums);

            // 显示投票比例
            int ratioX = (*drawData)[i - 1]->histgramX +
                        ((*drawData)[i - 1]->histgramWidth - XFontSize * ((*drawData)[i - 1]->ratio.length() - 1)) / 2;
            int ratioY = (*drawData)[i - 1]->histgramY - XFontSize / 2;
            painter->drawText(ratioX, ratioY, (*drawData)[i - 1]->ratio);
        }
    }
}

void DrawHistgram::DoWithCoodinate()
{

    zeroPoint = QPoint( width() / WidthSlices,
                           (HeightSlices - 2) * height() / HeightSlices);
    XaxisLength = XaixsLengthRatio * width();
    YaxisLength = YaxisLengthRatio * height();

    int maxOptionVoteNum = 0;       // 投票人数最多的那个选项的个数，用于决定Y轴刻度的长度

    for (int i = 0; i < optionNums; i++)
    {
        maxOptionVoteNum = drawData->at(i)->voterNum > maxOptionVoteNum ? \
                           drawData->at(i)->voterNum : maxOptionVoteNum;
    }
    // 更改增长模式，先固定使用默认的voterNum，然后当投票人数的最大值超过voterNum的时候
    // voterNum自动增加到原来的2倍

    if(maxOptionVoteNum > fakeVoterNums)
    {
        fakeVoterNums *= 2;
    }

    /*YaxisScaleNums = ceil((double)fakefakeVoterNums / BASIC_SLICE);     // 刻度数目
    YaxisScaleSpace =  RectHeigthRatio * YaxisLength / YaxisScaleNums;                    // 刻度间隔
    YaxisScaleNums += 2;*/

    XaxisScaleNums = optionNums;        // 刻度数=选项数
    XaxisScaleSpace = XaxisLength / XaxisScaleNums;     // 刻度间隔

    for (int i = 0; i < optionNums; i++)
    {
        (*drawData)[i]->histgramWidth = RectWidthRatio * XaxisScaleSpace;     // 柱状条宽度
        (*drawData)[i]->histgramHeight = (double)YaxisLength * (double)(*drawData)[i]->voterNum / (double)fakeVoterNums;   // 柱状图高度
        (*drawData)[i]->histgramX = zeroPoint.x() + (i + 1) * XaxisScaleSpace -
                                   (XaxisScaleSpace + (*drawData)[i]->histgramWidth) / 2;    // 柱状条左上角X坐标
        (*drawData)[i]->histgramY = zeroPoint.y() - (*drawData)[i]->histgramHeight;       // 柱状条左上角Y坐标
        if (displayResultWhileVoting)
        {
            (*drawData)[i]->nums = QString("%1").arg((*drawData)[i]->voterNum);       // 个数，画出来

            if(!voterNums)
            {
                (*drawData)[i]->ratio = QString("0");
            }
            else
            {
                (*drawData)[i]->ratio = QString("%1").arg((float)(*drawData)[i]->voterNum / voterNums * 100);       // 算出比例
            }

            if ((*drawData)[i]->ratio.size() > Precision + 1)
            {
                (*drawData)[i]->ratio.resize(Precision + 1);        // 只显示Precision位小数，算上小数点故+1
            }
            (*drawData)[i]->ratio += "%";
        }
    }

    //-----calc distance of text and axis-----//
    YFontSpace = height() / YFontSpaceRatio;
    XFontSpace = width() / XFontSpaceRatio;
    //-----calc distance of text and axis-----//

    //-----set Font Size&Style-----//
    YFontSize = height() / YaxisFontRatio;   // 计算字体大小
    YFont = QFont("Arial",YFontSize,QFont::Bold,true);  // 设置字体

    XFontSize = width() / XaxisFontRatio;   // 计算字体大小
    XFont = QFont("Arial",XFontSize,QFont::Bold,true);  // 设置字体
    //-----set Font Size&Style-----//
}
