#include <QApplication>
#include <QDebug>
#include <QWidget>


#include "tools.h"
#include "h2mesh.h"
#include "fenchelnielsenconstructor.h"
#include "h2canvasdelegate.h"
#include "canvas.h"
#include "h2meshfunction.h"
#include "h2meshfunctioniterator.h"

int main(int argc, char *argv[])
{
    std::cout << std::endl;
    QApplication a(argc, argv);


    int g = 2;

    std::vector<double> lengths;
    std::vector<double> twists;
    for (int i=0; i<3*g-3; i++)
    {
        //twists.push_back(1.0-.3*i);
        twists.push_back(0.2);
    }
    for (int i=0; i<3*g-3; i++)
    {
        lengths.push_back(1.0 + .3*i);
    }

    FenchelNielsenConstructor fn(lengths,twists);
    DiscreteGroup group;
    IsomH2Representation rhoImage(&group), rho(&group);

    rhoImage = fn.getRepresentation(&group);

    rho.setNiceRepresentation();
    rho.checkRelations();
    std::cout << group << std::endl;

    std::cout << group.getRelations().front() << std::endl;


    H2Mesh mesh;
    int depth = 1;
    std::cout << "depth = " << depth << std::endl;
    clock_t start = clock();
    mesh = H2Mesh(rho, depth);
    clock_t end = clock();
    std::cout << "Time to construct mesh: " << (end-start)*1.0/CLOCKS_PER_SEC << "s" << std::endl;
    std::cout << std::endl;

    std::cout << "The mesh has " << mesh.nbPoints() << " points" << std::endl;


/*    Canvas canvas(H2DELEGATE);
    ((H2CanvasDelegate *) canvas.delegate)->buffer.addElement(rho, "blue", 2);
    ((H2CanvasDelegate *) canvas.delegate)->buffer.addElement(mesh);
    ((H2CanvasDelegate *) canvas.delegate)->redrawBuffer();

    canvas.show();
*/

    H2MeshFunction f(&mesh, rhoImage);
    H2Point basept;
    basept.setDiskCoordinate(Complex(0.0,0.0));
    start = clock();
    f.initializePL(basept);
    end = clock();
    std::cout << "Time to build function: " << (end-start)*1.0/CLOCKS_PER_SEC << "s" << std::endl;
    std::cout << std::endl;

    start = clock();
    f.iterate(1);
    end = clock();
    std::cout << "Time to iterate discrete heat flow: " << (end-start)*1.0/CLOCKS_PER_SEC << "s" << std::endl;
    std::cout << std::endl;

    std::cout << std::endl;

/*    H2Point p1,p2,p3,cen;
    Complex z1,z2,z3,cenC;
    z1=Complex(-0.5,0.0);
    z2=Complex(0.5,0.0);
    z3=Complex(0.0,0.7);
    std::vector<H2Point> points;
    std::vector<double> weights;
    p1.setDiskCoordinate(z1);
    p2.setDiskCoordinate(z2);
    p3.setDiskCoordinate(z3);
    weights.push_back(0.3);
    weights.push_back(0.6);
    weights.push_back(0.1);
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    cen = H2Point::centroid(points,weights);
    cenC = cen.getDiskCoordinate();
    std::cout << "The " << weights << " centroid of " << z1 << ", " << z2 << ", and " << z3 << " = " << cenC << std::endl;
*/



    return a.exec();
}
