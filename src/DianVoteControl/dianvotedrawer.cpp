#include <QFont>
#include <QTimer>
#include <QString>
#include <QPainter>
#include <QToolButton>
#include "drawpie.h"
#include "drawhistgram.h"
#include "dianvotedrawer.h"
#include "ui_dianvotedrawer.h"

DianVoteDrawer::DianVoteDrawer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DianVoteDrawer),
    replidDeviceNum(0)
{
    ui->setupUi(this);

    previous = new QToolButton();
    previous->setObjectName(tr("Previous_Drawer"));
    ui->NextPrevious->addWidget(previous, 0, 1);

    next = new QToolButton();
    next->setObjectName(tr("Next_Drawer"));
    ui->NextPrevious->addWidget(next, 0, 2);

    showPie = new QToolButton();
    showPie->setObjectName(tr("Pie_Drawer"));
    ui->ContrulBar->addWidget(showPie, 0, 3);

    showHistgram = new QToolButton();
    showHistgram->hide();
    showHistgram->setObjectName(tr("Histgram_Drawer"));
    ui->ContrulBar->addWidget(showHistgram, 0, 3);

    correctAnswer = new QToolButton();
    correctAnswer->setObjectName(tr("CorrectAnsewer_Drawer"));
    ui->ContrulBar->addWidget(correctAnswer, 0, 4);

    option = new QToolButton();
    option->setObjectName(tr("Option_Drawer"));
    ui->ContrulBar->addWidget(option, 0, 5);

    close = new QToolButton();
    close->setObjectName(tr("Close_Drawer"));
    ui->ContrulBar->addWidget(close, 0, 6);

    connect(close, SIGNAL(clicked()), this, SLOT(close()));
    connect(showPie, SIGNAL(clicked()), this, SLOT(SwithChart()));
    connect(showHistgram, SIGNAL(clicked()), this, SLOT(SwithChart()));

    histgram = new DrawHistgram();
    ui->DrawConvas->addWidget(histgram);

    pie = new DrawPie();
    ui->DrawConvas->addWidget(pie);
    pie->hide();

    timer = new QTimer();
    timer->setInterval(DEFAULT_UPDATE_TIMER);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start();

    FontSizeRatio = 35;         // 控制字体大小
    RatioRepliedDeviceX = 0.52;   // 总数位置与窗口的宽度比例
    RatioRepliedDeviceY = 0.1;   //总数位置与窗口的高度度比例

    RatioTotalNumX = 0.75;       // 总数位置与窗口的宽度比例
    RatioTotalNumY = 0.1;       // 总数位置与窗口的高度度比例
}

DianVoteDrawer::~DianVoteDrawer()
{
    if(histgram)
    {
        delete histgram;
    }

    if(pie)
    {
        delete pie;
    }
}

void DianVoteDrawer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 设置视口为整个窗口大小
    painter.setViewport(0, 0, width(), height());
    // 设置可以绘图的大小
    painter.setWindow(0, 0, width(), height());

    draw(&painter);
}

void DianVoteDrawer::draw(QPainter *painter)
{
    int FontSize = width() / FontSizeRatio;   // 计算字体大小
    QFont Font = QFont("Arial", FontSize, QFont::Bold, true);  // 设置字体

    DoWithCoordinate();

    QString replyNum = "Total: " + QString("%1").arg(replidDeviceNum);
    painter->setFont(Font);
    painter->drawText(RepliedDeviceNumX, RepliedDeviceNumY,
                      replyNum);

    int voterNums = histgram->GetVoterNums() > pie->GetVoterNums() ? \
                    histgram->GetVoterNums() : pie->GetVoterNums();
    QString totalVoteNum = "Voted: " + QString("%1").arg(voterNums);
    painter->setFont(Font);
    painter->drawText(TotalNumX, TotalNumY,
                      totalVoteNum);
}

void DianVoteDrawer::DoWithCoordinate()
{
    //-----question name coordiante----//
    RepliedDeviceNumX = width() * RatioRepliedDeviceX;
    RepliedDeviceNumY = height() * RatioRepliedDeviceY;
    //-----question name coordiante----//

    //-----total voter number coordinate----//
    TotalNumX = width() * RatioTotalNumX;
    TotalNumY = height() * RatioTotalNumY;
    //-----total voter number coordinate----//
}

void DianVoteDrawer::SetRepliedVoters(uint num)
{
    replidDeviceNum = num;
    update();
}

void DianVoteDrawer::SwithChart()
{
    if(histgram->isVisible())
    {
        histgram->hide();
        pie->show();
        showPie->hide();
        showHistgram->show();
    }
    else
    {
        pie->hide();
        histgram->show();
        showHistgram->hide();
        showPie->show();
    }
    update();
}
