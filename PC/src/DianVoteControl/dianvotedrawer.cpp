#include <QFont>
#include <QTimer>
#include <QString>
#include <QPainter>
#include "dianvotedrawer.h"
#include "ui_dianvotedrawer.h"

DianVoteDrawer::DianVoteDrawer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DianVoteDrawer),
    replidDeviceNum(0)
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
    RatioRepliedDeviceX = 0.52;   // 总数位置与窗口的宽度比例
    RatioRepliedDeviceY = 0.1;   //总数位置与窗口的高度度比例

    RatioTotalNumX = 0.75;       // 总数位置与窗口的宽度比例
    RatioTotalNumY = 0.1;       // 总数位置与窗口的高度度比例
}

DianVoteDrawer::~DianVoteDrawer()
{

}

#ifdef DO_ROLL_CALL
void DianVoteDrawer::SetRepliedDeviceNum(uint replyNum)
{
    replidDeviceNum = replyNum;
    update();
}
#endif      // end ifdef

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

#ifdef DO_ROLL_CALL
    QString replyNum = "Total: " + QString("%1").arg(replidDeviceNum);
    painter->setFont(Font);
    painter->drawText(RepliedDeviceNumX, RepliedDeviceNumY,
                      replyNum);
#endif  // end ifdef

    QString totalVoteNum = "Voted: " + QString("%1").arg(histgram->GetVoterNums());
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
