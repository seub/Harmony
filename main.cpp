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

    /*H3canvasDelegate windows(60, 0,"Test");
    windows.show();

    DiscreteGroup Gamma;
    IsomH2Representation rho(&Gamma);
    rho.setNormalizedPairOfPantsRepresentation("a", "b", "c", 7.0, 3.0, 11.0, "c");


    Canvas * canvas = new Canvas;
    H2CanvasDelegate delegate(canvas);
    canvas->setDelegate(&delegate);
    delegate.drawExample();
    canvas->show();*/


    DiscreteGroup Gamma1, Gamma2;
    Gamma1.setPairOfPants("a", "b", "c");
    Gamma2.setPairOfPants("d", "e", "f");
    DiscreteGroup Gamma3 = DiscreteGroup::amalgamateOverInverse(Gamma1, "b", Gamma2, "f", "t");
    std::cout << Gamma3 << std::endl;

    IsomH2Representation rho1(&Gamma1), rho2(&Gamma2);
    rho1.setNormalizedPairOfPantsRepresentation("a1", "a2", "a3", 0.5, 1.5, 0.7, "a1");
    rho2.setNormalizedPairOfPantsRepresentation("b1", "b2", "b3", 0.7, 2.6, 1.0, "b2");
    std::cout << rho1 << std::endl;
    std::cout << rho2 << std::endl;

    DiscreteGroup Gamma;
    IsomH2Representation rho = IsomH2Representation::amalgamateOverInverse(&Gamma, rho1, "a3", "a2", rho2, "b1", "b3", "t", -4.9);
    std::cout << rho << std::endl;
    rho.checkRelations();



    std::cout << std::endl;
    return a.exec();
}
