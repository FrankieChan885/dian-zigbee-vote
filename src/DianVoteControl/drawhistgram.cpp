#include "drawhistgram.h"
#include <math.h>

DrawHistgram::DrawHistgram()
{
    YAxisDashLineFlag = true;  // Ĭ�ϲ���ʾ����

    WidthSlices = 18;      // �����ڿ�ֳ�20��
    HeightSlices = 20;     // �����ڸ߷ֳ�20��
    XaixsLengthRatio = 16 / (float)WidthSlices;      // X����ռ���ڵı���
    YaxisLengthRatio = 16 / (float)HeightSlices;     // Y����ռ���ڵı���

    ScaleSize = 5;         // X��Y���Ͽ̶ȵĿ�����߸߶�
    YFontSpace = 25;     // ������Y����ľ���
    YFontSpaceRatio = 15;    //
    XFontSpace = 25;     // ������X����ľ���
    XFontSpaceRatio = 30;
    YaxisFontRatio = 30;    // �����С������ı���
    YFontSize = 10;        // Ĭ�������С
    XaxisFontRatio = 50;    // �����С�����ı���
    XFontSize = 15;         // Ĭ�������С
    YaxisSlices = 10;     // ��10Ϊ������Y���ϵĿ̶�,��ͶƱ�������ڵ���10��ʱ���Ϊ1

    Precision = 4;         // ��ʾ����ʱ�ľ�ȷ��
    RectWidthRatio = 0.5;    // ������״���Ŀ��
    RectHeigthRatio = 0.8;      // ������״���ĸ߶�
    RatioRectSize = 10;    // ������ʾ�����ľ��δ�С

    // ��ʼ������ͶƱ������
    fakeVoterNums = INIT_VOTER_NUM;
}

DrawHistgram::~DrawHistgram()
{
}

void DrawHistgram::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // �����ӿ�Ϊ�������ڴ�С
    painter.setViewport(0, 0, width(), height());
    // ���ÿ��Ի�ͼ�Ĵ�С
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
    // ����Y��
