#include <QTime>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QMouseEvent>
#include <QPushButton>
#include <QMessageBox>
#ifdef WIN32
#include "qtwin.h"
#endif
#include "dianvotecontrol.h"
#include "ui_dianvotecontrol.h"
#include "exceptions.h"
#include "hidcontrol.h"

DianVoteControl::DianVoteControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DianVoteControl)
{
    ui->setupUi(this);

    pbStart = new QPushButton(this);
    ui->mainLayout->addWidget(pbStart, 0, 0);
    pbAuto = new QPushButton(this);
    ui->mainLayout->addWidget(pbAuto, 0, 1);
    pbResult = new QPushButton(this);
    ui->mainLayout->addWidget(pbResult, 0, 2);
    pbOption = new QPushButton(this);
    ui->mainLayout->addWidget(pbOption, 0, 3);
    pbClose = new QPushButton(this);
    ui->mainLayout->addWidget(pbClose, 0, 4);

    connect(pbClose, SIGNAL(clicked()), this, SLOT(close()));
    connect(pbStart, SIGNAL(clicked()), this, SLOT(DoShowResults()));

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
        if ((*log)[i])
        {
            delete (*log)[i];
        }
    }
    delete log;
}

void DianVoteControl::VoteStart()
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

void DianVoteControl::VoteAuto()
{

}

void DianVoteControl::SetStopWatch(int time)
{

}

void DianVoteControl::VotePause()
{

}

void DianVoteControl::VoteStop()
{

}

void DianVoteControl::DoShowResults()
{
    drawer = new DianVoteDrawer();
    // 开启aero效果
#ifdef WIN32
    if (QtWin::isCompositionEnabled()) {
        QtWin::extendFrameIntoClientArea(drawer);
        drawer->setContentsMargins(0, 0, 0, 0);
    }
#endif // #ifdef WIN32

    drawer->setAttribute(Qt::WA_DeleteOnClose);
    connect(this, SIGNAL(updateGraph(int)), drawer->histgram, SLOT(HandleData(int)));

    // 开始接收数据
    VoteStart();
    drawer->show();
}

void DianVoteControl::DoShowStatics()
{

}

void DianVoteControl::ParseData(quint32 id, quint8 option)
{
    RevData *rd = new RevData;
    rd->key = option;       // 取出最后一个字节
    emit updateGraph((int)rd->key - MAP_VALUE);  // 更新数据

	// 取出ID
	rd->id = id;

    // 接收时间
    QString *time;
    time = new QString();
    *time = QTime::currentTime().toString("hh:mm:ss");
    rd->revTime = time;
    log->append(rd);
}

void DianVoteControl::LoadStyleSheet(const QString &sheetName)
{
    QDir dir;
    QString cd = dir.absolutePath();
    QFile file(cd + "/res/skins/" + sheetName.toLower() + ".qss");
    if (!file.open(QFile::ReadOnly))
    {
        QMessageBox::critical(0, "error", sheetName + " Not Find. Use Default Style");
        return;
    }
    QString styleSheet = QLatin1String(file.readAll());

    qApp->setStyleSheet(styleSheet);
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
