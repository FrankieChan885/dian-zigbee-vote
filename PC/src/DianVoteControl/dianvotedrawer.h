#ifndef DIANVOTEDRAWER_H
#define DIANVOTEDRAWER_H

#include <QWidget>
#include "drawhistgram.h"

namespace Ui {
    class DianVoteDrawer;
}

class DianVoteDrawer : public QWidget
{

    Q_OBJECT

public:
    DianVoteDrawer(QWidget *parent = 0);

public:

    Ui::DianVoteDrawer *ui;
    DrawBase *histgram;

};

#endif // DIANVOTEDRAWER_H
