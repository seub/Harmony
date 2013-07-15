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
#include "fenchelnielsenconstructor.h"

int main(int argc, char *argv[])
{
    std::cout << std::endl;
    QApplication a(argc, argv);

    //H3canvasDelegate windows(60, 0,"Test");
    //windows.show();

/*
    int g = 3;
=======

    int g = 10;
>>>>>>> 63a62e2a9ef548c6bd736ac23214dc2bc4e86102
    std::vector<double> lengths;
    std::vector<double> twists;
    twists.resize(3*g-3);
    //twists[2] = 1.0;
    //twists[4] = 1.0;
    for (int i=0; i<3*g-3; i++)
    {
        lengths.push_back(2.5);
    }

    FenchelNielsenConstructor fn(lengths,twists);
    DiscreteGroup group;
    IsomH2Representation rho = fn.getRepresentation(&group);
    rho.checkRelations();




    Canvas canvas;
    H2CanvasDelegate delegate(&canvas);canvas.setDelegate(&delegate);



    H2Point p;
    p.setDiskCoordinate(0.0);
    H2Polygon P = rho.generateFundamentalDomainForNormalizedSurfaceGroup(p);
    //delegate.buffer.addElement(rho, "red");
    delegate.buffer.addElement(P, "blue", 2);
    H2Isometry id;
    id.setIdentity();
    delegate.redrawBuffer(id);


    canvas.show();
*/
    TopologicalSurface S(2,0);
    DiscreteGroup Gamma(S);

    clock_t t0 = clock();
    std::vector<word> words = Gamma.getWordsOfLengthEqualTo(4);
    clock_t t1 = clock();

    std::cout << "time spent: " << (t1 - t0)/(CLOCKS_PER_SEC*1.0) << std::endl;


    std::cout << words.size() << std::endl;


    std::cout << std::endl;
    return a.exec();
}

