#include "h2canvas.h"

#include <QPen>
#include <QPainter>
#include <QImage>
#include <QDebug>
#include "h2point.h"

H2Canvas::H2Canvas(int size)
{
    this->size = size;
    image =new QImage(size, size, QImage::Format_RGB32);

    pen = new QPen;
    pen->setWidth(3);
    painter = new QPainter(image);
    //painter.setPen();
    //canvas_delegate_->brush_.setStyle(Qt::SolidPattern);
    //painter.setBrush();

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->eraseRect(0, 0, size, size);
    painter->setPen(*pen);
    //painter->setWindow(-1.1, 1.1, 2.2, 2.1);

    //painter->translate(size/2.0,size/2.0);
    //painter->scale(size/2.2, size/2.2);
}

H2Canvas::~H2Canvas()
{
    delete image;
    delete painter;
    delete pen;
}

void H2Canvas::drawPoint(const H2point &p)
{
    complex z= p.getDiskCoordinate();
    z=conj(z)*(complex(size/2.)) +complex(size/2.)*(complex(1)+I);
    std::cout << z << " ";
    painter->drawPoint(z.real(),z.imag());

}

QImage *H2Canvas::getImage() const
{
    return image;
}
