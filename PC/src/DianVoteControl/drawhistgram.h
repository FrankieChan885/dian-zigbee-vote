#ifndef DRAWHISTGRAM_H
#define DRAWHISTGRAM_H

#include "drawbase.h"
#include <QPainter>

#define BASIC_SLICE 20
#define FONT_RATIO 70       // ���ڶ�̬���������С
#define FONT_SPACE 45       // ���ڶ�̬����������������ľ���

class DrawHistgram : public DrawBase
{
    Q_OBJECT

public:
    DrawHistgram();
    ~DrawHistgram();

    void draw(QPainter *painter);           // ��ͼ

protected:
    void paintEvent(QPaintEvent *event);    // ����paintEvent

private:
    void DoWithCoodinate();     // ���㻭ͼʱ��Ҫ������

    bool YAxisDashLineFlag;

    //-----Important Coordinate-----//
    QPoint zeroPoint;       // ��ͼʹ�õ�0������
    int XaxisLength;        // X��ĳ���
    int YaxisLength;        // Y��ĳ���
    //-----Important Coordinate-----//

    //-----Config Values-----//
    int WidthSlices;        // �����ڿ�ֳ�20��
    int HeightSlices;       // �����ڸ߷ֳ�20��
    float XaixsLengthRatio; // X����ռ���ڵı���
    float YaxisLengthRatio; // Y����ռ���ڵı���

    int ScaleSize;          // X��Y���Ͽ̶ȵĿ�����߸߶�
    int YFontSpace;        // ������Y����ľ���
    int YFontSpaceRatio;    // ����Y����ľ���ı���
    int XFontSpace;        // ������X����ľ���
    int XFontSpaceRatio;    // ����X����ľ���ı���
    int YaxisFontRatio;
    int YFontSize;           // Y��Ĭ�������С
    int XFontSize;          // X��Ĭ�������С
    int XaxisFontRatio;
    int YaxisSlices;        // ��10Ϊ������Y���ϵĿ̶�,��ͶƱ�������ڵ���10��ʱ���Ϊ1

    int YaxisScaleNums;     // Y����̶ȵ���Ŀ
    int YaxisScaleSpace;    // Y����̶ȼ��
    int XaxisScaleNums;      // X����̶ȵ���Ŀ
    int XaxisScaleSpace;    // X����̶ȼ��

    int Precision;          // ��ʾ����ʱ�ľ�ȷ��
    float RectWidthRatio;        // ������״���Ŀ��
    float RectHeigthRatio;      // ������״ͼ�ĸ߶�������ı���
    int RatioRectSize;      // ������ʾ�����ľ��δ�С
    //-----Config Values-----//

    //-----for font style-----//
    QFont YFont;             // Y���������ֵ���ʽ
    QFont XFont;            // X���꼰��״ͼ�ϵ��ֵ���ʽ
    //-----for font style-----//
};

#endif // DRAWHISTGRAM_H
