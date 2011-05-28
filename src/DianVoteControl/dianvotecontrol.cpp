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
    #include <windows.h>
    #include <winable.h>
#endif

QFile* DianVoteControl::VoteLog = new QFile(tr("Votelog.log"));

DianVoteControl::DianVoteControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DianVoteControl),
    options(NULL),
    drawer(NULL),
    hidControl(NULL),
    stopWatch(NULL),
    splash(NULL),
    curState(STOP),
    resultIndex(-1),
    previousOptionNum(DEFAULT_OPTION_NUM),
    previousLastTime(DEFAULT_DEADLINE),
    straightStart(false),
    getOptionNum(NULL),
    getLastTime(NULL)
{
    QDir dir;
    dir.setCurrent(QCoreApplication::applicationDirPath());
    windowIcon = new QIcon(dir.absoluteFilePath("res/images/app-icon.png"));
    this->setWindowIcon(*windowIcon);

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

#if 0
    qaSingleMode = new QAction(tr("SingleVoteMode"), this);  // 单选模式
    qaSingleMode->setCheckable(true);
    qaSingleMode->setChecked(true);         // 默认模式
    qaMutipleMode = new QAction(tr("MutipleVoteMode"), this); // 多选
    qaMutipleMode->setCheckable(true);
    qaRaceMode = new QAction(tr("RaceVoteMode"), this);    // 抢答
    qaRaceMode->setCheckable(true);
    pbOption->addAction(qaSingleMode);
    pbOption->addAction(qaMutipleMode);
    pbOption->addAction(qaRaceMode);
    connect(qaSingleMode, SIGNAL(triggered()), this, SLOT(DoSingleMode()));
    connect(qaMutipleMode, SIGNAL(triggered()), this, SLOT(DoMutipleMode()));
    connect(qaRaceMode, SIGNAL(triggered()), this, SLOT(DoRaceVoteMode()));
#endif

    pbClose = new QPushButton(this);
    pbClose->setObjectName(tr("pbClose"));
    ui->buttonLayout->addWidget(pbClose, 0, 4);

    connect(pbClose, SIGNAL(clicked()), this, SLOT(close()));
    connect(pbStart, SIGNAL(clicked()), this, SLOT(VoteStart()));
    connect(pbAuto, SIGNAL(clicked()), this, SLOT(VoteAuto()));
    connect(pbPause, SIGNAL(clicked()), this, SLOT(VotePause()));
    connect(pbStop, SIGNAL(clicked()), this, SLOT(VoteStop()));
    connect(pbResult, SIGNAL(clicked()), this, SLOT(DoShowResults()));
    connect(pbOption, SIGNAL(clicked()), this, SLOT(DoShowOptions()));

    drawer = new DianVoteDrawer();
    drawer->setWindowFlags(Qt::WindowStaysOnTopHint);
    drawer->setWindowIcon(*windowIcon);
    drawer->setWindowTitle(tr("Result"));
    connect(pbClose, SIGNAL(clicked()), this->drawer, SLOT(close()));
    connect(this, SIGNAL(setOptionNum(int)), drawer->histgram, SLOT(SetOptionNums(int)));
    connect(this, SIGNAL(setOptionNum(int)), drawer->pie, SLOT(SetOptionNums(int)));
    connect(this, SIGNAL(clearDrawData()), drawer->histgram, SLOT(ClearData()));
    connect(this, SIGNAL(clearDrawData()), drawer->pie, SLOT(ClearData()));
    connect(this, SIGNAL(updateGraph(int)), drawer->histgram, SLOT(HandleData(int)));
    connect(this, SIGNAL(updateGraph(int)), drawer->pie, SLOT(HandleData(int)));
    connect(this, SIGNAL(displayResult(bool)), drawer->histgram, SLOT(SetDisplayResultWhileVoting(bool)));
    connect(this, SIGNAL(displayResult(bool)), drawer->pie, SLOT(SetDisplayResultWhileVoting(bool)));
    connect(drawer, SIGNAL(ShowPreviousQuestion()), this, SLOT(DoShowPreviousQuestion()));
    connect(drawer, SIGNAL(ShowNextQuestion()), this, SLOT(DoShowNextQuestion()));
    connect(this, SIGNAL(setQuestionNum(int)), drawer, SLOT(SetQuestionNum(int)));

    try
    {
        hidControl = new HidControl();
        hidControl->open();
        connect(hidControl, SIGNAL(controlMessageComming(quint16,quint8)),
                this, SLOT(HandlerControlMessage(quint16,quint8)));
    }
    catch(DianVoteStdException *e)
    {
        QMessageBox::critical(0, "error", "Device Open Failed ! \nPlease ensure the Receiver is Pluged in.");
    }
    catch(...)
    {
        QMessageBox::critical(0, "error", "unknow exception.");
    }

    // show splash.
    QPixmap pixmap(dir.absoluteFilePath("res/images/logo.png"));
    QSplashScreen *splash = new QSplashScreen(pixmap);
    splash->setWindowIcon(*windowIcon);
    splash->setWindowFlags(Qt::WindowStaysOnTopHint);
    splash->setMask(pixmap.mask());
    splash->show();

    LoadStyleSheet("Default");

    // 记录初始化窗口大小
    initSize = this->size();
    this->setMaximumWidth(this->width());
    this->setMaximumHeight(this->height() + 100);
    this->move(0, 0);

    // 创建Log文件，并打开，程序退出是关闭
    VoteLog->open(QIODevice::WriteOnly | QIODevice::Append);

    resultList = new QList< QList < VoteData* >* >();

    desktopWidget = QApplication::desktop();
}

