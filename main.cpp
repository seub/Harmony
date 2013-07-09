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
    rho.setNormalizedPairOfPantsRepresentation("a", "b", "c", 7.0, 3.0, 11.0, "a");
    std::cout << rho << std::endl;

    std::cout << std::endl;
    return a.exec();
}
