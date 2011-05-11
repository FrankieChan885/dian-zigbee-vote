#include <QTime>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include <QByteArray>
#include <QMouseEvent>
#include <QPushButton>
#include <QMessageBox>
#include <QSpacerItem>
#include "ui_dianvotecontrol.h"
#include "dianvotecontrol.h"
#include "exceptions.h"
#include "hidcontrol.h"
#include "stopwatch.h"

#ifdef WIN32
    #include "qtwin.h"
#endif

DianVoteControl::DianVoteControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DianVoteControl),
    curState(STOP)
{
    ui->setupUi(this);

    pbStart = new QPushButton(this);
    pbStart->setText("Star");
    ui->buttonLayout->addWidget(pbStart, 0, 0);
    pbAuto = new QPushButton(this);
    pbAuto->setText("Auto");
    ui->buttonLayout->addWidget(pbAuto, 0, 1);
    pbPause = new QPushButton(this);
    pbPause->setText("Paus");
    ui->buttonLayout->addWidget(pbPause, 0, 0);
    pbPause->hide();
    pbStop = new QPushButton(this);
    pbStop->setText("Stop");
    ui->buttonLayout->addWidget(pbStop, 0, 1);
    pbStop->hide();
    pbResult = new QPushButton(this);
    pbResult->setText("Resu");
    ui->buttonLayout->addWidget(pbResult, 0, 2);
    pbOption = new QPushButton(this);
    pbOption->setText("Opti");
    ui->buttonLayout->addWidget(pbOption, 0, 3);
    pbClose = new QPushButton(this);
    pbClose->setText("Clos");
    ui->buttonLayout->addWidget(pbClose, 0, 4);

    connect(pbClose, SIGNAL(clicked()), this, SLOT(close()));
    connect(pbStart, SIGNAL(clicked()), this, SLOT(VoteStart()));
    connect(pbAuto, SIGNAL(clicked()), this, SLOT(VoteAuto()));
    connect(pbPause, SIGNAL(clicked()), this, SLOT(VotePause()));
    connect(pbStop, SIGNAL(clicked()), this, SLOT(VoteStop()));
    connect(pbResult, SIGNAL(clicked()), this, SLOT(DoShowResults()));

    LoadStyleSheet("Coffee");

    log = new QList< RevData* >();
}

DianVoteControl::~DianVoteControl()
{
    delete ui;

    for(int i = 0; i < log->length(); i++)
    {
        if((*log)[i]->revTime)
        {
            delete (*log)[i]->revTime;
        }
        if((*log)[i])
        {
            delete (*log)[i];
        }
    }
    delete log;
}

void DianVoteControl::VoteStart()
{
    if(curState == RUNNING)
    {
        QMessageBox::critical(0, "warn", "Already Started");
    }else if(curState == PAUSE)
    {
        // �޸�������
        pbStart->hide();
        pbPause->show();

        // �������
        stopWatch->start();
    }
    else if(curState == STOP)
    {
        PrepareHid();

        // �޸�������
        pbStart->hide();
        pbAuto->hide();
        pbPause->show();
        pbStop->show();

        // �������, ����ģʽ
        ShowStopWatch();
        Q_ASSERT(stopWatch != NULL);
        stopWatch->setMode(STOP_WATCH_INCREASE_MODE);
        stopWatch->start();
    }

    // �ı�״̬
    curState = RUNNING;
}

void DianVoteControl::VoteAuto()
{
    if(curState == RUNNING)
    {
        QMessageBox::critical(0, "warn", "Already Started");
    }else if(curState == PAUSE)
    {
        // �޸�������
        pbStart->hide();
        pbPause->show();

        // �������
        stopWatch->start();
    }
    else if(curState == STOP)
    {
        PrepareHid();

        // �޸�������
        pbStart->hide();
        pbAuto->hide();
        pbPause->show();
        pbStop->show();

        // �������, ����ģʽ
        ShowStopWatch();
        Q_ASSERT(stopWatch != NULL);
        stopWatch->setMode(STOP_WATCH_DECREASE_MODE);
        stopWatch->SetStartTime(60);//for test
        stopWatch->start();
    }

    // �ı�״̬
    curState = RUNNING;
}

void DianVoteControl::VotePause()
{
    Q_ASSERT(stopWatch != NULL);
//    Q_ASSERT(hidControl != NULL);
    stopWatch->pause();
//    hidControl->stop();

    // �޸�������
    pbPause->hide();
    pbStart->show();

    // �޸�״̬
    curState = PAUSE;
}

