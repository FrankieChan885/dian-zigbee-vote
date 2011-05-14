#include "drawpie.h"
#include <QFont>
#include <math.h>
#include <QPainter>

DrawPie::DrawPie() :
        RatioText(10),
        CoverTextSpace(25),
        RatioLeftTextSpace(1.5),
        RatioRightTextSpace(8.9),
        Precision(4),
        RatioPieRadius(0.8)
{
    DrawRegion = new QRect();
}

DrawPie::~DrawPie()
{

}

void DrawPie::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 设置视口为整个窗口大小
    painter.setViewport(0, 0, width(), height());
    // 设置可以绘图的大小
    painter.setWindow(0, 0, width(), height());

    draw(&painter);
}

void DrawPie::draw(QPainter *painter)
{
    DoWithCoodinate();

    if(voterNums == 0)
    {
        return;
    }

    for (int i = 0; i < optionNums; i++)
    {
        // 设置填充颜色
        painter->setBrush(QBrush((*drawData)[i]->color, Qt::SolidPattern));
        // 如果被设置成显示正确答案
        if (correctAnswerFlag)
        {
            // 如果是正确答案，设置成绿色，否则为红色
            if (drawData->at(i)->optionName == correctAnswer)
            {
                painter->setBrush(QBrush("green", Qt::SolidPattern));
            }
            else
            {
                painter->setBrush(QBrush("red", Qt::SolidPattern));
            }
        }
        painter->drawPie(*DrawRegion,
                         drawData->at(i)->pieStartAngle * 16,
                         drawData->at(i)->pieSpanAngle * 16);

        QString ratio = drawData->at(i)->optionName + ": " + drawData->at(i)->ratio;
        QFont Font = QFont("Arial",FontSize,QFont::Bold,true);  // 设置字体
        painter->setFont(Font);
        painter->drawText(drawData->at(i)->PieTextX, drawData->at(i)->PieTextY,
                          ratio);
    }
}

void DrawPie::DoWithCoodinate()
{
    int i;
    float percent;

    CentreOfPieX = width() / 2;     // 计算圆心坐标
    CentreOfPieY = height() / 2;
    PieRadius = RatioPieRadius * (CentreOfPieY > CentreOfPieX ? CentreOfPieX : CentreOfPieY);
                                                   // 计算半径大小，防止超出边界

    FontSize = PieRadius / RatioText;

    DrawRegion->setX(CentreOfPieX - PieRadius);
    DrawRegion->setY(CentreOfPieY - PieRadius);
    DrawRegion->setWidth(PieRadius * 2);
    DrawRegion->setHeight(PieRadius * 2);

    voterNums = 0;
    for (i = 0; i < optionNums; i++)
    {
        voterNums += (*drawData)[i]->voterNum;   // 计算投票总人数
    }

    int originStartAngle = DEFAULT_START_ANGLE;
    int startAngle = DEFAULT_START_ANGLE;
    for(i = 0; i < optionNums; i++)
    {
        if(!voterNums)
        {
            percent = 0;
        }
        else
        {
            percent = (float)drawData->at(i)->voterNum / (float)voterNums;
        }
        (*drawData)[i]->pieSpanAngle = qRound(360 * percent);
        (*drawData)[i]->pieStartAngle = startAngle;
        startAngle += (*drawData)[i]->pieSpanAngle;

        float angle = float((*drawData)[i]->pieStartAngle + (*drawData)[i]->pieSpanAngle / 2) * \
                      PI / 180;
        (*drawData)[i]->PieTextX = CentreOfPieX + PieRadius * cos(angle);
        // 调整X坐标，防止字与饼图覆盖
        if(cos(angle) < 0)
        {
            (*drawData)[i]->PieTextX -= PieRadius / RatioLeftTextSpace;
        }
        else
        {
            (*drawData)[i]->PieTextX += PieRadius / RatioRightTextSpace;
        }

        (*drawData)[i]->PieTextY = CentreOfPieY - PieRadius * sin(angle);
        // 调整坐标，防止出现两个坐标相同的情况，相互覆盖的情况
        for(int j = 0; j < i; j++)
        {
            if((*drawData)[i]->PieTextX == (*drawData)[j]->PieTextX)
            {
                (*drawData)[i]->PieTextX += CoverTextSpace;
            }
            if((*drawData)[i]->PieTextY == (*drawData)[j]->PieTextY)
            {
                (*drawData)[i]->PieTextY += CoverTextSpace;
            }

        }

        (*drawData)[i]->nums = QString("%1").arg((*drawData)[i]->voterNum);
        (*drawData)[i]->ratio = QString("%1").arg(percent * 100);       // 算出比例
        if ((*drawData)[i]->ratio.size() > Precision + 1)
        {
            (*drawData)[i]->ratio.resize(Precision + 1);        // 只显示Precision位小数，算上小数点故+1
        }
        (*drawData)[i]->ratio += "%";

    }
    // 补全最后一扇形，防止不闭合的情况发生
    (*drawData)[i - 1]->pieSpanAngle = originStartAngle + 360 - \
                                       (*drawData)[i - 1]->pieStartAngle;
}

