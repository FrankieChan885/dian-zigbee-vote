#include <QtGui>
#include "ui_dianvotecontrol.h"
#include "dianvotecontrol.h"
#include "exceptions.h"
#include "hidcontrol.h"
#include "stopwatch.h"
#include "drawhistgram.h"
#include "drawpie.h"
#include "dianvotedebug.h"

#ifdef WIN32
    #include "qtwin.h"
#endif

QFile* DianVoteControl::VoteLog = new QFile(tr("Votelog.log"));

DianVoteControl::DianVoteControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DianVoteControl),
    drawer(NULL),
    hidControl(NULL),
    stopWatch(NULL),
    splash(NULL),
    voteMode(SINGLE_VOTE),
    curState(STOP)
{
    QDir dir;
    dir.setCurrent(QCoreApplication::applicationDirPath());

    windowIcon = new QIcon(dir.absoluteFilePath("res/images/app-icon.png"));
    this->setWindowIcon(*windowIcon);

    // show splash.
    QPixmap pixmap(dir.absoluteFilePath("res/images/logo.png"));
    QSplashScreen *splash = new QSplashScreen(pixmap);
    splash->setWindowIcon(*windowIcon);
    splash->setWindowFlags(Qt::WindowStaysOnTopHint);
    splash->setMask(pixmap.mask());
    splash->show();

    ui->setupUi(this);

    pbStart = new QPushButton(this);
    pbStart->setObjectName(tr("pbStart"));
    ui->buttonLayout->addWidget(pbStart, 0, 0);

    pbAuto = new QPushButton(this);
    pbAuto->setObjectName(tr("pbAuto"));
    ui->buttonLayout->addWidget(pbAuto, 0, 1);

    pbPause = new QPushButton(this);
    pbPause->setObjectName(tr("pbPause"));
    ui->buttonLayout->addWidget(pbPause, 0, 0);
    pbPause->hide();

    pbStop = new QPushButton(this);
    pbStop->setObjectName(tr("pbStop"));
    ui->buttonLayout->addWidget(pbStop, 0, 1);
    pbStop->hide();

    pbResult = new QPushButton(this);
    pbResult->setObjectName(tr("pbResult"));
    ui->buttonLayout->addWidget(pbResult, 0, 2);

    pbOption = new QToolButton(this);
    pbOption->setObjectName(tr("pbOption"));
    ui->buttonLayout->addWidget(pbOption, 0, 3);

    qaSingleMode = new QAction(tr("SingleVoteMode"), this);  // ��ѡģʽ
    qaSingleMode->setCheckable(true);
    qaSingleMode->setChecked(true);         // Ĭ��ģʽ
    qaMutipleMode = new QAction(tr("MutipleVoteMode"), this); // ��ѡ
    qaMutipleMode->setCheckable(true);
    qaRaceMode = new QAction(tr("RaceVoteMode"), this);    // ����
    qaRaceMode->setCheckable(true);
    pbOption->addAction(qaSingleMode);
    pbOption->addAction(qaMutipleMode);
    pbOption->addAction(qaRaceMode);
    connect(qaSingleMode, SIGNAL(triggered()), this, SLOT(DoSingleMode()));
    connect(qaMutipleMode, SIGNAL(triggered()), this, SLOT(DoMutipleMode()));
    connect(qaRaceMode, SIGNAL(triggered()), this, SLOT(DoRaceVoteMode()));

    pbClose = new QPushButton(this);
    pbClose->setObjectName(tr("pbClose"));
    ui->buttonLayout->addWidget(pbClose, 0, 4);

    connect(pbClose, SIGNAL(clicked()), this, SLOT(close()));
    connect(pbStart, SIGNAL(clicked()), this, SLOT(VoteStart()));
    connect(pbAuto, SIGNAL(clicked()), this, SLOT(VoteAuto()));
    connect(pbPause, SIGNAL(clicked()), this, SLOT(VotePause()));
    connect(pbStop, SIGNAL(clicked()), this, SLOT(VoteStop()));
    connect(pbResult, SIGNAL(clicked()), this, SLOT(DoShowResults()));

    drawer = new DianVoteDrawer();
    drawer->setWindowIcon(*windowIcon);
    drawer->setWindowTitle(tr("Result"));
    connect(pbClose, SIGNAL(clicked()), this->drawer, SLOT(close()));
    connect(this, SIGNAL(setOptionNum(int)), drawer->histgram, SLOT(SetOptionNums(int)));
    connect(this, SIGNAL(setOptionNum(int)), drawer->pie, SLOT(SetOptionNums(int)));
    connect(this, SIGNAL(clearDrawData()), drawer->histgram, SLOT(ClearData()));
    connect(this, SIGNAL(clearDrawData()), drawer->pie, SLOT(ClearData()));
    connect(this, SIGNAL(updateGraph(int)), drawer->histgram, SLOT(HandleData(int)));
    connect(this, SIGNAL(updateGraph(int)), drawer->pie, SLOT(HandleData(int)));

    LoadStyleSheet("Default");

    // ��¼��ʼ�����ڴ�С
    initSize = this->size();
    this->setMaximumWidth(this->width());
    this->setMaximumHeight(this->height() + 100);
    this->move(0, 0);

    // ����Log�ļ������򿪣������˳��ǹر�
    VoteLog->open(QIODevice::WriteOnly | QIODevice::Append);

    // ��ʼ��log��¼����
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

    VoteLog->close();
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

//      emit clearDrawData();   // ע�⣬��һ��һ��Ҫ��GetOptionNum֮ǰ

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
        pbOption->setEnabled(false);

        // �������, ����ģʽ
        ShowStopWatch();
        Q_ASSERT(stopWatch != NULL);
        stopWatch->setMode(STOP_WATCH_INCREASE_MODE);
        stopWatch->start();
        resizeAnimation->start();
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
    }
    else if(curState == PAUSE)
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

