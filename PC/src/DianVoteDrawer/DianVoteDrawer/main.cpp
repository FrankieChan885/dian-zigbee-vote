#include <QtGui/QApplication>
#include "dianvotedrawer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DianVoteDrawer w;
    w.show();

    return a.exec();
}
