#pragma once

#include <QGraphicsItem>
#include <QPainter>
#include <visualfolder.h>
#include "utils.h"
class FolderPropertiesLabel: public QGraphicsItem
{
public:
    FolderPropertiesLabel();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    int width = 1000;
    int height = 100;

    int64_t folderSize;
};

