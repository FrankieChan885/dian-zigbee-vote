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
    int correctAnswerIndex;                   // 哪一个是正确答案？

    QString questionName;
    Ui::DianVoteDrawer *ui;
    Histgram *histgram;
    Pie *pie;

    QSignalMapper *signalMapper;              // SignalMapper

    // 按钮的Actions
    QAction *dashLine;                        // 纵轴横线
    QMenu *rectangelStyle;                  // 显示风格
    QAction *SolidPattern;                    // 1 -- SolidPattern -- default 纯色
    QAction *Dense5Pattern;                   // 2 -- Dense5Pattern 点状填充
    QAction *CrossPattern;                    // 3 -- CrossPattern 左斜线填充
};

#endif // DIANVOTEDRAWER_H
