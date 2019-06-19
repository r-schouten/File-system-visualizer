#include "middlecircle.h"


MiddleCircle::MiddleCircle(int middleX, int middleY)
    :middleX(middleX),middleY(middleY){}

QRectF MiddleCircle::boundingRect() const
{
    return QRectF(-size/2,-size/2,size,size);
}

void MiddleCircle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    this->setX(middleX);
    this->setY(middleY);

    QRectF rect = boundingRect();
    QBrush brush(Qt::black);
    QPen pen(Qt::black);
    pen.setWidth(4);
    QFont f;

    painter->setBrush(brush);
    painter->setPen(pen);

    painter->drawEllipse(boundingRect());
}
