#include <QApplication>
#include <QDebug>
#include <QWidget>


#include "types.h"

#include "canvas.h"
#include "canvasdelegate.h"
#include "h2point.h"
#include "h2canvasdelegate.h"
#include "h2geodesic.h"

int main(int argc, char *argv[])
{    
    std::cout << std::endl;
    QApplication a(argc, argv);



    Canvas * canvas = new Canvas;

    H2CanvasDelegate * delegate = new H2CanvasDelegate(canvas);
    canvas->setDelegate(delegate);

    delegate->drawCircle(0, 1, QColor("blue"));

    H2Point p1, p2, p3;
    p1.setDiskCoordinate(0.3*I);
    p2.setDiskCoordinate(-.8*I);
    p3.setDiskCoordinate(.5);

    H2Geodesic L1(p1, p2), L2(p1, p3), L3(p2, p3);
    delegate->drawH2Geodesic(L1);
    delegate->drawH2Geodesic(L2);
    delegate->drawH2Geodesic(L3);


    delegate->drawH2Point(p1, "red");
    delegate->drawH2Point(p2, "red");
    delegate->drawH2Point(p3, "red");


    canvas->show();


    std::cout << std::endl;
    return a.exec();
}
