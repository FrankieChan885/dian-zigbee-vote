#ifndef DIANVOTEDRAWER_H
#define DIANVOTEDRAWER_H

#include <QWidget>
#include <QPainter>
#include <QColor>
#include <QAction>
#include <QMenu>
#include <QSignalMapper>
#include "histgram.h"
#include "pie.h"

namespace Ui {
    class DianVoteDrawer;
}

class DianVoteDrawer : public QWidget
{
    Q_OBJECT

public:
    explicit DianVoteDrawer(QWidget *parent = 0);
    ~DianVoteDrawer();

signals:
    void CorretAnswer(QString answer);
    void RectangleStyle(int flag);

private slots:

private:
    int correctAnswerIndex;                   // ��һ������ȷ�𰸣�

    QString questionName;
    Ui::DianVoteDrawer *ui;
    Histgram *histgram;
    Pie *pie;

    QSignalMapper *signalMapper;              // SignalMapper

    // ��ť��Actions
    QAction *dashLine;                        // �������
    QMenu *rectangelStyle;                  // ��ʾ���
    QAction *SolidPattern;                    // 1 -- SolidPattern -- default ��ɫ
    QAction *Dense5Pattern;                   // 2 -- Dense5Pattern ��״���
    QAction *CrossPattern;                    // 3 -- CrossPattern ��б�����
};

#endif // DIANVOTEDRAWER_H
