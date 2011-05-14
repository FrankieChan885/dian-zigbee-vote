#include <QFont>
#include <QTimer>
#include <QString>
#include <QPainter>
#include "dianvotedrawer.h"
#include "ui_dianvotedrawer.h"

DianVoteDrawer::DianVoteDrawer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DianVoteDrawer)
{
    ui->setupUi(this);
    connect(ui->Close_Drawer, SIGNAL(clicked()), this, SLOT(close()));

    histgram = new DrawHistgram();
    ui->DrawConvas->addWidget(histgram);

    timer = new QTimer();
    timer->setInterval(DEFAULT_UPDATE_TIMER);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start();

    FontSizeRatio = 35;         // 控制字体大小
    RatioQuestionNameX = 0.8;   // 总数位置与窗口的宽度比例
    RatioQuestionNameY = 0.1;   //总数位置与窗口的高度度比例

    RatioTotalNumX = 0.6;       // 总数位置与窗口的宽度比例
    RatioTotalNumY = 0.1;       // 总数位置与窗口的高度度比例
}

DianVoteDrawer::~DianVoteDrawer()
{

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

    QString totalNum = "Total Voter: " + QString("%1").arg(histgram->GetVoterNums());
    painter->setFont(Font);
    painter->drawText(TotalNumX, TotalNumY,
                      totalNum);
}

void DianVoteDrawer::DoWithCoordinate()
{
    //-----question name coordiante----//
    QuestionNameX = width() * RatioQuestionNameX;
    QuestionNameY = height() * RatioQuestionNameY;
    //-----question name coordiante----//

    //-----total voter number coordinate----//
    TotalNumX = width() * RatioTotalNumX;
    TotalNumY = height() * RatioTotalNumY;
    //-----total voter number coordinate----//
}
