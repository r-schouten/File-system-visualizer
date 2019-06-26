#include "folderpropertieslabel.h"

FolderPropertiesLabel::FolderPropertiesLabel()
{
}

QRectF FolderPropertiesLabel::boundingRect() const
{
    return QRectF(0,0,width,height);
}

void FolderPropertiesLabel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    setPos(0,0);
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


    QString path = "selected folder: ";
    if(VisualFolder::selectedFolder)
    {
        path += QString::fromStdString(VisualFolder::selectedFolder->directory->directory.string());

        QString sizeString = "size: ";
        sizeString += Utils::sizeToString(VisualFolder::selectedFolder->directorySize);
        painter->drawText(0,60,width,50,Qt::TextWordWrap,sizeString);
    }
    else {
        path += "none";
    }
    painter->drawText(boundingRect(),Qt::TextWordWrap,path);



}
