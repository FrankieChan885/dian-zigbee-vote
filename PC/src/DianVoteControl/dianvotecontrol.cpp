#include <QTime>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QString>
#include <QSplashScreen>
#include <QInputDialog>
#include <QLabel>
#include <QDesktopWidget>
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
    drawer(NULL),
    hidControl(NULL),
    stopWatch(NULL),
    curState(STOP)
{

    QDir dir;
    dir.setCurrent(QCoreApplication::applicationDirPath());

    // show splash.
    QPixmap pixmap(dir.absoluteFilePath("res/images/splash.png"));
    QSplashScreen *splash = new QSplashScreen(pixmap);
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

    pbOption = new QPushButton(this);
    pbOption->setObjectName(tr("pbOption"));
    ui->buttonLayout->addWidget(pbOption, 0, 3);

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
    connect(this, SIGNAL(setOptionNum(int)), drawer->histgram, SLOT(SetOptionNums(int)));
    connect(this, SIGNAL(clearDrawData()), drawer->histgram, SLOT(ClearData()));
    connect(this, SIGNAL(updateGraph(int)), drawer->histgram, SLOT(HandleData(int)));

    LoadStyleSheet("Default");

    // ��¼��ʼ�����ڴ�С
    initSize = this->size();
    this->setFixedWidth(this->width());
    this->setMaximumHeight(this->height() + 100);
    this->move(0, 0);

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

        int num0 = GetOptionNum();     // ��ȡѡ�����

        // �������, �ݼ�ģʽ
        ShowStopWatch();
        Q_ASSERT(stopWatch != NULL);
        stopWatch->setMode(STOP_WATCH_DECREASE_MODE);

        int num1 = GetLastTime();     // ��ȡ����ʱ��

        if(num1)
        {
            emit setLastTime(num1);;
        }
        else
        {
            return;
        }

        if(num0)
        {
            emit setOptionNum(num0);
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
    ClearLog(); // ������Σ������־���ݲ�����֮�Ǳ���ġ�

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
    for(int i = 0; i < log->length(); i++)
    {
        if((log->at(i)->id = id) && (log->at(i)->type == NORMAL))
        {
            rd->type = DUPLICATE;
            log->append(rd);
            return;
        }
    }
    emit updateGraph((int)rd->key - MAP_VALUE);  // ��������

    rd->type = NORMAL;
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

void DianVoteControl::mousePressEvent(QMouseEvent *event)
{
    // �رջ�ӭ����
    splash->finish(this);
    delete splash;

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

void DianVoteControl::ClearLog()
{
    // �ڴ˽���������д��log�ļ�

    log->clear();
}
