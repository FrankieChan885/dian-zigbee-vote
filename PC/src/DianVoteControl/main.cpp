#include <QtGui/QApplication>
#include "dianvotecontrol.h"

#ifdef WIN32
    #include "qtwin.h"
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 重定向
    qInstallMsgHandler(DianVoteControl::DianVoteMsgHandler);

    DianVoteControl w;
    w.setWindowFlags(Qt::Window
                     | Qt::CustomizeWindowHint
                     | Qt::WindowStaysOnTopHint
                     /* | Qt::FramelessWindowHint*/
                     /* | Qt::WindowTitleHint*/);

#ifdef WIN32
    // 开启aero效果
    if (QtWin::isCompositionEnabled()) {
        QtWin::extendFrameIntoClientArea(&w);
        w.setContentsMargins(0, 0, 0, 0);
    }
#endif // #ifdef WIN32

    w.show();

    qDebug("---App Started---");
    return a.exec();
}
