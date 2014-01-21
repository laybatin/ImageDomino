#ifndef MYQGRAPHICSVIEW_H
#define MYQGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>

class MyQGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyQGraphicsView(QWidget *parent = 0);
    void wheelEvent(QWheelEvent *event);

signals:

public slots:
    void scalingTime(qreal);
    void animFinished();

public:
    int _numScheduledScalings ;
    int vertical_scroll_size;
};

#endif // MYQGRAPHICSVIEW_H
