#ifndef CANVASDELEGATE_H
#define CANVASDELEGATE_H


class QImage;
class QPainter;
class QPen;
class H2Point;

class CanvasDelegate
{
public:
    CanvasDelegate();
    ~CanvasDelegate();

private:
    int size;

    QPen *pen;
    QImage *image;
    QPainter *painter;

};

#endif // CANVASDELEGATE_H
