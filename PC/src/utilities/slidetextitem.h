#ifndef SLIDETEXTITEM_H
#define SLIDETEXTITEM_H

#include <QGraphicsTextItem>

class QString;

class QSlideTextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    QSlideTextItem(QGraphicsItem *parent = 0);
    QSlideTextItem(const QString & text, QGraphicsItem *parent = 0);

protected:
    void focusOutEvent(QFocusEvent *event);

signals:
    void lostFocus(QGraphicsTextItem *item);

public slots:

};

#endif // SLIDETEXTITEM_H
