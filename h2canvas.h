#ifndef H2CANVAS_H
#define H2CANVAS_H


#include "types.h"

class QImage;
class QPainter;
class QPen;
class H2point;

class H2Canvas
{
public:
    H2Canvas(int size);
    ~H2Canvas();

    void drawPoint(const H2point & p);
    QImage *getImage() const;

private:
    int size;
    QPen *pen;
    QImage *image;
    QPainter *painter;
};

#endif // H2CANVAS_H
