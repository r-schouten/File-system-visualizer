#pragma once
#include "directoryentry.h"
#include "middlecircle.h"
#include "config.h"

#include <QGraphicsItem>
#include <QLineEdit>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QRandomGenerator>
class VisualFolder : public QGraphicsItem
{
public:
    VisualFolder(DirectoryEntry *directory, MiddleCircle* center, int dept, int64_t beginSize, int64_t directorySize);

    int middleX;
    int middleY;
    int distanceFromCenter;
    int outerRadius;
    int64_t beginSize;
    int64_t directorySize;
    static int64_t totalSize;

    int h,s,l;

    DirectoryEntry* directory;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    //static pointer to the current selected folder to show in the tree view
    static VisualFolder* selectedFolder;
    static VisualFolder* rootFolder;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
private:
    bool validateClick(QGraphicsSceneMouseEvent *event);
};
