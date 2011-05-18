/* 唉...第一次用QPainter，代码写的不是一般的乱...
 * 画图的时候重新定义一个坐标轴和一个坐标0点zeroPoint
 * 所有的画图都是zeroPoint为标准的，所以要想看懂为什么那么算坐标
 * 先看懂zeroPoint是怎么设置的...OTZ..
 */

#include "histgram.h"
#include <QPoint>
#include <QPen>
#include <QColor>
#include <QFont>

const int WidthSlices = 18;      // 将窗口宽分成20份
const int HeightSlices = 20;     // 将窗口高分成20份
const float XaixsLengthRatio = 16 / (float)WidthSlices;      // X坐标占窗口的比例
const float YaxisLengthRatio = 15 / (float)HeightSlices;     // Y坐标占窗口的比例

const int ScaleSize = 5;         // X和Y轴上刻度的宽带或者高度
static int yFrontSpace = 25;     // 字离坐Y标轴的距离
static int xFrontSpace = 25;     // 字离坐X标轴的距离
static int FontSize = 15;        // 默认字体大小
static int YaxisSlices = 10;     // 以10为度量画Y轴上的刻度,当投票人数少于等于10的时候改为1

const int precision = 4;         // 显示比例时的精确度
const float rectRatio = 0.67;    // 决定柱状条的宽度
const int ratioRectSize = 10;    // 用于显示比例的矩形大小

Histgram::Histgram(QWidget *parent) :
    QWidget(parent), columnNums(5), voterNums(100),
    yAxisDashLineFlag(true), rectangleStyle(1),
    disCorrectAnswerFlag(false),questionName("Question 1")
{
    setDefaultDrawData();   //默认数据
    setDefaultColorSet();   //默认颜色
}

// 实际调用的Histgram初始化构造函数
Histgram::Histgram(QList<OptionData> &data)
{
    // some default values
    voterNums = 100; // default
    yAxisDashLineFlag = true;
    rectangleStyle = 1; // SolidPattern
    disCorrectAnswerFlag = false;
    questionName = "Question 1";

    // 设置颜色
    setDefaultColorSet();

    // 设置数据
    columnNums = data.length();
    drawData = new QList< OptionData >(data);
}

Histgram::~Histgram()
{
    if (drawData != NULL)
    {
        delete drawData;
    }
    delete colorSet;
}

void Histgram::setDefaultDrawData()
{
    // 初始化数据
    drawData = new QList< OptionData >();
    // 默认值
    drawData->append({"A", 20});
    drawData->append({"B", 30});
    drawData->append({"C", 19});
    drawData->append({"D", 8});
    //drawData->append({"E", 23});

    columnNums = drawData->length();
}

void Histgram::setDefaultColorSet()
{
    // 初始化颜色集合
    colorSet = new QStringList();
    colorSet->append("blue");
    colorSet->append("magenta");
    colorSet->append("cyan");
    colorSet->append("yellow");
    colorSet->append("darkCyan");
    colorSet->append("darkMagenta");
}

void Histgram::setColumnNums(int nums)
{
    columnNums = nums;
}

int Histgram::getColumnNums()
{
    return columnNums;
}

void Histgram::setData(QList< OptionData > &data)
{
    // 如果数据不为空，则首先清空数据
    if (!drawData->empty())
    {
        delete drawData;
    }
    drawData = new QList< OptionData >(data);
}

QString Histgram::getOptionName(int index)
{
    return (*drawData)[index].getOptionName();
}

void Histgram::setColorSet(QList<QString> &set)
{
    if (set.length() < columnNums)
    {
        // 传参错误，使用默认颜色
        setDefaultColorSet();
    }
    for (int i = 0; i < columnNums; i++)
    {
        (*colorSet)[i] = set[i];
    }
    update();
}

