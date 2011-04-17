#include <QGraphicsView>
#include <QHBoxLayout>
#include <QString>
#include <QSize>

#include "slideeditor.h"
#include "../utilities/slidemodel.h"
#include "../utilities/slidescene.h"

QSlideEditor::QSlideEditor(QString &modelStr, QWidget *parent)
        : QWidget(parent)
        , slideScene(0)
{
    QSlideModel *model = new QSlideModel(modelStr, this);

    // set the slide scene with gray surround the white.
    slideScene = new QSlideScene(this, model);
    QPixmap pixmap(QSize(800, 600));
    pixmap.fill(Qt::white);
    slideScene->setBackgroundPixmap(pixmap);
    slideScene->setBackgroundBrush(Qt::darkGray);

    //set slide view
    QGraphicsView *view = new QGraphicsView(slideScene, this);
    view->setFrameShape(QFrame::NoFrame);
    QHBoxLayout *viewLayout = new QHBoxLayout;
    viewLayout->addWidget(view);
    setLayout(viewLayout);
}
