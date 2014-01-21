#include "myqgraphicsview.h"
#include <QDebug>
#include <QWheelEvent>
#include <QTimeLine>
#include <QScrollBar>
MyQGraphicsView::MyQGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{

}


void MyQGraphicsView::wheelEvent(QWheelEvent *event) {

    vertical_scroll_size = this->horizontalScrollBar()->value();

    int numDegress = event->delta() / 8;        //+가 wheelUp, -가 wheelDown
    int numSteps = numDegress / 15;

    if(numSteps < -100) return;

    _numScheduledScalings += numSteps;

    if(_numScheduledScalings * numSteps < 0)
        _numScheduledScalings = numSteps;

    QTimeLine * anim = new QTimeLine(350, this);
    anim->setUpdateInterval(20);

    connect(anim, SIGNAL(valueChanged(qreal)), SLOT(scalingTime(qreal)));
    connect(anim, SIGNAL(finished()), SLOT(animFinished()));

    anim->start();

    qDebug() << _numScheduledScalings;

    //event->accept();

}


void MyQGraphicsView::scalingTime(qreal num) {
    if(_numScheduledScalings > 6) {
        _numScheduledScalings = 6;
    } else if (_numScheduledScalings < -6) {
        _numScheduledScalings = -6;
    }



    qreal factor = 1.0 + qreal(_numScheduledScalings ) / 300.0;


    scale(factor, factor);



}

void MyQGraphicsView::animFinished() {
    if(_numScheduledScalings > 0) {
        _numScheduledScalings--;

    } else {
        _numScheduledScalings++;

    }

    sender()->~QObject();

}