void Histgram::draw(QPainter *painter)
{
    QPen thickPen(palette().foreground(), 4);
    QPen thinPen(palette().foreground(), 0.5);

    // 默认0点坐标在左上角...
    QPoint zeroPoint(width() / WidthSlices,
                     (HeightSlices - 2) * height() / HeightSlices);
    int xaxisLength = XaixsLengthRatio * width();
    int yaxisLength = YaxisLengthRatio * height();
    painter->setPen(thickPen);
    // 画出Y轴
    painter->drawLine(zeroPoint.x(), zeroPoint.y(),
                      zeroPoint.x(), zeroPoint.y() - yaxisLength);
                                                // 0_0 默认0点坐标在左上角...
    // 画出X轴
    painter->drawLine(zeroPoint.x(), zeroPoint.y(),
                      zeroPoint.x() + xaxisLength, zeroPoint.y());

    // 画出Y轴刻度
    if (voterNums <= 10)     // 检查投票总人数是否为0
    {
        if (!voterNums)
        {
            return;
        }
        YaxisSlices = 1;//投票人数少于等于10的时候改为1
    }
    int yaxisScaleNums = qRound((float)voterNums / (float)YaxisSlices);    // 刻度数目
    int yaxisScaleSpace = yaxisLength / yaxisScaleNums;     // 刻度间隔
    for (int i = 1; i <= yaxisScaleNums; i++)
    {
        painter->setPen(thickPen);
        painter->drawLine(zeroPoint.x() - ScaleSize, zeroPoint.y() - i * yaxisScaleSpace,
                          zeroPoint.x(), zeroPoint.y() - i * yaxisScaleSpace);
                                            // 是“-”...原坐标0点在左上角问题...
        QString num = QString("%1").arg(i * YaxisSlices);
        painter->drawText(zeroPoint.x() - yFrontSpace, zeroPoint.y() - i * yaxisScaleSpace, num);
        // 如果纵轴虚线被设置成显示状态
        if (yAxisDashLineFlag)
        {
            painter->setPen(QPen(Qt::black, 0.1, Qt::DashLine, Qt::SquareCap));
            painter->drawLine(zeroPoint.x(), zeroPoint.y() - i * yaxisScaleSpace,
                              zeroPoint.x() + xaxisLength, zeroPoint.y() - i * yaxisScaleSpace);
        }
    }

    // 画出柱状图和X轴
    int xaxisScaleNums = columnNums;    // 刻度数目等于列数
    int xaxisScaleSpace = xaxisLength / xaxisScaleNums;     // 刻度间隔
    int rectWidth = rectRatio * xaxisScaleSpace;        // 设置柱状条宽度
    FontSize = (width() + height()) / 70;              // 设置字体大小
    xFrontSpace = (width() + height()) / 45;            // 设置字离X轴的距离
    QFont font("Arial",FontSize,QFont::Bold,true);  // 设置字体
    for (int i = 1; i <= columnNums; i++)
    {
        // 画出X轴刻度
        painter->setPen(thickPen);
        painter->drawLine(zeroPoint.x() + i * xaxisScaleSpace, zeroPoint.y(),
                          zeroPoint.x() + i * xaxisScaleSpace, zeroPoint.y() + ScaleSize);
        // 画出选项
        painter->setFont(font);
        painter->drawText(zeroPoint.x() + i * xaxisScaleSpace -(xaxisScaleSpace + FontSize * drawData->at(i - 1).getOptionName().length()) /2, // 文字的位置在每两个刻度的中间
                          zeroPoint.y() + xFrontSpace,
                          drawData->at(i - 1).getOptionName());

        // 画柱状图
        painter->setRenderHint(QPainter::Antialiasing, true);
        thinPen.setColor("black");
        painter->setPen(thickPen);
        // style determined by rectangelMetirial
        switch (rectangleStyle)
        {
        case 1:
            {
                painter->setBrush(QBrush(colorSet->at(i), Qt::SolidPattern));
                break;
            }
        case 2:
            {
                painter->setBrush(QBrush(colorSet->at(i), Qt::Dense5Pattern));
                break;
            }
        case 3:
            {
                painter->setBrush(QBrush(colorSet->at(i), Qt::CrossPattern));
                break;
            }
        default:
            {
                // 默认为纯色
                painter->setBrush(QBrush(colorSet->at(i), Qt::SolidPattern));
            }
        }

        // 如果被设置成显示正确答案
        if (disCorrectAnswerFlag)
        {
            // 如果是正确答案，设置成绿色，否则为红色
            if (drawData->at(i - 1).getOptionName() == correctAnswer)
            {
                painter->setBrush(QBrush("green", Qt::SolidPattern));
            }
            else
            {
                painter->setBrush(QBrush("red", Qt::SolidPattern));
            }
        }


        // 长方形左上角坐标
        int rectHeight = qRound(yaxisLength * ((float)drawData->at(i - 1).getVoterNum() / (float)voterNums));
        int rectX = zeroPoint.x() + i * xaxisScaleSpace - (xaxisScaleSpace + rectWidth) / 2;
        int rectY = zeroPoint.y() - rectHeight;

        painter->drawRect(rectX, rectY, rectWidth, rectHeight);
        // 画出各项投票人数及其所占比例
        painter->setFont(font);
        QString nums = QString("%1").arg(drawData->at(i - 1).getVoterNum());    // 将投票人数转换成字符串
        QString ratio = QString("%1").arg((float)drawData->at(i - 1).getVoterNum() \
                                          / (float)voterNums * 100);   // 算出比例，专程字符串
        if (ratio.size() > precision + 1)
        {
            ratio.resize(precision + 1);        // 只显示precision位小数，算上小数点故+1
        }
        ratio += "%";
        QString text = nums + "(" + ratio + ")";
        painter->drawText(rectX + (rectWidth - FontSize * (text.length() - 3)) / 2, \
                          rectY - FontSize / 2, text);      // 3 是试出来的... - -!
    }

    // 画出题目名称
    QFont titleFont("Arial",FontSize,QFont::Bold,true);
    painter->setFont(titleFont);
    painter->drawText(zeroPoint.x(), FontSize, questionName);
}

