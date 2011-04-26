#include "dianvotedrawer.h"
#include "ui_dianvotedrawer.h"

DianVoteDrawer::DianVoteDrawer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DianVoteDrawer)
{
    ui->setupUi(this);
    connect(ui->Close, SIGNAL(clicked()), this, SLOT(close()));

    histgram = new Histgram();
    ui->DrawConvas->addWidget(histgram);

    // 给正确答案按钮添加Actions
    QAction *showCorrectAnswer = new QAction("Show Answer", this);
    showCorrectAnswer->setCheckable(true);
    ui->CorrectAnswer->addAction(showCorrectAnswer);
    connect(showCorrectAnswer, SIGNAL(triggered(bool)), histgram, SLOT(setUsetCorrectAnswer(bool)));

    signalMapper = new QSignalMapper(this);
    for (int i = 0; i < histgram->getColumnNums(); i++)
    {
        QAction *action = new QAction(histgram->getOptionName(i), this);
        connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
        signalMapper->setMapping(action, action->text());
        ui->CorrectAnswer->addAction(action);
    }
    connect(signalMapper, SIGNAL(mapped(QString)), SIGNAL(CorretAnswer(QString)));
    connect(this, SIGNAL(CorretAnswer(QString)), histgram, SLOT(setCorrectAnswer(QString)));
    connect(ui->CorrectAnswer, SIGNAL(clicked()), histgram, SLOT(setUsetCorrectAnswer()));

    // 给设置按钮添加Actions
    dashLine = new QAction("Show DashLine", this);
    dashLine->setCheckable(true);
    dashLine->setChecked(true);
    SolidPattern = new QAction("SolidPattern", this);
    Dense5Pattern = new QAction("Dense5Pattern", this);
    CrossPattern = new QAction("CrossPattern", this);
    ui->Options->addAction(dashLine);
    ui->Options->addAction(SolidPattern);
    ui->Options->addAction(Dense5Pattern);
    ui->Options->addAction(CrossPattern);

    connect(dashLine, SIGNAL(triggered()), histgram, SLOT(setyAxisDashLineFlag()));
    connect(SolidPattern, SIGNAL(triggered()), histgram, SLOT(setSolidPatternStyle()));
    connect(Dense5Pattern, SIGNAL(triggered()), histgram, SLOT(setDense5PatternStyle()));
    connect(CrossPattern, SIGNAL(triggered()), histgram, SLOT(setCrossPatternStyle()));

    pie = new Pie();
    ui->DrawConvas->addWidget(pie);
    pie->hide();
}

DianVoteDrawer::~DianVoteDrawer()
{
    delete ui;
}
