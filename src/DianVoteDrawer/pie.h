#ifndef PIE_H
#define PIE_H

#include <QWidget>
#include "datatype.h"

class Pie : public QWidget
{
    Q_OBJECT
public:
    explicit Pie(QWidget *parent = 0);
    Pie(QList< OptionData > &data);

signals:

public slots:

};

#endif // PIE_H
