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

    FontSizeRatio = 35;         // ���������С
    RatioQuestionNameX = 0.8;   // ����λ���봰�ڵĿ�ȱ���
    RatioQuestionNameY = 0.1;   //����λ���봰�ڵĸ߶ȶȱ���

    RatioTotalNumX = 0.6;       // ����λ���봰�ڵĿ�ȱ���
    RatioTotalNumY = 0.1;       // ����λ���봰�ڵĸ߶ȶȱ���
}

DianVoteDrawer::~DianVoteDrawer()
{

}

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
