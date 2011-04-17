#include <QString>
#include "slidetextitem.h"

QSlideTextItem::QSlideTextItem(QGraphicsItem *parent)
        : QGraphicsTextItem(parent)
{
}

QSlideTextItem::QSlideTextItem(const QString &text, QGraphicsItem *parent)
        : QGraphicsTextItem(text, parent)
{
}

void QSlideTextItem::focusOutEvent(QFocusEvent *event)
{
    emit lostFocus(this);
    QGraphicsTextItem::focusOutEvent(event);
}