DianVoteControl::~DianVoteControl()
{
    delete ui;
    VoteLog->close();
    for(int i = 0; i < resultList->length(); i++)
    {
        for(int j = 0; j < resultList->at(i)->length(); j++)
        {
            delete resultList->at(i)->at(j);
        }
        delete resultList->at(i);
    }
    delete resultList;
}

void DianVoteControl::StartNewSession()
{
    // 创建已投票者记录
     voted = new QList< VoteData* >();
}

void DianVoteControl::SavePreviousSession()
{
    if(!voted->length())
    {
        // 如果没有投票数据
        return;
    }
    resultList->append(voted);
    resultIndex++;
    emit setQuestionNum(resultIndex + 1);
}

void DianVoteControl::DoShowPreviousQuestion()
{
    if(resultIndex == -1)
    {
        return;
    }
    if(resultIndex == 0)
    {
        voted = resultList->at(resultIndex);
        emit setOptionNum(optionNumList.at(resultIndex));
        emit setQuestionNum(resultIndex + 1);
    }
    else
    {
        voted = resultList->at(--resultIndex);
        emit setOptionNum(optionNumList.at(resultIndex));
        emit setQuestionNum(resultIndex + 1);
    }
    for(int i = 0; i < voted->length(); i++)
    {
        updateGraph(voted->at(i)->key - MAP_VALUE);
    }
    emit displayResult(true);
}

void DianVoteControl::DoShowNextQuestion()
{
    if(resultIndex == (resultList->length() - 1))
    {
        return;
    }
    voted = resultList->at(++resultIndex);
    emit setOptionNum(optionNumList.at(resultIndex));
    emit setQuestionNum(resultIndex + 1);
    for(int i = 0; i < voted->length(); i++)
    {
        updateGraph(voted->at(i)->key - MAP_VALUE);
    }
    emit displayResult(true);
}

void DianVoteControl::DoShowOptions()
{
    if(options == NULL)
    {
        options = new DianVoteOption();
        connect(pbClose, SIGNAL(clicked()),
                options, SLOT(close()));
    }

    options->isVisible() ?
            options->hide() :
            options->show();
}

void DianVoteControl::DoShowResults()
{
    if(!drawer->isVisible())
    {
    #ifdef WIN32
        // 开启aero效果
        if (QtWin::isCompositionEnabled()) {
            QtWin::extendFrameIntoClientArea(drawer);
            drawer->setContentsMargins(0, 0, 0, 0);
        }
    #endif // #ifdef WIN32

        drawer->show();
        drawer->raise();
    }
    else
    {
        drawer->hide();
    }
}

void DianVoteControl::DoShowStatics()
{

}

