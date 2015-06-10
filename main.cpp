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
    int depth = 2;
    //for (k=0; k<10; ++k)
    //{
    std::cout << "depth = " << depth << std::endl;
    clock_t start = clock();
    mesh = H2Mesh(rho, depth);
    clock_t end = clock();
    std::cout << "Time to construct mesh: " << (end-start)*1.0/CLOCKS_PER_SEC << "s" << std::endl;
    std::cout << std::endl;
    //}

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
    f.iterate(3);
    end = clock();
    std::cout << "Time to iterate discrete heat flow: " << (end-start)*1.0/CLOCKS_PER_SEC << "s" << std::endl;
    std::cout << std::endl;

    std::cout << std::endl;
    return a.exec();
}
