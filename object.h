#ifndef OBJECT_H
#define OBJECT_H

#include <QRect>

class Object : QRect
{
public:
    Object() : QRect() {}
    Object(const QPoint &topLeft, const QSize &size);

    void calculateArea();
    int getArea();

    bool operator==(const Object &rhs);
    bool operator!=(const Object &rhs);
    friend bool operator>(const Object &lhs, const Object &rhs);
    bool operator<(const Object &rhs);
    bool operator>=(const Object &rhs);
    bool operator<=(const Object &rhs);
    inline bool compare (Object a, Object b) {
        return (a > b); }

    friend class CuttingDialog;

private:
    int area {0};
};


#endif // OBJECT_H
