#include <QDir>
#include <QIcon>
#include <QtGui/QApplication>
#include "dianvotecontrol.h"

#ifdef WIN32
    #include "qtwin.h"
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // �ض���
#ifdef REDIRECT_MESSAGE_TO_FILE
    qInstallMsgHandler(DianVoteControl::DianVoteMsgHandler);
#endif // end if def

    DianVoteControl w;
    w.setWindowFlags(Qt::Window
                     | Qt::CustomizeWindowHint
                     | Qt::WindowStaysOnTopHint
                     /* | Qt::WindowTitleHint*/);

#ifdef WIN32
    // ����aeroЧ��
    if (QtWin::isCompositionEnabled()) {
        QtWin::extendFrameIntoClientArea(&w);
        w.setContentsMargins(0, 0, 0, 0);
    }
#endif // #ifdef WIN32

    w.show();

    return a.exec();
}