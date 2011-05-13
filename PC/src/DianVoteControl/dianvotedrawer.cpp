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
}
