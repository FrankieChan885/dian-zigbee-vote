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
    void draw(QPainter *painter);           // ��ͼ

protected:
    void paintEvent(QPaintEvent *event);    // ����paintEvent

signals:
    void autoStop();

public slots:
    void start();
    void pause();
    void stop();
    void SetStartTime(int sec);

private slots:
    void Tick();    // ��ʱ

private:
    QTimer *timer;
    int sSecond;    // startSecond��ʼ��ʱ��
    int cSecond;    // currentSecond��ǰʱ��
    int tick;   // 1 -- ����
                // -1 -- ����
};

#endif // STOPWATCH_H