void DianVoteControl::HandlerControlMessage(quint16 id, quint8 option)
{
    switch (option)
    {
        case CONTROL_START_OR_STOP: {
                straightStart = true;
#ifdef WIN32
            INPUT input[3];
            memset(input, 0, 3 * sizeof(INPUT));

            input[0].type = INPUT_KEYBOARD;
            input[0].ki.wVk = VK_SHIFT;

            input[1].type = INPUT_KEYBOARD;
            input[1].ki.wVk = VK_CONTROL;

            input[2].type = INPUT_KEYBOARD;
            input[2].ki.wVk = 0x53;

            SendInput(3, input, sizeof(INPUT));

            input[0].type = INPUT_KEYBOARD;
            input[0].ki.wVk = 0x53;
            input[0].ki.dwFlags = KEYEVENTF_KEYUP;
            input[1].type = INPUT_KEYBOARD;
            input[1].ki.wVk = VK_SHIFT;
            input[1].ki.dwFlags = KEYEVENTF_KEYUP;
            input[2].type = INPUT_KEYBOARD;
            input[2].ki.wVk = VK_CONTROL;
            input[2].ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(3, input, sizeof(INPUT));

#endif // #ifdef WIN32

                break;
            }

        case CONTROL_SHOW_OR_HIDE_CONTROLER: {
#ifdef WIN32
            INPUT input[3];
            memset(input, 0, 3 * sizeof(INPUT));

            input[0].type = INPUT_KEYBOARD;
            input[0].ki.wVk = VK_SHIFT;

            input[1].type = INPUT_KEYBOARD;
            input[1].ki.wVk = VK_CONTROL;

            input[2].type = INPUT_KEYBOARD;
            input[2].ki.wVk = 0x43;

            SendInput(3, input, sizeof(INPUT));

            input[0].type = INPUT_KEYBOARD;
            input[0].ki.wVk = 0x43;
            input[0].ki.dwFlags = KEYEVENTF_KEYUP;
            input[1].type = INPUT_KEYBOARD;
            input[1].ki.wVk = VK_SHIFT;
            input[1].ki.dwFlags = KEYEVENTF_KEYUP;
            input[2].type = INPUT_KEYBOARD;
            input[2].ki.wVk = VK_CONTROL;
            input[2].ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(3, input, sizeof(INPUT));
#endif
                break;
            }

        case CONTROL_SHOW_OR_HIDE_RESULT: {
#ifdef WIN32
            INPUT input[3];
            memset(input, 0, 3 * sizeof(INPUT));

            input[0].type = INPUT_KEYBOARD;
            input[0].ki.wVk = VK_SHIFT;

            input[1].type = INPUT_KEYBOARD;
            input[1].ki.wVk = VK_CONTROL;

            input[2].type = INPUT_KEYBOARD;
            input[2].ki.wVk = 0x52;

            SendInput(3, input, sizeof(INPUT));

            input[0].type = INPUT_KEYBOARD;
            input[0].ki.wVk = 0x52;
            input[0].ki.dwFlags = KEYEVENTF_KEYUP;
            input[1].type = INPUT_KEYBOARD;
            input[1].ki.wVk = VK_SHIFT;
            input[1].ki.dwFlags = KEYEVENTF_KEYUP;
            input[2].type = INPUT_KEYBOARD;
            input[2].ki.wVk = VK_CONTROL;
            input[2].ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(3, input, sizeof(INPUT));

#endif // #ifdef WIN32
                break;
            }

        case CONTROL_PREVIOUS_SLIDE: {
#ifdef WIN32
            INPUT input[1];
            memset(input, 0, 1 * sizeof(INPUT));

            input[0].type = INPUT_KEYBOARD;
            input[0].ki.wVk = VK_PRIOR;

            SendInput(1, input, sizeof(INPUT));

            input[0].type = INPUT_KEYBOARD;
            input[0].ki.wVk = VK_PRIOR;
            SendInput(1, input, sizeof(INPUT));

#endif // #ifdef WIN32
                break;
            }

        case CONTROL_NEXT_SLIDE: {
#ifdef WIN32
            INPUT input[1];
            memset(input, 0, 1 * sizeof(INPUT));

            input[0].type = INPUT_KEYBOARD;
            input[0].ki.wVk = VK_NEXT;

            SendInput(1, input, sizeof(INPUT));

            input[0].type = INPUT_KEYBOARD;
            input[0].ki.wVk = VK_NEXT;
            SendInput(1, input, sizeof(INPUT));

#endif // #ifdef WIN32
                break;
            }
    }
}

void DianVoteControl::ParseData(quint16 id, quint8 option)
{
    VoteData *data = new VoteData();
    for(int i = 0; i < voted->length(); i++)
    {
        if(voted->at(i)->id == id)
        {
            return;
        }
    }

    data->name = "";
    data->id = id;
    data->key = option;
    voted->append(data);
    emit updateGraph(option - MAP_VALUE);  // 更新数据
}

int DianVoteControl::GetOptionNum()
{
    bool ok;
    getOptionNum = new QInputDialog();
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
        }
        connect(hidControl, SIGNAL(voteComing(quint16, quint8)),
                this, SLOT(ParseData(quint16, quint8)));
        connect(hidControl, SIGNAL(idAmountComing(uint)),
                drawer, SLOT(SetRepliedVoters(uint)));


