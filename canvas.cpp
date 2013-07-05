#include "canvas.h"
#include "types.h"
#include "h2canvas.h"
#include "h2point.h"
#include <QPainter>
#include <QImage>
#include <QLayout>

Canvas::Canvas(QWidget *parent) :
    QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setFocusPolicy(Qt::WheelFocus);

    hypPlan = new H2Canvas(500);
    resize(500,500);
    H2point p;
    p.setDiskCoordinate(complex(0.5,0.5));
    hypPlan->drawPoint(p);
    p.setDiskCoordinate(complex(0,0));
    hypPlan->drawPoint(p);
}

void Canvas::paintEvent(QPaintEvent *)
{

    QPainter canvas_painter(this);
    canvas_painter.drawImage(0, 0, *(hypPlan->getImage()));
}
