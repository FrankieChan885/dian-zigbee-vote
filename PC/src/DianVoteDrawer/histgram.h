/* histgram.h
 *
 * ���Ҫ������״ͼ��ֻ��Ҫ���ù��캯��Histgram(QList< OptionData > &data)���ɡ�
 * ��Ȼ�㻹��Ҫ����һЩ�������ڻ�ͼ������3�����ݾ��������������á�
 *
 * setVoterNums(int nums);    // ����ͶƱ������
 *              �������� ���Ǳ���ģ���Ҫ֪��ͶƱ��������
 * setColumnNums(int nums);   // ��������
 *              �������� �����룬�ڹ��캯���˻Ὣ����ĳ��ȸ�ֵ������
 * setData(QList< OptionData > &data);   //��������
 *              �������� �����룬�ɹ��캯�����ã����á�
 * ------------------------------------------------------
 *
 * ������һЩ��־λ�����ã����Ƕ���һЩpublic slot����Ҫ�������������źţ��ڽ������Ӽ��ɡ�
 * setyAxisDashLineFlag();    // ����Y��̶��ߴ��Ƿ񻭺�����
 * setRectangelStyle(int flag);    // ������״ͼ����
 * setCorrectAnswer(QString &correctAnswer);  // ������ȷ��
 */

#ifndef HISTGRAM_H
#define HISTGRAM_H

#include "datatype.h"
#include <QWidget>
#include <QPainter>
#include <QList>
#include <QString>
#include <QPair>

class Histgram : public QWidget
{
    Q_OBJECT
public:
    explicit Histgram(QWidget *parent = 0);
    Histgram(QList< OptionData > &data);
    ~Histgram();

    void setVoterNums(int nums);                // ����ͶƱ������
    int getColumnNums();                        // ��ȡ��������ʱ�����ţ������Ժ��ò���
    void setColumnNums(int nums);               // ��������
    QString getOptionName(int index);           // ��ȡѡ�����֣���ʱ�����ţ������Ժ��ò���
    void setData(QList< OptionData > &data);    //��������

    void draw(QPainter *painter);               // ��ͼ

protected:
    void setDefaultDrawData();                  // ����Ĭ������
    void setDefaultColorSet();                  // ����Ĭ����ɫ����

    void paintEvent(QPaintEvent *event);        // ����paintEvent

public slots:
    void updateData(QString &optionName);           // ����һ�����ݣ�ˢ����״ͼ
    void setyAxisDashLineFlag();                    // ����Y��̶��ߴ��Ƿ񻭺�����
    void setSolidPatternStyle();                    // ���ô�ɫ
    void setDense5PatternStyle();                   // ���õ�״���
    void setCrossPatternStyle();                    // ������б�����
    void setRectangelStyle(int flag);               // ������״ͼ����
    void setUsetCorrectAnswer();                    // ���û���ȡ����ȷ��
    void setUsetCorrectAnswer(bool flag);           // ���û���ȡ����ȷ��
    void setCorrectAnswer(QString correctAnswer);   // ������ȷ��
    void setColorSet(QList< QString > &set);        // ������ɫ����, ����

private:
    // flags����������״ͼ����ʾ
    bool yAxisDashLineFlag;         // Y��̶��ߴ��Ƿ񻭺����ߵ�flag--default==true
    int rectangleStyle;             // ��״ͼ�Ĳ��ʵ�flag
                                    // 1 -- SolidPattern -- default ��ɫ
                                    // 2 -- Dense5Pattern ��״���
                                    // 3 -- CrossPattern ��б�����
    bool disCorrectAnswerFlag;      // ��ʾ��ȷ�𰸲���

    // ˽������
    int voterNums;                  // ͶƱ��������
    int columnNums;                 // ��״ͼ����
    QString questionName;           // ���� ����Ŀһ��
    QString correctAnswer;          // ��ȷ�𰸣�ֻ����disCorrectAnswerΪtrueʱ����
                                    // ��correctAnswer == "" ʱҲ��ʾ����ʾ��ȷ��
    QStringList *colorSet;          // ��ɫ�ļ��ϣ���ͼ�Ǵ���ȡ��ɫ
    QList< OptionData > *drawData;  // ��״ͼ�����������ͼ�λ�
                                    // QString--ѡ������    int--����

};

#endif // HISTGRAM_H
