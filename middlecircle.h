#pragma once

#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include "config.h"
class MiddleCircle : public QGraphicsItem
{
public:
    MiddleCircle(int middleX, int middleY);
    int middleX;
    int middleY;
    int size = CIRCLE_RADIUS*2;
    QString path;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    bool doubleClicked = false;
    // QGraphicsItem interface
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};
