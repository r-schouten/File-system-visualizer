#include "visualfolder.h"
//declaration of static member pointer
VisualFolder* VisualFolder::selectedFolder = nullptr;
VisualFolder* VisualFolder::rootFolder = nullptr;
int64_t VisualFolder::totalSize = 0;
VisualFolder::VisualFolder(DirectoryEntry *directory, MiddleCircle *center, int dept, int64_t beginSize, int64_t directorySize, int parentColor)
    :directory(directory),beginSize(beginSize),directorySize(directorySize)
{
    middleX = center->middleX;
    middleY = center->middleY;
    distanceFromCenter = CIRCLE_RADIUS + (VISUAL_RING_SIZE+5) * dept;
    outerRadius = distanceFromCenter + VISUAL_RING_SIZE;

    s = 30 + qrand() % 225;
    l = 80 + std::min((255-80)/(MAX_DEPT+1) * dept, 255);
    if(parentColor == -1)
    {
        h = qrand();
        h = h%359;
    }
    else {
        //value between -40 - -10 or 10 - 40
        int randomValue = (qrand()%60)-30;
        if(randomValue >0)
        {
            randomValue +=10;
        }
        else {
            randomValue -=10;
        }
        h = parentColor + randomValue;
        if(h<0)h+=360;
        if(h>359)h-=360;
    }

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
    const QRectF r1(boundingRect());
    QRectF r2(-distanceFromCenter/2, -distanceFromCenter/2, distanceFromCenter, distanceFromCenter);

    double startAngle = 360 * (double)beginSize/totalSize;
    double angle = 360 * (double)directorySize/totalSize;

    // create a path with two arcs to form the outline
    QPoint startPoint(0.5*distanceFromCenter*cos(startAngle/-57.2958),0.5*distanceFromCenter*sin(startAngle/-57.2958));
    QPainterPath path(startPoint);
    path.arcTo(r1,startAngle,angle);
    path.arcTo(r2,angle + startAngle,-angle);

    // and finally fill it
    painter->fillPath(path, brushColor);
    if(selectedFolder == this)
    {
        painter->drawPath(path);
    }

    //if a item is to small to visualize delete it
    if((double)directorySize/totalSize < 0.005)//0.5%
    {
        delete this;
    }
}
bool VisualFolder::validateClick(QGraphicsSceneMouseEvent *event)
{
    //event for click in the bounding rect
    //the bounding rect is not the pie, there are perhaps multiple folders that are clicked
    //check if this is the right folder
    int x = event->scenePos().x();
    int y = event->scenePos().y();
    int deltaX = x - middleX;
    int deltaY = y - middleY;
    //calculate the angle from the center
    double clickAngle = atan2(deltaY,deltaX);
    if(clickAngle < 0) clickAngle += 2*M_PI;
    clickAngle = 2*M_PI - clickAngle;
    double startAngle = 2*M_PI * (double)beginSize/totalSize;
    double endAngle = 2*M_PI * (double)directorySize/totalSize + startAngle;

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
                    return true;
                }
            }
        }

    }
    return false;
}
void VisualFolder::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //event for click in the bounding rect
    //the bounding rect is not the pie, there are perhaps multiple folders that are clicked
    //check if this is the right folder
    if(validateClick(event))
    {
        selectedFolder = this;
        event->setAccepted(true);
        return;

    }
    event->setAccepted(false);
}

void VisualFolder::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    //event for click in the bounding rect
    //the bounding rect is not the pie, there are perhaps multiple folders that are clicked
    //check if this is the right folder
    if(validateClick(event))
    {
        rootFolder = this;
        event->setAccepted(true);
        return;

    }
    event->setAccepted(false);
}
