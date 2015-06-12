#include <QApplication>
#include <QDebug>
#include <QWidget>

#include <chrono>
#include <thread>

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

    std::vector<double> lengths1,lengths2;
    std::vector<double> twists1,twists2;
    for (int i=0; i<3*g-3; i++)
    {
        //twists.push_back(1.0-.3*i);
        twists1.push_back(0.0);
        twists2.push_back(0.0);
    }
    twists2[1] = 2*M_PI;

    for (int i=0; i<3*g-3; i++)
    {
        lengths1.push_back(1.0);
        lengths2.push_back(1.0);
    }

    FenchelNielsenConstructor fn1(lengths1,twists1);
    FenchelNielsenConstructor fn2(lengths2,twists2);
    DiscreteGroup group;
    IsomH2Representation rhoImage(&group), rhoDomain(&group);

    rhoDomain = fn1.getRepresentation(&group);
    rhoImage = fn2.getRepresentation(&group);

    //rhoImage.setNiceRepresentation();
    //rho.setNiceRepresentation();
    rhoDomain.checkRelations();
    std::cout << group << std::endl;

    std::cout << group.getRelations().front() << std::endl;


    H2Mesh mesh;
    int depth = 4;
    std::cout << "depth = " << depth << std::endl;
    clock_t start = clock();
    mesh = H2Mesh(rhoDomain, depth);
    clock_t end = clock();
    std::cout << "Time to construct mesh: " << (end-start)*1.0/CLOCKS_PER_SEC << "s" << std::endl;
    std::cout << std::endl;

    std::cout << "The mesh has " << mesh.nbPoints() << " points" << std::endl;


    Canvas canvas1(H2DELEGATEDOMAIN);
    ((H2CanvasDelegateDomain *) canvas1.delegate)->buffer.addElement(rhoDomain, "blue", 2);
    ((H2CanvasDelegateDomain *) canvas1.delegate)->buffer.addElement(&mesh);
    ((H2CanvasDelegateDomain *) canvas1.delegate)->redrawBuffer();
    canvas1.show();


    H2MeshFunction f(&mesh, rhoImage);
    start = clock();
    f.initializePLsmart();
    end = clock();
    std::cout << "Time to build function: " << (end-start)*1.0/CLOCKS_PER_SEC << "s" << std::endl;
    std::cout << std::endl;

<<<<<<< HEAD
=======

>>>>>>> 2c43ef5e3074a295dfeecb9ea26f455241fa1b6d
    Canvas canvas2(H2DELEGATETARGET);
    ((H2CanvasDelegateTarget *) canvas2.delegate)->buffer.addElement(&f, "red", 1);
    ((H2CanvasDelegateTarget *) canvas2.delegate)->redrawBuffer();
    canvas2.show();


    std::cout << std::endl;




    return a.exec();
}