//      emit clearDrawData();   // ע�⣬��һ��һ��Ҫ��GetOptionNum֮ǰ

        int num1 = GetLastTime();     // ��ȡ����ʱ��

        if(!num1)
        {
            return;
        }

        int num0 = GetOptionNum();     // ��ȡѡ�����
        if(!num0)
        {
            return;
        }

        // �������, �ݼ�ģʽ
        ShowStopWatch();
        Q_ASSERT(stopWatch != NULL);
        stopWatch->setMode(STOP_WATCH_DECREASE_MODE);

        emit setLastTime(num1);;
        emit setOptionNum(num0);

        StartHid();     // ���������豸

        // �޸�������
        pbStart->hide();
        pbAuto->hide();
        pbPause->show();
        pbStop->show();
        pbOption->setEnabled(false);

        stopWatch->start();
        resizeAnimation->start();
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
    ClearLogList(); // ������Σ������־�����Ǳ���ġ�

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

    pbOption->setEnabled(true);
    resizeAnimation->start();
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
    rd->id = id;            // ȡ��ID

    QString *time;          // ����ʱ��
    time = new QString();
    *time = QTime::currentTime().toString("hh:mm:ss");
    rd->revTime = time;

    // �ݴ��Լ�飬����յ��Ѿ����͹��������ֳֶ�
    //      ���϶������ݣ�����д��log��
#ifndef TEST_RECEIVE_DATA_CONTINUE
    for(int i = 0; i < log->length(); i++)
    {
        if((log->at(i)->id == id) && (log->at(i)->type == NORMAL))
        {
            rd->type = DUPLICATE;
            log->append(rd);
            WriteRevDataLog(rd);
            return;
        }
    }
#endif
    emit updateGraph((int)rd->key - MAP_VALUE);  // ��������

    rd->type = NORMAL;
    log->append(rd);
    WriteRevDataLog(rd);
}

