#include <QTime>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QInputDialog>
#include <QLabel>
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

    drawer = new DianVoteDrawer();
    connect(this, SIGNAL(setOptionNum(int)), drawer->histgram, SLOT(SetOptionNums(int)));
    connect(this, SIGNAL(clearDrawData()), drawer->histgram, SLOT(ClearData()));
    connect(this, SIGNAL(updateGraph(int)), drawer->histgram, SLOT(HandleData(int)));

    LoadStyleSheet("Default");

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

        StartHid();     // ���������豸
    }
    else if(curState == STOP)
    {
        // ������hid��
        if(!PrepareHid())
        {
            // �Ѵ�����Ϣд��log�ļ�
            return;
        }

        emit clearDrawData();   // ע�⣬��һ��һ��Ҫ��GetOptionNum֮ǰ

        int num = GetOptionNum();
        if(num)
        {
            emit setOptionNum(num);     // ��ȡѡ�����
        }
        else
        {
            return;
        }

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
        animationGroup->start();
        StartHid();     // ���������豸
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

        StartHid();     // ���������豸
    }
    else if(curState == STOP)
    {
        // ������hid��
        if(!PrepareHid())
        {
            // �Ѵ�����Ϣд��log�ļ�
            return;
        }

        emit clearDrawData();   // ע�⣬��һ��һ��Ҫ��GetOptionNum֮ǰ

        int num = GetOptionNum();     // ��ȡѡ�����
        if(num)
        {
            emit setOptionNum(num);
        }
        else
        {
            return;
        }

        // �������, �ݼ�ģʽ
        ShowStopWatch();
        Q_ASSERT(stopWatch != NULL);
        stopWatch->setMode(STOP_WATCH_DECREASE_MODE);

        num = GetLastTime();     // ��ȡ����ʱ��
        if(num)
        {
            emit setLastTime(num);;
        }
        else
        {
            return;
        }

        StartHid();     // ���������豸

        // �޸�������
        pbStart->hide();
        pbAuto->hide();
        pbPause->show();
        pbStop->show();

        stopWatch->start();
        animationGroup->start();
    }

    // �ı�״̬
    curState = RUNNING;
}

void DianVoteControl::VotePause()
{
    if(curState == RUNNING)
    {
        Q_ASSERT(stopWatch != NULL);
        stopWatch->pause();

        // �޸�������
        pbPause->hide();
        pbStart->show();

        // ֹͣUSB�豸
        if(hidControl == NULL)
        {
            QMessageBox::critical(0, "Internal Error", "Can't Stop HidDevice");
            return;
        }
        StopHid();  // ֹͣ
    }

    // �޸�״̬
    curState = PAUSE;
}

void DianVoteControl::VoteStop()
{
    if(curState == RUNNING)
    {
        Q_ASSERT(stopWatch != NULL);
        stopWatch->stop();

        // �޸�������
        HideStopWatch();
        pbPause->hide();
        pbStop->hide();
        pbStart->show();
        pbAuto->show();

        // ֹͣUSB�豸
        if(hidControl == NULL)
        {
            QMessageBox::critical(0, "Internal Error", "Can't Stop HidDevice");
            return;
        }
        StopHid();  // ֹͣ
    }else if(curState == PAUSE)
    {
        Q_ASSERT(stopWatch != NULL);
        stopWatch->stop();

        // �޸�������
        HideStopWatch();
        pbPause->hide();
        pbStop->hide();
        pbStart->show();
        pbAuto->show();

        // ��Ϊpause״̬hidCntrol�Ѿ�ֹͣ�����Բ���Ҫֹͣ
    }

    animationGroup->start();
    // �޸�״̬
    curState = STOP;
}

void DianVoteControl::DoShowResults()
{

#ifdef WIN32
    // ����aeroЧ��
    if (QtWin::isCompositionEnabled()) {
        QtWin::extendFrameIntoClientArea(drawer);
        drawer->setContentsMargins(0, 0, 0, 0);
    }
#endif // #ifdef WIN32

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

int DianVoteControl::GetOptionNum()
{
    bool ok;
    getOptionNum = new QInputDialog();
    getOptionNum->setAttribute(Qt::WA_DeleteOnClose);
    int i = QInputDialog::getInt(this, tr("Get Options Amounts"),
                                 tr("Options Amounts"), 5, 0, 10, 1, &ok);
    if (ok)
    {
        return i;
    }
    else
    {
        return 0;
    }
}

int DianVoteControl::GetLastTime()
{
    bool ok;
    getLastTime = new QInputDialog();
    getLastTime->setAttribute(Qt::WA_DeleteOnClose);
    int i = QInputDialog::getInt(this, tr("Get Last Time"),
                                 tr("Last Time"), 60, 0, 1000, 1, &ok);
    if (ok)
    {
        return i;
    }
    else
    {
        return 0;
    }
}

bool DianVoteControl::PrepareHid()
{
    try
    {
        hidControl = new HidControl(this);
        connect(hidControl, SIGNAL(voteComing(quint32, quint8)),
                this, SLOT(ParseData(quint32, quint8)));

        return true;
    }
    catch(DianVoteStdException *e)
    {
        QMessageBox::critical(0, "error", e->what());
        return false;
    }
    catch(...)
    {
        QMessageBox::critical(0, "error", "unknow exception.");
        return false;
    }
}

bool DianVoteControl::StartHid()
{
    try
    {
        hidControl->start();
        // start all remote.
        return true;
    }
    catch(DianVoteStdException *e)
    {
        QMessageBox::critical(0, "error", e->what());
        return false;
    }
    catch(...)
    {
        QMessageBox::critical(0, "error", "unknow exception.");
        return false;
    }
}

bool DianVoteControl::StopHid()
{
    try
    {
        hidControl->stop();;
        // start all remote.
        return true;
    }
    catch(DianVoteStdException *e)
    {
        QMessageBox::critical(0, "error", e->what());
        return false;
    }
    catch(...)
    {
        QMessageBox::critical(0, "error", "unknow exception.");
        return false;
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
    connect(this, SIGNAL(setLastTime(int)), stopWatch, SLOT(SetStartTime(int)));
    connect(stopWatch, SIGNAL(autoStop()), this, SLOT(VoteStop()));
    ui->stopWatchLayout->addWidget(stopWatch);

//    showStopWatchAnimation = new QPropertyAnimation(stopWatch, "geometry");
//    showStopWatchAnimation->setDuration(1500);
//    showStopWatchAnimation->setStartValue(QRect(0, 0, width(), 0));
//    showStopWatchAnimation->setEndValue(QRect(0, height(), width(), 100));

    animationGroup->addAnimation(resizeAnimation);
//    animationGroup->addAnimation(showStopWatchAnimation);
}

void DianVoteControl::HideStopWatch()
{
    // show�������
    resizeAnimation->setStartValue(QRect(0, 0, width(), height()));
    resizeAnimation->setEndValue(QRect(0, 0, width(), height() - 100));
    connect(resizeAnimation, SIGNAL(finished()), this, SLOT(DoHideStopWatch()));
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
