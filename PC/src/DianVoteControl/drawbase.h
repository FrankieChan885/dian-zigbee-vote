#ifndef DRAWBASE_H
#define DRAWBASE_H

#include <QWidget>
#include <QString>
#include <QColor>
#include <QList>

#define INIT_VOTER_NUM 20   // 初始化投票总人数

typedef struct _DisplayData
{
    int type;   // 直方图数据还是饼图数据
    QString optionName; // 选项名称
    int voterNum;   // 选了这个选项的人的个数
    QString color;   // 画图时用的颜色

    //-----coordinate info-----//
    int histgramX;      // 直方图横坐标
    int histgramY;      // 直方图纵坐标
    int histgramWidth;  // 直方图宽度
    int histgramHeight; // 直方图高度
    int pieStartAngle;  // 饼图开始的弧度
    int pieSpanAngle;   // 饼图经过的弧度
    //-----coordinate info-----//

    //-----text info-----//
    QString nums;       // 投票的人数
    QString ratio;      // 占的比例
    //-----text info-----//
}DisplayData;

class DrawBase : public QWidget
{
    Q_OBJECT
public:
    explicit DrawBase(QWidget *parent = 0);
    virtual ~DrawBase();

signals:

public slots:
    void SetQuestionName(QString name);
    void SetOptionNums(int nums);
    void SetVoterNums(int nums);
    void SetCorrectAnswer(QString answer);
    void SetCorrectAnswerFlag(bool flag);

    void ClearData();               // 每一次Start都要将上次数据清空
    void HandleData(int index);    // 接收一个数据

protected:
    bool correctAnswerFlag;     // 是否显示正确答案
    QString questionName;       // 问题名称
    QString correctAnswer;      // 正确答案
    int voterNums;              // 投票的总人数
    int optionNums;             // 选项的数目

    QList< DisplayData* > *drawData;    // 画图时所依据的数据
    QStringList *colorSet;              // 颜色集合

private:
    void setDefaultDrawData();                  // 设置默认数据
    void setDefaultColorSet();                  // 设置默认颜色方案
};

#endif // DRAWBASE_H
