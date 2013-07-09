#include <QApplication>
#include <QDebug>
#include <QWidget>


#include "types.h"

#include "canvas.h"
#include "canvasdelegate.h"
#include "h2canvasdelegate.h"
#include "discretegroup.h"
#include "grouprepresentation.h"
#include "h2isometry.h"

int main(int argc, char *argv[])
{
    std::cout << std::endl;
    QApplication a(argc, argv);

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


    DiscreteGroup Gamma1, Gamma2;
    Gamma1.setPairOfPants("a", "b", "c");
    Gamma2.setPairOfPants("d", "e", "f");
    DiscreteGroup Gamma3 = DiscreteGroup::amalgamateOverInverse(Gamma1, "b", Gamma2, "f");
    std::cout << Gamma3 << std::endl;

    std::cout << std::endl;

    return a.exec();
}
