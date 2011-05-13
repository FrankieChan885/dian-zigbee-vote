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

    // �����ӿ�Ϊ�������ڴ�С
    painter.setViewport(0, 0, width(), height());
    // ���ÿ��Ի�ͼ�Ĵ�С
    painter.setWindow(0, 0, width(), height());

    draw(&painter);
}

void StopWatch::draw(QPainter *painter)
{
    int fontSize = height() / 4;
    QFont Font = QFont("Arial",fontSize,QFont::Bold,true);
    painter->setFont(Font);

    // ��������Ҫ����ʱ��drawText
    QString Minute = QString("%1").arg(cSecond / 60);
    QString dSecond = QString("%1").arg(cSecond % 60 / 10);
    QString sSecond = QString("%1").arg(cSecond % 60 % 10);
    QString Time = Minute + " : " + dSecond + sSecond;
    painter->drawText(width() / 2, height() / 2, Time);
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
        cSecond = sSecond;  // ����
        return;
    }
    timer->stop();
    cSecond = sSecond;  // ����
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
