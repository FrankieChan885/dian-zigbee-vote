#include <QtGui/QApplication>
#include "dianvotecontrol.h"

#ifdef WIN32
    #include "qtwin.h"
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DianVoteControl w;
    w.setWindowFlags(Qt::Window
                     | Qt::CustomizeWindowHint
                     | Qt::WindowStaysOnTopHint
                     | Qt::FramelessWindowHint
                     /* | Qt::WindowTitleHint*/);

#ifdef WIN32
    // ¿ªÆôaeroÐ§¹û
    if (QtWin::isCompositionEnabled()) {
        QtWin::extendFrameIntoClientArea(&w);
        w.setContentsMargins(0, 0, 0, 0);
    }
#endif // #ifdef WIN32

    w.show();

    return a.exec();
}
