#ifndef DRAWBASE_H
#define DRAWBASE_H

#include <QWidget>
#include <QString>
#include <QColor>
#include <QList>

#define INIT_VOTER_NUM 20   // ��ʼ��ͶƱ������

typedef struct _DisplayData
{
    int type;   // ֱ��ͼ���ݻ��Ǳ�ͼ����
    QString optionName; // ѡ������
    int voterNum;   // ѡ�����ѡ����˵ĸ���
    QString color;   // ��ͼʱ�õ���ɫ

    //-----coordinate info-----//
    int histgramX;      // ֱ��ͼ������
    int histgramY;      // ֱ��ͼ������
    int histgramWidth;  // ֱ��ͼ���
    int histgramHeight; // ֱ��ͼ�߶�
    int pieStartAngle;  // ��ͼ��ʼ�Ļ���
    int pieSpanAngle;   // ��ͼ�����Ļ���
    //-----coordinate info-----//

    //-----text info-----//
    QString nums;       // ͶƱ������
    QString ratio;      // ռ�ı���
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

    void ClearData();               // ÿһ��Start��Ҫ���ϴ��������
    void HandleData(int index);    // ����һ������

protected:
    bool correctAnswerFlag;     // �Ƿ���ʾ��ȷ��
    QString questionName;       // ��������
    QString correctAnswer;      // ��ȷ��
    int voterNums;              // ͶƱ��������
    int optionNums;             // ѡ�����Ŀ

    QList< DisplayData* > *drawData;    // ��ͼʱ�����ݵ�����
    QStringList *colorSet;              // ��ɫ����

private:
    void setDefaultDrawData();                  // ����Ĭ������
    void setDefaultColorSet();                  // ����Ĭ����ɫ����
};

#endif // DRAWBASE_H
