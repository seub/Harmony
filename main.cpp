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
#include "circle.h"
#include "h2mesh.h"

int main(int argc, char *argv[])
{
    std::cout << std::endl;
    QApplication a(argc, argv);
    Canvas canvas;
    H2CanvasDelegate delegate(&canvas);
    canvas.setDelegate(&delegate);

<<<<<<< HEAD
    //H3canvasDelegate windows(60, 0,"Test");
    //windows.show();

    int g = 2;
=======
/*    int g = 2;
>>>>>>> cf90d18b07f66c577724f041aac770c8594d156c

    std::vector<double> lengths;
    std::vector<double> twists;
    for (int i=0; i<3*g-3; i++)
    {
<<<<<<< HEAD
        //twists.push_back(-1.0 + i*.1);
        twists.push_back(0.0);
=======
        twists.push_back(-1.0 + i*.1);
>>>>>>> cf90d18b07f66c577724f041aac770c8594d156c
    }
    for (int i=0; i<3*g-3; i++)
    {
        //lengths.push_back(1.0 + .3*i);
        lengths.push_back(2.0);
    }

    FenchelNielsenConstructor fn(lengths,twists);
    DiscreteGroup group;
    IsomH2Representation rho(&group);

    rho = fn.getRepresentation(&group);
    rho.checkRelations();
<<<<<<< HEAD


    Canvas canvas;
    H2CanvasDelegate delegate(&canvas);
    canvas.setDelegate(&delegate);

    //H2Triangle T = Q.triangulate()[2];
=======
*/

    //std::vector<H2Isometry> listOfIsoms = rho.getSidePairingsNormalizedAroundVerticesToDepth(1);
    //std::vector<H2Polygon> listOfPolys = listOfIsoms*P;

    /*H2Polygon Q = rho.generatePolygon(200);
    delegate.buffer.addElement(rho);
    delegate.buffer.addElement(Q,"blue",3);
    std::vector<H2Triangle> triangleList = Q.triangulate();

    H2Triangle T;
>>>>>>> cf90d18b07f66c577724f041aac770c8594d156c
    H2Point aT,bT,cT;

    clock_t t0 = clock();
    for (unsigned int  j=0; j<triangleList.size(); j++)
    {
        T = triangleList[j];
        T.getPoints(aT,bT,cT);
        H2Triangulation Ts(&aT,&bT,&cT,5);
        delegate.buffer.addElement(Ts);
    }
    clock_t t1 = clock();
    std::cout << "time to construct triangulation: " << (t1-t0)*1.0/CLOCKS_PER_SEC << "s" << std::endl;*/

    H2Triangle T(3,3,4);
    delegate.buffer.addElement(T,"blue",3);

/*    H2Triangulation Ts(T,4);
    Ts.getTriangle();
    //delegate.buffer.addElement(Ts,"red");*/

    H2Isometry id;
    id.setIdentity();
    delegate.redrawBuffer(id);
    canvas.show();


    std::cout << std::endl;
    return a.exec();
}
