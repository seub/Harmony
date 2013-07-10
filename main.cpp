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

    Canvas * canvas = new Canvas;
    H2CanvasDelegate delegate(canvas);
    canvas->setDelegate(&delegate);
    delegate.drawExample();
    canvas->show();


    DiscreteGroup Gamma1, Gamma2;
    Gamma1.setPairOfPants("a", "b", "c");
    Gamma2.setPairOfPants("d", "e", "f");
    DiscreteGroup Gamma3 = DiscreteGroup::amalgamateOverInverse(Gamma1, "b", Gamma2, "f", "t");
    //std::cout << Gamma3 << std::endl;

    IsomH2Representation rho1(&Gamma1), rho2(&Gamma2);
    rho1.setNormalizedPairOfPantsRepresentation("a1", "a2", "a3", 3.5, 3.3, 3.4, "a3");
    rho2.setNormalizedPairOfPantsRepresentation("b1", "b2", "b3", 3.4, 3.6, 3.4, "b1");
    //std::cout << rho1 << std::endl;
    //std::cout << rho2 << std::endl;

    DiscreteGroup Gamma;
    IsomH2Representation rho = IsomH2Representation::amalgamateOverInverse(&Gamma, rho1, "a3", "a2", rho2, "b1", "b3", "t", 0.4);
    //std::cout << rho << std::endl;
    rho.checkRelations();

    DiscreteGroup Gamma4, Gamma5;
    IsomH2Representation rho3(&Gamma4);
    rho3.setNormalizedPairOfPantsRepresentation("c1","c2","c3",2.3,3.4,3.5,"c2");
    IsomH2Representation rhoBig = IsomH2Representation::amalgamateOverInverse(&Gamma5, rho, "b3", "b2", rho3, "c2","c1","s", 2.8);
    std::cout << rhoBig << std::endl;
    rhoBig.checkRelations();

    DiscreteGroup pf;
    IsomH2Representation test = IsomH2Representation::doHNNextensionOverInverse(&pf, rhoBig, "a1", "a2", "c3", "c1", "di", 0.0);
    std::cout << test << std::endl;

    std::cout << std::endl;
    return a.exec();
}





