#ifndef _DIAN_VOTE_DRAWER_H_
#define _DIAN_VOTE_DRAWER_H_

#include <qwt_plot.h>

class DianVoteDrawer: public QwtPlot
{
    Q_OBJECT

public:
    DianVoteDrawer(QWidget * = NULL);

private:
    void populate();     // Éú³ÉÍ¼

private Q_SLOTS:
    void showItem(QwtPlotItem *, bool on);
};

#endif