int DianVoteControl::GetOptionNum()
{
    bool ok;
    getOptionNum = new QInputDialog();
    getOptionNum->setAttribute(Qt::WA_DeleteOnClose);
    int i = QInputDialog::getInt(this,
                                 tr("Get Options Amounts"),
                                 tr("Options Amounts"),
                                 DEFAULT_OPTION_NUM,
                                 0,
                                 MAXIMUN_OPTION_NUM,
                                 1,
                                 &ok);
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
    int i = QInputDialog::getInt(this,
                                 tr("Get Last Time"),
                                 tr("Last Time"),
                                 DEFAULT_DEADLINE,
                                 0,
                                 MAXIMUN_DEADLINE,
                                 1,
                                 &ok);
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
        if(!hidControl)
        {
            hidControl = new HidControl(this);

#ifdef DO_ROLL_CALL
            connect(hidControl, SIGNAL(rollCallFinished(uint)),
                    drawer, SLOT(SetRepliedDeviceNum(uint)));
            hidControl->startRollCall();
#endif  // end ifdef
        }
        // for test, comment this line
#ifndef TEST_RECEIVE_DATA_CONTINUE
        hidControl->setStopOnReceive(true);
#endif  // end ifndef
        connect(hidControl, SIGNAL(voteComing(quint32, quint8)),
                this, SLOT(ParseData(quint32, quint8)));
        if(voteMode == RACE_VOTE)
        {
            connect(hidControl, SIGNAL(voteComing(quint32, quint8)),
                    this, SLOT(ShowRaceVoteWinner(quint32,quint8)));
        }

        return true;
    }
    catch(DianVoteStdException *e)
    {
        QMessageBox::critical(0, "Hid Open Failed", e->what());
        return false;
    }
    catch(...)
    {
        QMessageBox::critical(0, "Hid Open Failed", "unknow exception.");
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
    // �������߶�ʮ�ˣ��ȰѴ������ó�Ĭ�ϵĴ�С
//    QPoint curPos = this->pos();
    this->resize(initSize);
    QSize endSize(initSize);
    endSize.setHeight(initSize.height() + 100);

    resizeAnimation = new QPropertyAnimation(this, "size");
    resizeAnimation->setDuration(1000);
    resizeAnimation->setStartValue(initSize);
    resizeAnimation->setEndValue(endSize);

    if(!stopWatch)
    {
        stopWatch = new StopWatch(this);
    }
    connect(this, SIGNAL(setLastTime(int)), stopWatch, SLOT(SetStartTime(int)));
    connect(stopWatch, SIGNAL(autoStop()), this, SLOT(VoteStop()));
    ui->stopWatchLayout->addWidget(stopWatch);
}

void DianVoteControl::HideStopWatch()
{
    // show�������
    QSize curSize = this->size();
    resizeAnimation->setStartValue(curSize);
    resizeAnimation->setEndValue(initSize);
    connect(resizeAnimation, SIGNAL(finished()), this, SLOT(DoHideStopWatch()));
}


void DianVoteControl::ShowRaceVoteWinner(quint32 id, quint8 key)
{
    // �ȹر����е��豸���ڴ򿪱�ѡ�е��豸
    VoteStop();
    hidControl->start(id);

    disconnect(hidControl, SIGNAL(voteComing(quint32, quint8)),
            this, SLOT(ShowRaceVoteWinner(quint32,quint8)));

    raceWinner = new QDialog();
    raceWinner->setAttribute(Qt::WA_DeleteOnClose);
    raceWinner->setWindowIcon(*windowIcon);
    raceWinner->setWindowTitle(tr("Winner~!"));
    QGridLayout *mainLayout = new QGridLayout(raceWinner);
    QLabel *winner = new QLabel(raceWinner);
    QString winnerID = QString("0x%L1").arg(id, 8, 16, QChar('0'));
    QString Text = "Winner: " + winnerID;
    QFont Font = QFont("Arial", 15, QFont::Bold, true);
    winner->setFont(Font);
    winner->setText(Text);
    mainLayout->addWidget(winner, 0, 0, 1, 2);
    QPushButton *close = new QPushButton(raceWinner);
    mainLayout->addWidget(close, 1, 1);
    connect(close, SIGNAL(clicked()), raceWinner, SLOT(close()));
    connect(close, SIGNAL(clicked()), this, SLOT(CloseRaceVoteWinner()));
    close->setText(tr("Close"));
    raceWinner->setLayout(mainLayout);

    raceWinner->resize(200, 200);
    raceWinner->setFixedSize(this->width(), this->height());
    raceWinner->show();
}

