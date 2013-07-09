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

    DiscreteGroup G;
    G.setPairOfPants("x", "y", "z");
    std::cout << G << std::endl;

/*    std::vector<generatorName> generators;
    generators.push_back("a");
    generators.push_back("b");
    generators.push_back("c");
    word w;
    std::vector<word> relations;
    w.push_back(letter(0, 1));
    w.push_back(letter(1, -1));
    w.push_back(letter(2, 1));
    w.push_back(letter(0, 3));
    relations.push_back(w);
    DiscreteGroup Gamma1(generators, relations);
    std::cout << Gamma1 << std::endl;

    generators.clear();
    generators.push_back("c");
    generators.push_back("d");
    generators.push_back("e");
    w.clear();
    relations.clear();
    w.push_back(letter(0, 1));
    w.push_back(letter(1, 1));
    w.push_back(letter(2, 1));
    w.push_back(letter(0, -2));
    relations.push_back(w);
    DiscreteGroup Gamma2(generators, relations);
    std::cout << Gamma2 << std::endl;

    DiscreteGroup Gamma = amalgamateOverCommonGenerator(Gamma1, Gamma2);
    std::cout << Gamma << std::endl;
*/

    Canvas * canvas = new Canvas;
    H2CanvasDelegate * delegate = new H2CanvasDelegate(canvas);
    canvas->setDelegate(delegate);
    delegate->drawExample();
    canvas->show();


/*    GroupRepresentation<H2Isometry> rho;
    rho.setNormalizedPairOfPantsRepresentation("a","b","c",1.0,2.0,2.0,"a");
    H2Isometry A,B,C;
    word w;
    w.push_back(letter(0,1));
    A = rho.evaluateRepresentation(w);
    std::cout << "translation length(A) = " << A.translationLength() << std::endl;

    w.push_back(letter(1,1));
    B = rho.evaluateRepresentation(w);
    std::cout << "translation length(B) = " << B.translationLength() << std::endl;

    w.push_back(letter(2,1));
    C = rho.evaluateRepresentation(w);
    std::cout << "translation length(C) = " << C.translationLength() << std::endl;

    double l=-1.4;
    SL2RMatrix M(exp(.5*l),0.0,0.0,exp(-.5*l));
    H2Isometry f;
    f.setSL2Rmatrix(M);

    std::cout << "translation length(f) = " << f.translationLength() << std::endl;
*/
    std::cout << std::endl;
    return a.exec();
}
