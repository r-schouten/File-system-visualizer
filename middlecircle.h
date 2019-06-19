#pragma once

#include <QGraphicsItem>
#include <QPainter>

class MiddleCircle : public QGraphicsItem
{
public:
    MiddleCircle(int middleX, int middleY);
    int middleX;
    int middleY;
    int size = 200;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};
