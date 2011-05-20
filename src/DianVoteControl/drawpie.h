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

    void draw(QPainter *painter);           // ��ͼ

protected:
    void paintEvent(QPaintEvent *event);    // ����paintEvent

private:
    void DoWithCoodinate();     // ���㻭ͼʱ��Ҫ������

private:
    //---- configure values----//
    int FontSize;           // �����С
    int RatioText;          // �����С���ͼ�ı���
    int CoverTextSpace;     // ������ͶƱ��ĿΪ0��������ֵľ���
    float RatioLeftTextSpace; // ���������ľ�����뾶�ı���
    float RatioRightTextSpace; // �ұ�������ľ�����뾶�ı���
    int Precision;          // ��ʾ����ʱ�ľ�ȷ��
    int CentreOfPieX;       // Բ��X����
    int CentreOfPieY;       // Բ��Y����
    int PieRadius;          // ԰�뾶
    float RatioPieRadius;   // Բ�뾶�봰�ڴ�С�ı���
    QRect *DrawRegion;        // ����ֻ�ܻ�������һ����������
    //---- configure values----//
};

#endif // DRAWPIE_H
