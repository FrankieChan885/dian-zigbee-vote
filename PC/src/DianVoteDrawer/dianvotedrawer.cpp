#include <stdlib.h>
#include <qpen.h>
#include <qwt_plot_layout.h>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_histogram.h>
#include <qwt_column_symbol.h>
#include <qwt_series_data.h>
#include "dianvotedrawer.h"
#include "histgram.h"

DianVoteDrawer::DianVoteDrawer(QWidget *parent):
    QwtPlot(parent)
{
    setTitle("Vote Results");

    setCanvasBackground(QColor(Qt::gray));
    plotLayout()->setAlignCanvasToScales(true);

    setAxisTitle(QwtPlot::yLeft, "Number of People");
    setAxisTitle(QwtPlot::xBottom, "Number of Hours");

    QwtLegend *legend = new QwtLegend;
    legend->setItemMode(QwtLegend::CheckableItem);
    insertLegend(legend, QwtPlot::RightLegend);

    populate();

    connect(this, SIGNAL(legendChecked(QwtPlotItem *, bool)),
        SLOT(showItem(QwtPlotItem *, bool)));

    replot(); // creating the legend items

    QwtPlotItemList items = itemList(QwtPlotItem::Rtti_PlotHistogram);
    for ( int i = 0; i < items.size(); i++ )
    {
        if ( i == 0 )
        {
            QwtLegendItem *legendItem = (QwtLegendItem *)legend->find(items[i]);
            if ( legendItem )
                legendItem->setChecked(true);
            items[i]->setVisible(true);
        }
        else
            items[i]->setVisible(false);
    }

    setAutoReplot(true);
}

void DianVoteDrawer::populate()
{
    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->enableX(false);
    grid->enableY(true);
    grid->enableXMin(false);
    grid->enableYMin(false);
    grid->setMajPen(QPen(Qt::black, 0, Qt::DotLine));
    grid->attach(this);

    const double juneValues[] = { 7, 0, 24, 0, 10, 0, 3 };
    const double novemberValues[] = { 4, 0, 22, 0, 13, 0, 4 };

    Histogram *histogramJune = new Histogram("Summer", Qt::red);
    histogramJune->setValues(
        sizeof(juneValues) / sizeof(double), juneValues);
    histogramJune->attach(this);

    Histogram *histogramNovember = new Histogram("Winter", Qt::blue);
    histogramNovember->setValues(
        sizeof(novemberValues) / sizeof(double), novemberValues);
    histogramNovember->attach(this);
}

void DianVoteDrawer::showItem(QwtPlotItem *item, bool on)
{
    item->setVisible(on);
}

