/**
* @file slideplayerscene.h
* @brief the QSlidePlayerScene definition file.
* @author Tankery Chen @ Dian Group
* @version 1.0.0
* @date 2011-04-27
*/
#include "../utilities/slidescene.h"

class QSlidePlayerScene : public QSlideScene
{
    Q_OBJECT

public:
    QSlidePlayerScene(QWidget * parent = 0,
                      QSlideModel *sm = 0);

public slots:
    void textItemLostFocus(QGraphicsTextItem*);
    void textItemSelectedChange(QGraphicsTextItem*, bool);

};

