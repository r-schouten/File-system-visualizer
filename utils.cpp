#include "utils.h"

Utils::Utils()
{

}

QString Utils::sizeToString(int64_t size)
{
    QStringList sizes;
    sizes << "bytes" << "kb" << "mb" << "gb" << "tb";
    int index = 0;
    int64_t oldSize = size;
    while(size > 1024)
    {
        oldSize = size;
        size = size / 1024;
        index++;
    }
    QString sizeString;
    sizeString += QString::number((int)size);
    if(index != 0)
    {
        sizeString += ".";
        sizeString += QString::number(oldSize - size*1024);
    }
    sizeString += " ";
    sizeString += sizes[index];

//    if(size < 1024)
//    {
//       sizeString = QString::number(size);
//       sizeString += "bytes";
//    }
//    if(size > 1024)
//    {
//        size = size >> 8;
//        sizeString = QString::number(size);
//        sizeString +="kb";
//    }
//    if(size > 1024)
//    {
//        size = size >> 8;
//        sizeString = QString::number(size);
//        sizeString +="mb";
//    }
//    if(size > 1024)
//    {
//        size = size >> 8;
//        sizeString = QString::number(size);
//        sizeString +="gb";
//    }
    return sizeString;
}
