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

    //H3canvasDelegate windows(60, 0,"Test");
    //windows.show();

    int g = 4;

    std::vector<double> lengths;
    std::vector<double> twists;
    for (int i=0; i<3*g-3; i++)
    {
        //twists.push_back(1.0-.3*i);
        twists.push_back(-1.0 + i*.1);
    }
    for (int i=0; i<3*g-3; i++)
    {
        lengths.push_back(1.0 + .3*i);
    }

    FenchelNielsenConstructor fn(lengths,twists);
    DiscreteGroup group;
    IsomH2Representation rho(&group);

    rho = fn.getRepresentation(&group);
    //rho.setNiceRepresentation();

    rho.checkRelations();


    Canvas canvas;
    H2CanvasDelegate delegate(&canvas);
    canvas.setDelegate(&delegate);


    //std::vector<H2Isometry> listOfIsoms = rho.getSidePairingsNormalizedAroundVerticesToDepth(1);
    //std::vector<H2Polygon> listOfPolys = listOfIsoms*P;

    /*delegate.buffer.addElement(listOfPolys);
    clock_t t0 = clock();
    H2mesh mesh(5.0, rho);
    clock_t t1 = clock();
    std::cout << "time to construct mesh: " << (t1-t0)*1.0/CLOCKS_PER_SEC << "s" << std::endl;
    delegate.buffer.addElement(mesh, "red", 2);*/

    H2Polygon Q = rho.generatePolygon(200);
    delegate.buffer.addElement(rho);
    delegate.buffer.addElement(Q,"blue",2);
    std::vector<H2Triangle> triangleList = triangulate(triangulate(Q.triangulate()));
    delegate.buffer.addElement(triangleList,"red",1);

    std::cout << "There are " << triangleList.size() << " triangles" << std::endl;

    /*int i,index;
    for(i=0; i<10; i++)
    {
        index = mesh.getClosestMeshIndex(0.0 + (10*i+0.0)*(.02 + I*.03));
        std::cout << mesh.getMeshPoints()[index] << std::endl;
    }*/

    /*const complex * meshPoints = mesh.getMeshPoints();
    int nbMeshPoints = mesh.getNbMeshPoints();

    H2Point p;
    for (int i=0; i<nbMeshPoints; i++)
    {
        p.setHyperboloidProjection(meshPoints[i]);
        delegate.buffer.addElement(p, "black", 2);
    }*/

    H2Isometry id;
    id.setIdentity();
    delegate.redrawBuffer(id);
    canvas.show();


    std::cout << std::endl;
    return a.exec();
}
