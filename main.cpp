#include <QApplication>
#include <QDebug>
#include <QWidget>

#include "window.h"
#include "fenchelnielsenconstructor.h"
#include "h2mesh.h"
#include "h2meshfunction.h"
#include "h2canvasdelegate.h"
#include "canvas.h"
#include "equivariantharmonicmapsfactory.h"
#include "h2isometry.h"

int main(int argc, char *argv[])
{

    std::cout << std::endl;
    QApplication a(argc, argv);

    Window window;

    int g = 2;
    std::vector<double> lengths1,lengths2;
    std::vector<double> twists1,twists2;
    for (int i=0; i<3*g-3; i++)
    {
        twists1.push_back(1.0-.3*i);
        //twists1.push_back(0.0);
        twists2.push_back(0.0);
    }
    twists2[2] = 2.0*M_PI;

    for (int i=0; i<3*g-3; i++)
    {
        lengths1.push_back(2);
        lengths2.push_back(1.0+3.0*i);
    }

    EquivariantHarmonicMapsFactory F;
    F.setGenus(2);
    //F.setRhoDomain(lengths1, twists1);
    F.setNiceRhoDomain();
    F.setRhoTarget(lengths2, twists2);
    //F.setNiceRhoTarget();
    F.setMeshDepth(4);
    F.initialize();

<<<<<<< HEAD

=======
    IsomH2Representation rhoDomain = F.getRhoDomain();
    IsomH2Representation rhoTarget = F.getRhoTarget();

    //H2Mesh mesh(rho, 1);

    H2MeshFunction f(F.functionInit);
    //f.iterate();


    ((H2CanvasDelegateDomain*) (window.leftCanvas->delegate))->buffer.addElement(&F.mesh,"blue", 1);
    ((H2CanvasDelegateDomain*) (window.leftCanvas->delegate))->redrawBuffer();
    ((H2CanvasDelegateDomain*) (window.leftCanvas->delegate))->addKickedDrawing(rhoDomain.getSidePairingsNormalizedAroundVertex());
    ((H2CanvasDelegateDomain*) (window.leftCanvas->delegate))->redrawBuffer();
    ((H2CanvasDelegateTarget*) (window.rightCanvas->delegate))->buffer.addElement(&f,"red", 1);
    ((H2CanvasDelegateTarget*) (window.rightCanvas->delegate))->redrawBuffer();
    ((H2CanvasDelegateTarget*) (window.rightCanvas->delegate))->buffer.setIsometries(rhoTarget.getSidePairingsNormalizedAroundVertex());
    ((H2CanvasDelegateTarget*) (window.rightCanvas->delegate))->addKickedDrawing(rhoTarget.getSidePairingsNormalizedAroundVertex());
    ((H2CanvasDelegateTarget*) (window.rightCanvas->delegate))->redrawBuffer();
>>>>>>> 91fedd6e4c65254211900beb9463362abc24b3a3

    window.show();
    window.resizeCanvases();





    std::cout << std::endl;




    return a.exec();
}
