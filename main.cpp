#include <QApplication>
#include <QDebug>
#include <QWidget>


#include "types.h"

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
    H3canvasDelegate windows(20, 0,"Test");
    windows.show();

/*    DiscreteGroup Gamma;
    IsomH2Representation rho(&Gamma);
    rho.setNormalizedPairOfPantsRepresentation("a", "b", "c", 7.0, 3.0, 11.0, "c");
    std::cout << rho << std::endl;
*/
    Canvas * canvas = new Canvas;
    H2CanvasDelegate delegate(canvas);
    canvas->setDelegate(&delegate);
    delegate.drawExample();
    canvas->show();

    std::cout << std::endl;
    return a.exec();
}
