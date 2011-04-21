#ifndef HISTGRAM_H
#define HISTGRAM_H

#include <qwt_plot_histogram.h>

// ÷±∑ΩÕº¿‡
class Histogram: public QwtPlotHistogram
{
public:
    Histogram(const QString &, const QColor &);

    void setColor(const QColor &);
    void setValues(uint numValues, const double *);
};

#endif // HISTGRAM_H
