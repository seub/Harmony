#include <QApplication>
#include <QDebug>
#include <QWidget>


#include "types.h"

#include "canvas.h"
#include "canvasdelegate.h"
#include "h2canvasdelegate.h"
#include "discretegroup.h"
#include "h3canvasdelegate.h"
int main(int argc, char *argv[])
{    
    std::cout << std::endl;
    QApplication a(argc, argv);
    H3canvasDelegate windows(20, 0,"Test");
    windows.show();
    return a.exec();
}