void DianVoteControl::CloseRaceVoteWinner()
{
    hidControl->stop();
}

void DianVoteControl::mousePressEvent(QMouseEvent *event)
{
    // �رջ�ӭ����
    if(splash != NULL)
    {
        splash->finish(this);;
        delete splash;
        splash = NULL;
    }

    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void DianVoteControl::mouseMoveEvent(QMouseEvent *event)
{
    QDesktopWidget* desktopWidget = QApplication::desktop();
    //��ȡ�豸��Ļ��С
    int screenWidth = desktopWidget->screenGeometry().width();
    int screenHeight = desktopWidget->screenGeometry().height();

    if (event->buttons() & Qt::LeftButton) {
        QPoint pos = event->globalPos() - dragPosition;
        if((pos.x() <= DEFAULT_DOCK_SPACE))
        {
            pos.setX(0);
        }
        else if((pos.x() + this->width() + DEFAULT_DOCK_SPACE) \
                 >= screenWidth)
        {
            pos.setX(screenWidth - this->width() - 2);
        }

        if((pos.y() <= DEFAULT_DOCK_SPACE))
        {
            pos.setY(0);
        }
        else if((pos.y() + this->height() + DEFAULT_DOCK_SPACE) \
                >= screenHeight)
        {
            pos.setY(screenHeight - this->height() - 2);
        }

        move(pos);
        event->accept();
    }
}

void DianVoteControl::DoHideStopWatch()
{
    delete stopWatch;
    stopWatch = NULL;
}

void DianVoteControl::DianVoteMsgHandler(QtMsgType type, const char *msg)
{
    QString log;
    switch (type) {
        case QtDebugMsg:
        {
            log = QString("%1").arg(msg);
            break;
        }

        case QtWarningMsg:
        {
            log = QString("Warning: %1").arg(msg);
            break;
        }

        case QtCriticalMsg:
        {
            log = QString("Critical: %1").arg(msg);
            break;
        }

        case QtFatalMsg:
        {
            log = QString("Fatal: %1").arg(msg);
            abort();
        }
    }

    QTextStream ts(VoteLog);
    ts << log << endl;
}

void DianVoteControl::WriteRevDataLog(RevData *rd)
{
    QString log(*(rd->revTime) + " : ");
    log += "Device ID: ";
    log += QString("%1").arg(rd->id) + "\t";
    log += "Key: ";
    log += QString("%1").arg(rd->key) + "\t";
    log += "Type: ";
    log += QString("%1").arg(rd->type) + "\t";
    log += "Received";

    qDebug(log.toAscii().data());
}

void DianVoteControl::ClearLogList()
{
    log->clear();
}

void DianVoteControl::DoRaceVoteMode()
{
    voteMode = RACE_VOTE;
    // ����Action��check״̬
    qaRaceMode->setChecked(true);
    qaMutipleMode->setChecked(false);
    qaSingleMode->setChecked(false);
}

void DianVoteControl::DoSingleMode()
{
    voteMode = SINGLE_VOTE;
    // ����Action��check״̬
    qaRaceMode->setChecked(false);
    qaMutipleMode->setChecked(false);
    qaSingleMode->setChecked(true);
}

void DianVoteControl::DoMutipleMode()
{
    voteMode = MULTIPLE_VOTE;
    // ����Action��check״̬
    qaRaceMode->setChecked(false);
    qaMutipleMode->setChecked(true);
    qaSingleMode->setChecked(false);
}
