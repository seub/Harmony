#include <QApplication>
#include <QDebug>
#include <QWidget>


#include "tools.h"

#include "canvas.h"
#include "canvasdelegate.h"
#include "h2canvasdelegate.h"
#include "discretegroup.h"
#include "h3canvasdelegate.h"
#include "grouprepresentation.h"
#include "h2isometry.h"

int main(int argc, char *argv[])
{
    std::cout << std::endl;
    QApplication a(argc, argv);

    H3canvasDelegate windows(60, 0,"Test");
    windows.show();

    /*Canvas * canvas = new Canvas;
H2CanvasDelegate delegate(canvas);
canvas->setDelegate(&delegate);
delegate.drawExample();
canvas->show();

TopologicalSurface S(5,0);
std::vector<DiscreteGroup> vector = S.getPantsDecomposition();

for(unsigned int i=0;i<vector.size();i++)
{
std::cout << vector[i] << std::endl;
}*/

    std::cout << std::endl;
    return a.exec();
}

