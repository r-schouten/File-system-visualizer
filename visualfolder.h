#pragma once
#include "directoryentry.h"

#include <QGraphicsItem>
#include <QLineEdit>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QRandomGenerator>
class VisualFolder : public QGraphicsItem
{
public:
    VisualFolder(DirectoryEntry *directory, int middleX, int middleY, int dept, int hue);

    int middleX;
    int middleY;
    int distanceFromCenter;
    int outerRadius;
    double startPercentage = 0;
    double percentage = 10;

    int h,s,l;

    DirectoryEntry* directory;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    //static pointer to the current selected folder to show in the tree view
    static VisualFolder* selectedFolder;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};
