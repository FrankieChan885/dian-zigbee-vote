#include <QtGui/QApplication>
#include "dianvotedrawer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DianVoteDrawer plot;

    plot.resize(600,600);
    plot.show();

    return a.exec();
}
