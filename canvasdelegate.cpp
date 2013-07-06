#include "canvasdelegate.h"

#include <QPen>
#include <QPainter>
#include <QImage>

CanvasDelegate::CanvasDelegate()
{
    //this->size = size;

    pen = new QPen;
    pen->setWidth(2);

    painter = new QPainter(image);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->eraseRect(0, 0, size, size);
    painter->setPen(*pen);
}

CanvasDelegate::~CanvasDelegate()
{
    delete image;
    delete painter;
    delete pen;
}
