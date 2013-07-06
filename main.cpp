#include <QApplication>
#include <QDebug>
#include <QWidget>


#include "types.h"

#include "canvas.h"
#include "canvasdelegate.h"
#include "h2point.h"
#include "h2canvasdelegate.h"

int main(int argc, char *argv[])
{    
    std::cout << std::endl;
    QApplication a(argc, argv);



    Canvas * canvas = new Canvas;

    H2CanvasDelegate * delegate = new H2CanvasDelegate(canvas);
    canvas->setDelegate(delegate);

    delegate->drawPoint(0.0, QColor("red"));
    delegate->drawSegment(-1.0, 1.0+1.0*I);
    delegate->drawCircle(0, 1, QColor("blue"));
    delegate->drawArc(0, 1, -M_PI/2, 3*M_PI/4, QColor("green"));

    H2Point p;
    p.setDiskCoordinate(0.5);
    delegate->drawH2Point(p, "orange");

    canvas->show();


    std::cout << std::endl;
    return a.exec();
}
