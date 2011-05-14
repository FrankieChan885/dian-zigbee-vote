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

    FontSizeRatio = 35;         // ���������С
    RatioRepliedDeviceX = 0.52;   // ����λ���봰�ڵĿ�ȱ���
    RatioRepliedDeviceY = 0.1;   //����λ���봰�ڵĸ߶ȶȱ���

    RatioTotalNumX = 0.75;       // ����λ���봰�ڵĿ�ȱ���
    RatioTotalNumY = 0.1;       // ����λ���봰�ڵĸ߶ȶȱ���
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

    // �����ӿ�Ϊ�������ڴ�С
    painter.setViewport(0, 0, width(), height());
    // ���ÿ��Ի�ͼ�Ĵ�С
    painter.setWindow(0, 0, width(), height());

    draw(&painter);
}

void DianVoteDrawer::draw(QPainter *painter)
{
    int FontSize = width() / FontSizeRatio;   // ���������С
    QFont Font = QFont("Arial", FontSize, QFont::Bold, true);  // ��������

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
