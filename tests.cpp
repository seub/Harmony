#include "tools.h"
#include "canvas.h"
#include "canvasdelegatetests.h"

#include "grouprepresentation.h"
#include "topologicalsurface.h"
#include "fenchelnielsenconstructor.h"
#include "liftedgraph.h"
#include "discreteflowiteratorcentroid.h"


void runTests()
{
    DiscreteGroup Gamma(TopologicalSurface(2, 0));

    GroupRepresentation<H2Isometry> rhoDomain(Gamma);
    GroupRepresentation<H2Isometry> rhoImage(Gamma);

    std::vector<double> lengths = {1, 3, 2};
    std::vector<double> twists = {0, -1, 0};
    FenchelNielsenConstructor FN(lengths, twists);

    rhoDomain = FN.getRepresentation();
    rhoImage.setNiceRepresentation();

    LiftedGraphFunctionTriangulated<H2Point, H2Isometry> graph(rhoDomain, rhoImage, 4);
    DiscreteFlowIteratorCentroid<H2Point, H2Isometry> iterator(&graph);

    std::vector<H2Point> traject1, traject2, traject3, traject4;
    uint nbPoints = graph.getNbPoints(), nbBoundaryPoints = graph.getNbBoundaryPoints();
    uint index1 = 0, index2 = nbBoundaryPoints/3, index3 = nbBoundaryPoints+1, index4 = nbBoundaryPoints + (nbPoints - nbBoundaryPoints)/2;

    uint N = 1000;
    traject1.reserve(N);
    traject2.reserve(N);
    traject3.reserve(N);
    traject4.reserve(N);

    iterator.iterate(500);
    for (uint i=0; i!=N; ++i)
    {
        iterator.iterate();
        traject1.push_back(iterator.getValue(index1));
        traject2.push_back(iterator.getValue(index2));
        traject3.push_back(iterator.getValue(index3));
        traject4.push_back(iterator.getValue(index4));
    }

    iterator.iterate(2000);

    H2Point final1 = iterator.getValue(index1), final2 = iterator.getValue(index2), final3 = iterator.getValue(index3), final4 = iterator.getValue(index4);
    std::vector<double> sequence1, sequence2, sequence3, sequence4;
    sequence1.reserve(N);
    sequence2.reserve(N);
    sequence3.reserve(N);
    sequence4.reserve(N);

    for (uint i=0; i!=N; ++i)
    {
        sequence1.push_back(H2Point::distance(traject1[i], final1));
        sequence2.push_back(H2Point::distance(traject2[i], final2));
        sequence3.push_back(H2Point::distance(traject3[i], final3));
        sequence4.push_back(H2Point::distance(traject4[i], final4));
    }




    std::cout << "sequence 1 front and back : " << sequence1.front() << ",  " << sequence1.back() << std::endl;


    CanvasDelegateTests* delegate = new CanvasDelegateTests(700);
    Canvas *canvasTest = new Canvas(delegate);

    CanvasDelegateTests2* delegate2 = new CanvasDelegateTests2(700);
    Canvas *canvasTest2 = new Canvas(delegate2);

    delegate->addSequence(sequence1, "red");
    delegate->addSequence(sequence2, "blue");
    delegate->addSequence(sequence3, "green");
    delegate->addSequence(sequence4, "purple");

    delegate2->addSequence(traject1, "red");
    delegate2->addSequence(traject2, "blue");
    delegate2->addSequence(traject3, "green");
    delegate2->addSequence(traject4, "purple");

    canvasTest->show();
    canvasTest2->show();
}