void Histgram::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 设置视口为整个窗口大小
    painter.setViewport(0, 0, width(), height());
    // 设置可以绘图的大小
    painter.setWindow(0, 0, width(), height());

    draw(&painter);
}

// 将名字为optionName的列的高度加1
void Histgram::updateData(QString& optionName)
{
    for (int i = 0; i < drawData->length(); i++)
    {
        if ((*drawData)[i].getOptionName() == optionName)
        {
            // 投了选项optionName的投票总数+1
            (*drawData)[i].setVoteNum((drawData->at(i).getVoterNum() + 1));
        }
    }
    update();
}

// 设置是否显示纵轴虚线
void Histgram::setyAxisDashLineFlag()
{
    yAxisDashLineFlag = !yAxisDashLineFlag;
    update();
}

void Histgram::setRectangelStyle(int flag)
{
    rectangleStyle = flag;
    update();
}

void Histgram::setSolidPatternStyle()
{
    setRectangelStyle(1);
}

void Histgram::setDense5PatternStyle()
{
    setRectangelStyle(2);
}

void Histgram::setCrossPatternStyle()
{
    setRectangelStyle(3);
}

void Histgram::setUsetCorrectAnswer()
{
    if (correctAnswer.length())
    {
        disCorrectAnswerFlag = !disCorrectAnswerFlag;
        update();
    }
}

void Histgram::setUsetCorrectAnswer(bool flag)
{
    if (correctAnswer.length())
    {
        disCorrectAnswerFlag = flag;
        update();
    }
}

void Histgram::setCorrectAnswer(QString answer)
{
    disCorrectAnswerFlag = true;
    correctAnswer = answer;
    update();
}