void DianVoteControl::VoteStop()
{
    Q_ASSERT(stopWatch != NULL);
//    Q_ASSERT(hidControl != NULL);
    stopWatch->stop();
//    hidControl->stop();

    // �޸�������
    HideStopWatch();
    pbPause->hide();
    pbStop->hide();
    pbStart->show();
    pbAuto->show();

    // �޸�״̬
    curState = STOP;
}

void DianVoteControl::DoShowResults()
{
    drawer = new DianVoteDrawer();

#ifdef WIN32
    // ����aeroЧ��
    if (QtWin::isCompositionEnabled()) {
        QtWin::extendFrameIntoClientArea(drawer);
        drawer->setContentsMargins(0, 0, 0, 0);
    }
#endif // #ifdef WIN32

    drawer->setAttribute(Qt::WA_DeleteOnClose);
    connect(this, SIGNAL(updateGraph(int)), drawer->histgram, SLOT(HandleData(int)));

    drawer->show();
}

void DianVoteControl::DoShowStatics()
{

}

void DianVoteControl::ParseData(quint32 id, quint8 option)
{
    RevData *rd = new RevData;
    rd->key = option;       // ȡ�����һ���ֽ�
    emit updateGraph((int)rd->key - MAP_VALUE);  // ��������

	// ȡ��ID
	rd->id = id;

    // ����ʱ��
    QString *time;
    time = new QString();
    *time = QTime::currentTime().toString("hh:mm:ss");
    rd->revTime = time;
    log->append(rd);
}

void DianVoteControl::PrepareHid()
{
    try
    {
        hidControl = new HidControl(this);
        connect(hidControl, SIGNAL(voteComing(quint32, quint8)),
                this, SLOT(ParseData(quint32, quint8)));
        // start all remote.
        hidControl->start();
    }
    catch(DianVoteStdException *e)
    {
        QMessageBox::critical(0, "error", e->what());
    }
    catch(...)
    {
        QMessageBox::critical(0, "error", "unknow exception.");
    }
}

void DianVoteControl::LoadStyleSheet(const QString &sheetName)
{
    QDir dir(QCoreApplication::applicationDirPath());
    dir.setCurrent(QCoreApplication::applicationDirPath());
    QString qss = "res/skins/" + sheetName.toLower() + ".qss";
    QFile file(dir.absoluteFilePath(qss));
    if (!file.open(QFile::ReadOnly))
    {
        QMessageBox::critical(0, "error", sheetName + " Not Find. Use Default Style");
        return;
    }
    QString styleSheet = QLatin1String(file.readAll());

    qApp->setStyleSheet(styleSheet);
}

void DianVoteControl::ShowStopWatch()
{
    animationGroup = new QSequentialAnimationGroup(this);

    resizeAnimation = new QPropertyAnimation(this, "geometry");
    resizeAnimation->setDuration(1000);
    resizeAnimation->setStartValue(QRect(0, 0, width(), height()));
    resizeAnimation->setEndValue(QRect(0, 0, width(), height() + 100));

    stopWatch = new StopWatch(this);
    connect(stopWatch, SIGNAL(autoStop()), this, SLOT(VoteStop()));
    ui->stopWatchLayout->addWidget(stopWatch);

//    showStopWatchAnimation = new QPropertyAnimation(stopWatch, "geometry");
//    showStopWatchAnimation->setDuration(1500);
//    showStopWatchAnimation->setStartValue(QRect(0, 0, width(), 0));
//    showStopWatchAnimation->setEndValue(QRect(0, height(), width(), 100));

    animationGroup->addAnimation(resizeAnimation);
//    animationGroup->addAnimation(showStopWatchAnimation);
    animationGroup->start();
}

void DianVoteControl::HideStopWatch()
{
    // show�������
    resizeAnimation->setStartValue(QRect(0, 0, width(), height()));
    resizeAnimation->setEndValue(QRect(0, 0, width(), height() - 100));
    connect(resizeAnimation, SIGNAL(finished()), this, SLOT(DoHideStopWatch()));
    animationGroup->start();

    if (stopWatch == NULL)
    {
        return;
    }
}

void DianVoteControl::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void DianVoteControl::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

//void DianVoteControl::DoShowStopWatch()
//{

//}

void DianVoteControl::DoHideStopWatch()
{
    delete stopWatch;
}
