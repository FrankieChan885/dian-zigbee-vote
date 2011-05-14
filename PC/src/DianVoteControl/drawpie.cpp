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

    // �����ӿ�Ϊ�������ڴ�С
    painter.setViewport(0, 0, width(), height());
    // ���ÿ��Ի�ͼ�Ĵ�С
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
        // ���������ɫ
        painter->setBrush(QBrush((*drawData)[i]->color, Qt::SolidPattern));
        // ��������ó���ʾ��ȷ��
        if (correctAnswerFlag)
        {
            // �������ȷ�𰸣����ó���ɫ������Ϊ��ɫ
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
        QFont Font = QFont("Arial",FontSize,QFont::Bold,true);  // ��������
        painter->setFont(Font);
        painter->drawText(drawData->at(i)->PieTextX, drawData->at(i)->PieTextY,
                          ratio);
    }
}

void DrawPie::DoWithCoodinate()
{
    int i;
    float percent;

    CentreOfPieX = width() / 2;     // ����Բ������
    CentreOfPieY = height() / 2;
    PieRadius = RatioPieRadius * (CentreOfPieY > CentreOfPieX ? CentreOfPieX : CentreOfPieY);
                                                   // ����뾶��С����ֹ�����߽�

    FontSize = PieRadius / RatioText;

    DrawRegion->setX(CentreOfPieX - PieRadius);
    DrawRegion->setY(CentreOfPieY - PieRadius);
    DrawRegion->setWidth(PieRadius * 2);
    DrawRegion->setHeight(PieRadius * 2);

    voterNums = 0;
    for (i = 0; i < optionNums; i++)
    {
        voterNums += (*drawData)[i]->voterNum;   // ����ͶƱ������
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
        // ����X���꣬��ֹ�����ͼ����
        if(cos(angle) < 0)
        {
            (*drawData)[i]->PieTextX -= PieRadius / RatioLeftTextSpace;
        }
        else
        {
            (*drawData)[i]->PieTextX += PieRadius / RatioRightTextSpace;
        }

        (*drawData)[i]->PieTextY = CentreOfPieY - PieRadius * sin(angle);
        // �������꣬��ֹ��������������ͬ��������໥���ǵ����
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
        (*drawData)[i]->ratio = QString("%1").arg(percent * 100);       // �������
        if ((*drawData)[i]->ratio.size() > Precision + 1)
        {
            (*drawData)[i]->ratio.resize(Precision + 1);        // ֻ��ʾPrecisionλС��������С�����+1
        }
        (*drawData)[i]->ratio += "%";

    }
    // ��ȫ���һ���Σ���ֹ���պϵ��������
    (*drawData)[i - 1]->pieSpanAngle = originStartAngle + 360 - \
                                       (*drawData)[i - 1]->pieStartAngle;
}

