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
    basept.setDiskCoordinate(Complex(-0.7,-0.7));
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

    std::vector<Complex> test{Complex(0.880804,0.454267),Complex(0.873785,0.485382),
                Complex(0.874183,0.485592),Complex(0.872984,0.485971),Complex(0.874071,0.485753),
                Complex(0.874076,0.485756),Complex(0.874061,0.485751),Complex(0.872908,0.486938),
                Complex(0.87358,0.48638),Complex(0.0,0.0),Complex(0.830459,0.505288),Complex(-0.0941877,0.304982),
                Complex(-0.17561,0.302569),Complex(0.768432,0.427753),Complex(0.947301,0.241949),
                Complex(0.857906,0.432588),Complex(0.857906,0.432588),Complex(0.904807,0.416147),
                Complex(0.866591,0.482394)};
    std::vector<H2Point> testPts;
    H2Point pt;
    for (auto z : test)
    {
        pt.setDiskCoordinate(z);
        testPts.push_back(pt);
    }
    std::vector<double> testWeights{0.0694475, 0.0750824, 0.0309618, 0.0290859, 0.0135102,
                                    0.00469644, 0.0135215, 0.0290229, 0.0306143, 0.0739811,
                                    0.0684015, 0.0194336, 0.0123215, 0.0329096, 0.0263232,
                                    0.0329648, 0.0124699, 0.0196033, 0.405649};
    H2Point bar = H2Point::centroid(testPts,testWeights);
    std::cout << "The barycenter = " << bar.getDiskCoordinate() << std::endl;
*/




    return a.exec();
}
