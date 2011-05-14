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

    // 记录初始化窗口大小
    initSize = this->size();
    this->setFixedWidth(this->width());
    this->setMaximumHeight(this->height() + 100);
    this->move(0, 0);

    // 初始化log记录链表
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
        // 修改主界面
        pbStart->hide();
        pbPause->show();

        // 启动秒表
        stopWatch->start();

        StartHid();     // 开启接收设备
    }
    else if(curState == STOP)
    {
        // 这里会把hid打开
        if(!PrepareHid())
        {
            // 把错误信息写入log文件
            return;
        }

        emit clearDrawData();   // 注意，这一步一定要在GetOptionNum之前

        int num = GetOptionNum();
        if(num)
        {
            emit setOptionNum(num);     // 获取选项个数
        }
        else
        {
            return;
        }

        // 修改主界面
        pbStart->hide();
        pbAuto->hide();
        pbPause->show();
        pbStop->show();

        // 画出秒表, 递增模式
        ShowStopWatch();
        Q_ASSERT(stopWatch != NULL);
        stopWatch->setMode(STOP_WATCH_INCREASE_MODE);
        stopWatch->start();
        resizeAnimation->start();
        StartHid();     // 开启接收设备
    }

    // 改变状态
    curState = RUNNING;
}

void DianVoteControl::VoteAuto()
{
    if(curState == RUNNING)
    {
        QMessageBox::critical(0, "warn", "Already Started");
    }else if(curState == PAUSE)
    {
        // 修改主界面
        pbStart->hide();
        pbPause->show();

        // 启动秒表
        stopWatch->start();

        StartHid();     // 开启接收设备
    }
    else if(curState == STOP)
    {
        // 这里会把hid打开
        if(!PrepareHid())
        {
            // 把错误信息写入log文件
            return;
        }

        emit clearDrawData();   // 注意，这一步一定要在GetOptionNum之前

        int num0 = GetOptionNum();     // 获取选项个数

        // 画出秒表, 递减模式
        ShowStopWatch();
        Q_ASSERT(stopWatch != NULL);
        stopWatch->setMode(STOP_WATCH_DECREASE_MODE);

        int num1 = GetLastTime();     // 获取持续时间

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

        StartHid();     // 开启接收设备

        // 修改主界面
        pbStart->hide();
        pbAuto->hide();
        pbPause->show();
        pbStop->show();

        stopWatch->start();
        resizeAnimation->start();
    }

    // 改变状态
    curState = RUNNING;
}

void DianVoteControl::VotePause()
{
    if(curState == RUNNING)
    {
        Q_ASSERT(stopWatch != NULL);
        stopWatch->pause();

        // 修改主界面
        pbPause->hide();
        pbStart->show();

        // 停止USB设备
        if(hidControl == NULL)
        {
            QMessageBox::critical(0, "Internal Error", "Can't Stop HidDevice");
            return;
        }
        StopHid();  // 停止
    }

    // 修改状态
    curState = PAUSE;
}

void DianVoteControl::VoteStop()
{
    ClearLog(); // 不管如何，清空日志数据并保存之是必须的。

    if(curState == RUNNING)
    {
        Q_ASSERT(stopWatch != NULL);
        stopWatch->stop();

        // 修改主界面
        HideStopWatch();
        pbPause->hide();
        pbStop->hide();
        pbStart->show();
        pbAuto->show();

        // 停止USB设备
        if(hidControl == NULL)
        {
            QMessageBox::critical(0, "Internal Error", "Can't Stop HidDevice");
            return;
        }
        StopHid();  // 停止
    }else if(curState == PAUSE)
    {
        Q_ASSERT(stopWatch != NULL);
        stopWatch->stop();

        // 修改主界面
        HideStopWatch();
        pbPause->hide();
        pbStop->hide();
        pbStart->show();
        pbAuto->show();

        // 因为pause状态hidCntrol已经停止，所以不需要停止
    }

    resizeAnimation->start();
    // 修改状态
    curState = STOP;
}

void DianVoteControl::DoShowResults()
{

#ifdef WIN32
    // 开启aero效果
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
    rd->key = option;       // 取出最后一个字节
    rd->id = id;            // 取出ID

    QString *time;          // 接收时间
    time = new QString();
    *time = QTime::currentTime().toString("hh:mm:ss");
    rd->revTime = time;

    // 容错性检查，如果收到已经发送过的数据手持端
    //      果断丢弃数据，并且写入log中
    for(int i = 0; i < log->length(); i++)
    {
        if((log->at(i)->id = id) && (log->at(i)->type == NORMAL))
        {
            rd->type = DUPLICATE;
            log->append(rd);
            return;
        }
    }
    emit updateGraph((int)rd->key - MAP_VALUE);  // 更新数据

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
    // 不管三七二十八，先把窗口设置成默认的大小
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
    // show的逆过程
    QSize curSize = this->size();
    resizeAnimation->setStartValue(curSize);
    resizeAnimation->setEndValue(initSize);
    connect(resizeAnimation, SIGNAL(finished()), this, SLOT(DoHideStopWatch()));
}

void DianVoteControl::mousePressEvent(QMouseEvent *event)
{
    // 关闭欢迎界面
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
    //获取设备屏幕大小
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
    // 在此将所有数据写入log文件

    log->clear();
}
