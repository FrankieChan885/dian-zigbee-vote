#ifndef SLIDEEDITOR_H
#define SLIDEEDITOR_H

#include <QWidget>

class QString;
class QSlideScene;

class QSlideEditor : public QWidget
{
    Q_OBJECT
public:
    QSlideEditor(QString &modelStr, QWidget *parent = 0);

signals:

public slots:

private:
    QSlideScene *slideScene;
};

#endif // SLIDEEDITOR_H
