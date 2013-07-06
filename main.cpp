#include <QApplication>
#include <QDebug>
#include <QWidget>


#include "types.h"

#include "canvas.h"
#include "canvasdelegate.h"

int main(int argc, char *argv[])
{    
    std::cout << std::endl;
    QApplication a(argc, argv);



    Canvas * canvas = new Canvas;

    std::cout << "alice" << std::endl;

    CanvasDelegate * delegate = new CanvasDelegate(canvas);
    canvas->setDelegate(delegate);

    delegate->drawPoint(0.0, QColor("red"));
    canvas->show();


    std::cout << "bob" << std::endl;
    std::cout << std::endl;
    return a.exec();
}
