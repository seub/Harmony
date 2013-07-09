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

    DiscreteGroup Gamma;
    IsomH2Representation rho(&Gamma);
    rho.setNormalizedPairOfPantsRepresentation("a", "b", "c", 7.0, 13.0, 19.0, "c");
    //std::cout << Gamma << std::endl;

    word w1;
    w1.push_back(letter(0,1));
    H2Isometry f1 = rho.evaluateRepresentation(w1);
    std::cout << "translation length " << f1.translationLength() << std::endl;
    word w2;
    w2.push_back(letter(1,1));
    H2Isometry f2 = rho.evaluateRepresentation(w2);
    std::cout << "translation length " << f2.translationLength() << std::endl;
    word w3;
    w3.push_back(letter(2,1));
    H2Isometry f3 = rho.evaluateRepresentation(w3);
    std::cout << "translation length " << f3.translationLength() << std::endl;


    /*

    Canvas * canvas = new Canvas;
    H2CanvasDelegate * delegate = new H2CanvasDelegate(canvas);
    canvas->setDelegate(delegate);
    delegate->drawExample();
    canvas->show();*/

    std::cout << std::endl;
    return a.exec();
}
