#include <QApplication>
#include <QDebug>
#include <QWidget>


#include "types.h"

#include "canvas.h"
#include "canvasdelegate.h"
#include "h2canvasdelegate.h"

int main(int argc, char *argv[])
{    
    std::cout << std::endl;
    QApplication a(argc, argv);



    Canvas * canvas = new Canvas;
    H2CanvasDelegate * delegate = new H2CanvasDelegate(canvas);
    canvas->setDelegate(delegate);
    delegate->drawExample();
    canvas->show();


    std::cout << std::endl;
    return a.exec();
}
