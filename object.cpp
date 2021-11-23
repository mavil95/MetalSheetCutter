#include "object.h"

Object::Object(const QPoint &topLeft, const QSize &size) :
    QRect(topLeft, size)
{
    calculateArea();
}

void Object::calculateArea()
{
    this->area = this->height() * this->width();
}

int Object::getArea()
{
    return this->area;
}

bool Object::operator==(const Object &rhs)
{
    return this->width() == rhs.width() && this->height() == rhs.height();
}

bool Object::operator!=(const Object &rhs)
{
    return !(*this == rhs);
}

bool Object::operator<(const Object &rhs)
{
    return this->area < rhs.area;
}

bool operator>(const Object &lhs, const Object &rhs)
{
    return lhs.area > rhs.area;
}

bool Object::operator>=(const Object &rhs)
{
    return !(*this < rhs);
}

bool Object::operator<=(const Object &rhs)
{
    return !(*this > rhs);
}