//    painter->drawLine(zeroPoint.x(), zeroPoint.y(),
//                      zeroPoint.x(), zeroPoint.y() - YaxisLength);
                                                // 0_0 Ĭ��0�����������Ͻ�...
    // ����X��
    painter->drawLine(zeroPoint.x(), zeroPoint.y(),
                          zeroPoint.x() + XaxisLength, zeroPoint.y());

    // ��Y��̶�
    /*for (int i = 1; i <= YaxisScaleNums; i++)
    {
        painter->setPen(thinPen);
        painter->drawLine(zeroPoint.x() - ScaleSize, zeroPoint.y() - i * YaxisScaleSpace,
                          zeroPoint.x(), zeroPoint.y() - i * YaxisScaleSpace);
                                            // �ǡ�-��...ԭ����0�������Ͻ�����...

        painter->setFont(YFont);
        QString num = QString("%1").arg(i * BASIC_SLICE);
        painter->drawText(zeroPoint.x() - YFontSpace, zeroPoint.y() - i * YaxisScaleSpace, num);
        // ����������߱����ó���ʾ״̬
        if (YAxisDashLineFlag)
        {
            painter->setPen(QPen(Qt::black, 0.1, Qt::DashLine, Qt::SquareCap));
            painter->drawLine(zeroPoint.x(), zeroPoint.y() - i * YaxisScaleSpace,
                              zeroPoint.x() + XaxisLength, zeroPoint.y() - i * YaxisScaleSpace);
        }
    }*/

    // ������״ͼ��X��

    painter->drawLine(zeroPoint.x(), zeroPoint.y(),
                      zeroPoint.x(), zeroPoint.y() - ScaleSize);

    for (int i = 1; i <= optionNums; i++)
    {
        // ����X��̶�
        painter->setPen(thinPen);
        painter->setFont(XFont);
        painter->drawLine(zeroPoint.x() + i * XaxisScaleSpace, zeroPoint.y(),
                          zeroPoint.x() + i * XaxisScaleSpace, zeroPoint.y() - ScaleSize);
        // ����ѡ��
        painter->setFont(XFont);
        painter->drawText(zeroPoint.x() + i * XaxisScaleSpace -(XaxisScaleSpace + XFontSize * drawData->at(i - 1)->optionName.length()) /2, // ���ֵ�λ����ÿ�����̶ȵ��м�
                          zeroPoint.y() + XFontSpace,
                          drawData->at(i - 1)->optionName);

        // ����ֱ��ͼ
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setPen(thinPen);
        painter->setBrush(QBrush((*drawData)[i - 1]->color, Qt::SolidPattern));   // ���������ɫ
        // ��������ó���ʾ��ȷ��
        if (correctAnswerFlag)
        {
            // �������ȷ�𰸣����ó���ɫ������Ϊ��ɫ
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
                          (*drawData)[i - 1]->histgramWidth, (*drawData)[i - 1]->histgramHeight); // ����״ͼ

        if (displayResultWhileVoting)
        {
            // ��ʾͶƱ�˵ĸ���
            int numX = (*drawData)[i - 1]->histgramX +
                        ((*drawData)[i - 1]->histgramWidth - XFontSize * ((*drawData)[i - 1]->nums.length())) / 2;
            int numY = (*drawData)[i - 1]->histgramY - 2 * XFontSize;
            painter->drawText(numX, numY, (*drawData)[i - 1]->nums);

            // ��ʾͶƱ����
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

    int maxOptionVoteNum = 0;       // ͶƱ���������Ǹ�ѡ��ĸ��������ھ���Y��̶ȵĳ���

    for (int i = 0; i < optionNums; i++)
    {
        maxOptionVoteNum = drawData->at(i)->voterNum > maxOptionVoteNum ? \
                           drawData->at(i)->voterNum : maxOptionVoteNum;
    }
    // ��������ģʽ���ȹ̶�ʹ��Ĭ�ϵ�voterNum��Ȼ��ͶƱ���������ֵ����voterNum��ʱ��
    // voterNum�Զ����ӵ�ԭ����2��

    if(maxOptionVoteNum > fakeVoterNums)
    {
        fakeVoterNums *= 2;
    }

    /*YaxisScaleNums = ceil((double)fakefakeVoterNums / BASIC_SLICE);     // �̶���Ŀ
    YaxisScaleSpace =  RectHeigthRatio * YaxisLength / YaxisScaleNums;                    // �̶ȼ��
    YaxisScaleNums += 2;*/

    XaxisScaleNums = optionNums;        // �̶���=ѡ����
    XaxisScaleSpace = XaxisLength / XaxisScaleNums;     // �̶ȼ��

    for (int i = 0; i < optionNums; i++)
    {
        (*drawData)[i]->histgramWidth = RectWidthRatio * XaxisScaleSpace;     // ��״�����
        (*drawData)[i]->histgramHeight = (double)YaxisLength * (double)(*drawData)[i]->voterNum / (double)fakeVoterNums;   // ��״ͼ�߶�
        (*drawData)[i]->histgramX = zeroPoint.x() + (i + 1) * XaxisScaleSpace -
                                   (XaxisScaleSpace + (*drawData)[i]->histgramWidth) / 2;    // ��״�����Ͻ�X����
        (*drawData)[i]->histgramY = zeroPoint.y() - (*drawData)[i]->histgramHeight;       // ��״�����Ͻ�Y����
        if (displayResultWhileVoting)
        {
            (*drawData)[i]->nums = QString("%1").arg((*drawData)[i]->voterNum);       // ������������

            if(!voterNums)
            {
                (*drawData)[i]->ratio = QString("0");
            }
            else
            {
                (*drawData)[i]->ratio = QString("%1").arg((float)(*drawData)[i]->voterNum / voterNums * 100);       // �������
            }

            if ((*drawData)[i]->ratio.size() > Precision + 1)
            {
                (*drawData)[i]->ratio.resize(Precision + 1);        // ֻ��ʾPrecisionλС��������С�����+1
            }
            (*drawData)[i]->ratio += "%";
        }
    }

    //-----calc distance of text and axis-----//
    YFontSpace = height() / YFontSpaceRatio;
    XFontSpace = width() / XFontSpaceRatio;
    //-----calc distance of text and axis-----//

    //-----set Font Size&Style-----//
    YFontSize = height() / YaxisFontRatio;   // ���������С
    YFont = QFont("Arial",YFontSize,QFont::Bold,true);  // ��������

    XFontSize = width() / XaxisFontRatio;   // ���������С
    XFont = QFont("Arial",XFontSize,QFont::Bold,true);  // ��������
    //-----set Font Size&Style-----//
}
