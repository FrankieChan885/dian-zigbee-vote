#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QWidget>

#define STOP_WATCH_INCREASE_MODE 1
#define STOP_WATCH_DECREASE_MODE -1

class QTimer;

class StopWatch : public QWidget
{
    Q_OBJECT
public:
    explicit StopWatch(QWidget *parent = 0);

    void setMode(int mode);
    void draw(QPainter *painter);           // 画图

protected:
    void paintEvent(QPaintEvent *event);    // 重载paintEvent

signals:
    void autoStop();

public slots:
    void start();
    void pause();
    void stop();
    void SetStartTime(int sec);

private slots:
    void Tick();    // 计时

private:
    QTimer *timer;
    int sSecond;    // startSecond开始的时间
    int cSecond;    // currentSecond当前时间
    int tick;   // 1 -- 增加
                // -1 -- 减少
};

#endif // STOPWATCH_H
