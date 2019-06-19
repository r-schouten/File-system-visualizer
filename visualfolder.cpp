#include "visualfolder.h"
//declaration of static member pointer
VisualFolder* VisualFolder::selectedFolder = nullptr;

VisualFolder::VisualFolder(DirectoryEntry *directory, int middleX, int middleY, int dept, int hue)
    :directory(directory),middleX(middleX),middleY(middleY)
{
    distanceFromCenter = 100 + 105 * dept;
    outerRadius = distanceFromCenter + 100;
    s = 30 + qrand() % 225;
    l = 80 + std::min(dept * 35,255);
    h = hue + qrand() % 10;
    h = h%359;
    qDebug("%d %d %d",h,s,l);
}
//todo optimize bounding rect, boundingrect is to big with cost a lot of performance
QRectF VisualFolder::boundingRect() const
{
    return QRectF(-outerRadius/2, -outerRadius/2, outerRadius, outerRadius);
}
void VisualFolder::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    this->setX(middleX);
    this->setY(middleY);

    QRectF rect = boundingRect();
    QColor brushColor = QColor::fromHsl(h,s,l);
    QBrush brush(brushColor);
    QPen pen(Qt::black);
    pen.setWidth(1);
    QFont f;
    painter->setPen(pen);
    painter->setBrush(brush);

    // outer and inner washer dimensions
    QRectF r1(boundingRect());
    QRectF r2(-distanceFromCenter/2, -distanceFromCenter/2, distanceFromCenter, distanceFromCenter);

    double startAngle = 3.60 * startPercentage;
    double angle = 3.60 * percentage;
    // create a path with two arcs to form the outline
    QPainterPath path;
    path.arcTo(r1,startAngle,angle);
    path.arcTo(r2,angle + startAngle,-angle);

    // and finally fill it
    painter->fillPath(path, brushColor);
    //painter->drawPath(path);
}

void VisualFolder::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //event for click in the bounding rect
    //the bounding rect is not the pie, there are perhaps multiple folders that are clicked
    //check if this is the right folder
    int x = event->scenePos().x();
    int y = event->scenePos().y();
    int deltaX = x - middleX;
    int deltaY = y - middleY;
    //calculate the angle from the center
    double clickAngle = atan2(deltaY,deltaX) * 57;
    if(clickAngle < 0) clickAngle += 360;
    clickAngle = 360 - clickAngle;
    double startAngle = 3.60 * startPercentage;
    double endAngle = 3.60 * percentage + startAngle;

    //if the angle is between the start and end angle of the pie
    if(clickAngle > startAngle)
    {
        if(clickAngle < endAngle)
        {
            //also check the distance from the center because there are multiple layers
            int clickDistanceFromCenter = sqrt(deltaX * deltaX + deltaY * deltaY);
            if(clickDistanceFromCenter > distanceFromCenter/2)
            {
                if(clickDistanceFromCenter < outerRadius/2)
                {
                    selectedFolder = this;
                    event->setAccepted(true);
                    return;
                }
            }

        }
    }
    event->setAccepted(false);
}
