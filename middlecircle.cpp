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
    QBrush brush(Qt::red);
    QPen pen(Qt::black);
    pen.setWidth(4);
    QFont f;
    f.setPixelSize(20);
    painter->setFont(f);

    f.setWeight(10);
    painter->setBrush(brush);
    painter->setPen(pen);

    painter->drawEllipse(boundingRect());
    const QRect rectangle = QRect(-50, -50, 100, 100);
    painter->drawText(rectangle,Qt::AlignCenter,path);
}

void MiddleCircle::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    int x = event->scenePos().x();
    int y = event->scenePos().y();
    int deltaX = x - middleX;
    int deltaY = y - middleY;
    int clickDistanceFromCenter = sqrt(deltaX * deltaX + deltaY * deltaY);
    if(clickDistanceFromCenter < size/2)
    {
        doubleClicked = true;
        event->setAccepted(true);
    }
    event->setAccepted(false);
}
