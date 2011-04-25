/* histgram.h
 *
 * 如果要画出柱状图，只需要调用构造函数Histgram(QList< OptionData > &data)即可。
 * 当然你还需要设置一些东西用于画图。下面3个根据具体的情况进行设置。
 *
 * setVoterNums(int nums);    // 设置投票总人数
 *              ———— 这是必须的，需要知道投票的总人数
 * setColumnNums(int nums);   // 设置列数
 *              ———— 不必须，在构造函数了会将链表的长度赋值给列数
 * setData(QList< OptionData > &data);   //设置数据
 *              ———— 不必须，由构造函数设置，备用。
 * ------------------------------------------------------
 *
 * 以下是一些标志位的设置，它们都是一些public slot，如要调用请先申明信号，在进行连接即可。
 * setyAxisDashLineFlag();    // 设置Y轴刻度线处是否画横虚线
 * setRectangelStyle(int flag);    // 设置柱状图材质
 * setCorrectAnswer(QString &correctAnswer);  // 设置正确答案
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

    void setVoterNums(int nums);                // 设置投票总人数
    int getColumnNums();                        // 获取列数，暂时先用着，估计以后用不着
    void setColumnNums(int nums);               // 设置列数
    QString getOptionName(int index);           // 获取选项名字，暂时先用着，估计以后用不着
    void setData(QList< OptionData > &data);    //设置数据

    void draw(QPainter *painter);               // 画图

protected:
    void setDefaultDrawData();                  // 设置默认数据
    void setDefaultColorSet();                  // 设置默认颜色方案

    void paintEvent(QPaintEvent *event);        // 重载paintEvent

public slots:
    void updateData(QString &optionName);           // 接收一个数据，刷新柱状图
    void setyAxisDashLineFlag();                    // 设置Y轴刻度线处是否画横虚线
    void setSolidPatternStyle();                    // 设置纯色
    void setDense5PatternStyle();                   // 设置点状填充
    void setCrossPatternStyle();                    // 设置左斜线填充
    void setRectangelStyle(int flag);               // 设置柱状图材质
    void setUsetCorrectAnswer();                    // 设置或者取消正确答案
    void setUsetCorrectAnswer(bool flag);           // 设置或者取消正确答案
    void setCorrectAnswer(QString correctAnswer);   // 设置正确答案
    void setColorSet(QList< QString > &set);        // 设置颜色方案, 备用

private:
    // flags用来配置柱状图的显示
    bool yAxisDashLineFlag;         // Y轴刻度线处是否画横虚线的flag--default==true
    int rectangleStyle;             // 柱状图的材质的flag
                                    // 1 -- SolidPattern -- default 纯色
                                    // 2 -- Dense5Pattern 点状填充
                                    // 3 -- CrossPattern 左斜线填充
    bool disCorrectAnswerFlag;      // 显示正确答案不？

    // 私有数据
    int voterNums;                  // 投票的总人数
    int columnNums;                 // 柱状图列数
    QString questionName;           // 比如 “题目一”
    QString correctAnswer;          // 正确答案，只有在disCorrectAnswer为true时有用
                                    // 当correctAnswer == "" 时也表示不显示正确答案
    QStringList *colorSet;          // 颜色的集合，画图是从中取颜色
    QList< OptionData > *drawData;  // 柱状图就是这个数据图形化
                                    // QString--选项名字    int--人数

};

#endif // HISTGRAM_H