#if 0
        if(voteMode == RACE_VOTE)
        {
            connect(hidControl, SIGNAL(voteComing(quint32, quint8)),
                    this, SLOT(ShowRaceVoteWinner(quint32,quint8)));
        }
#endif

        return true;
    }
    catch(DianVoteStdException *e)
    {
        QMessageBox::critical(0, "Hid Open Failed", "\nPlease ensure the Receiver is Pluged in.");
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
        hidControl->GetIDListLength();
        // start all remote.
        return true;
    }
    catch(DianVoteStdException *e)
    {
        QMessageBox::critical(0, "error", "Device Open Failed ! \nPlease ensure the Receiver is Pluged in.");
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
    connect(this, SIGNAL(setLastTime(int)),
            stopWatch, SLOT(SetStartTime(int)));
    connect(stopWatch, SIGNAL(autoStop()),
            this, SLOT(VoteStop()));
    ui->stopWatchLayout->addWidget(stopWatch);
}

void DianVoteControl::DoHideStopWatch()
{
    delete stopWatch;
    stopWatch = NULL;
}

void DianVoteControl::HideStopWatch()
{
    // show的逆过程
    QSize curSize = this->size();
    resizeAnimation->setStartValue(curSize);
    resizeAnimation->setEndValue(initSize);
    connect(resizeAnimation, SIGNAL(finished()), this, SLOT(DoHideStopWatch()));
}

#if 0
void DianVoteControl::ShowRaceVoteWinner(quint32 id, quint8 key)
{
    // 先关闭所有的设备，在打开被选中的设备
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
#endif  // do not compile this

void DianVoteControl::mousePressEvent(QMouseEvent *event)
{
    // 关闭欢迎界面
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
    //获取设备屏幕大小
//    int screenWidth = desktopWidget->screenGeometry().width();
    int screenHeight = desktopWidget->screenGeometry().height();

    if (event->buttons() & Qt::LeftButton) {
        QPoint pos = event->globalPos() - dragPosition;

        /*
        if((pos.x() <= DEFAULT_DOCK_SPACE))
        {
            pos.setX(0);
        }
        else if((pos.x() + this->width() + DEFAULT_DOCK_SPACE)
                 >= screenWidth)
        {
            pos.setX(screenWidth - this->width() - 2);
        }
        */

        if((pos.y() <= DEFAULT_DOCK_SPACE))
        {
            pos.setY(0);
        }
        else if((pos.y() + this->height() + DEFAULT_DOCK_SPACE)
                >= screenHeight)
        {
            pos.setY(screenHeight - this->height() - 2);
        }

        move(pos);
        event->accept();
    }
}

void DianVoteControl::DianVoteMsgHandler(QtMsgType type, const char *msg)
{
    QString log;
    switch (type) {
        case QtDebugMsg:
        {
            log = QString("Debug: %1").arg(msg);
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

void DianVoteControl::ClearLogList()
{
}

void DianVoteControl::GetIDList()
{
    try {
        if (!hidControl) {
            hidControl = new HidControl(this);
        }

        // get id list
        hidControl->GetIDList();

    } catch (DianVoteStdException *e) {
        QMessageBox::critical(0, "error", e->what());
        return;
    } catch (...) {
        QMessageBox::critical(0, "error", "unknow exception.");
        return;
    }
}

void DianVoteControl::GetIDListLength()
{
    try {
        if (!hidControl) {
            hidControl = new HidControl(this);
        }

        // get id amounts
        hidControl->GetIDListLength();
    } catch (DianVoteStdException *e) {
        QMessageBox::critical(0, "error", e->what());
        return;
    } catch (...) {
        QMessageBox::critical(0, "error", "unknow exception.");
        return;
    }
}

#if 0
void DianVoteControl::DoRaceVoteMode()
{
    voteMode = RACE_VOTE;
    // 更改Action的check状态
    qaRaceMode->setChecked(true);
    qaMutipleMode->setChecked(false);
    qaSingleMode->setChecked(false);
}

void DianVoteControl::DoSingleMode()
{
    voteMode = SINGLE_VOTE;
    // 更改Action的check状态
    qaRaceMode->setChecked(false);
    qaMutipleMode->setChecked(false);
    qaSingleMode->setChecked(true);
}

void DianVoteControl::DoMutipleMode()
{
    voteMode = MULTIPLE_VOTE;
    // 更改Action的check状态
    qaRaceMode->setChecked(false);
    qaMutipleMode->setChecked(true);
    qaSingleMode->setChecked(false);
}
#endif  // discard this
