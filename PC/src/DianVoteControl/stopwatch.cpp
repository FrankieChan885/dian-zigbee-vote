#include "stopwatch.h"
#include <QPainter>
#include <QTimer>
#include <QMessageBox>

StopWatch::StopWatch(QWidget *parent) :
    QWidget(parent)
    , sSecond(0)
    , cSecond(0)
    , tick(1)
{
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(Tick()));
}

void StopWatch::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 设置视口为整个窗口大小
    painter.setViewport(0, 0, width(), height());
    // 设置可以绘图的大小
    painter.setWindow(0, 0, width(), height());

    draw(&painter);
}

void StopWatch::draw(QPainter *painter)
{
    // 在这里需要画出时间drawText
    painter->drawText(width() / 2, height() / 2, QString("%1").arg(cSecond));
}

void StopWatch::SetStartTime(int sec)
{
    sSecond = sec;
    cSecond = sSecond;
}

void StopWatch::start()
{
    if (timer->isActive())
    {
        return;
    }
    timer->start();
}

void StopWatch::pause()
{
    if(!timer->isActive())
    {
        return;
    }
    timer->stop();
}

void StopWatch::stop()
{
    if(!timer->isActive())
    {
        cSecond = sSecond;  // 重置
        return;
    }
    timer->stop();
    cSecond = sSecond;  // 重置
}

void StopWatch::Tick()
{
    cSecond += tick;
    update();

    if(tick == STOP_WATCH_DECREASE_MODE &&
       !cSecond)
    {
        timer->stop();
        emit autoStop();
    }
}

void StopWatch::setMode(int mode)
{
    tick = mode;
}
