#include "mainapplication.h"
#include "h2polygontriangulater.h"
#include "tools.h"
#include "h2point.h"
#include "h2tangentvector.h"
#include "fenchelnielsenconstructor.h"
#include "outputmenu.h"

int main(int argc, char *argv[])
{



//    return MainApplication(argc, argv).exec();

    DiscreteGroup Gamma(TopologicalSurface(2, 0));

    GroupRepresentation<H2Isometry> rhoDomain(Gamma);
    GroupRepresentation<H2Isometry> rhoImage(Gamma);
    rhoDomain.setNiceRepresentation();


    std::vector<double> lengths = {2.0, 2.0, 0.2};
    std::vector<double> twists = {-1.5, 2.0, 0.5};



    double eps = 1.0;
    int N = 0;

    int maxIterations = 20000;
    double energy;

    for(uint i=0; i<25; i++)
    {
        FenchelNielsenConstructor FN(lengths,twists);
        rhoImage = FN.getRepresentation();
        LiftedGraphFunctionTriangulated<H2Point, H2Isometry> graph(rhoDomain, rhoImage, 4);
        DiscreteFlowIterator<H2Point,H2Isometry> iterator(& graph);
        eps = 1.0;
        N = 0;
        while(eps > std::pow(10.0,-10) && N < maxIterations)
        {
            iterator.iterate(OutputMenu::FLOW_CENTROID);
            eps = iterator.updateSupDelta();
            N += 1;
        }
        energy = iterator.getEnergy();
        std::cout<<"With length = "<< lengths[2] <<", the number of iterations for centroid = "
                <<N<<" and terminated with energy = "<<energy<<std::endl;
        iterator.reset();
        N=0;
        eps=1.0;
        while(eps > std::pow(10.0,-10) && N < maxIterations)
        {
            iterator.iterate(OutputMenu::FLOW_ENERGY_CONSTANT_STEP);
            eps = iterator.updateSupDelta();
            N += 1;
        }
        energy = iterator.getEnergy();
        std::cout<<"With length = "<< lengths[2] <<", the number of iterations for constant step = "
                <<N<<" and terminated with energy = "<<energy<<std::endl;
        iterator.reset();
        N=0;
        eps=1.0;
        while(eps > std::pow(10.0,-10) && N < maxIterations)
        {
            iterator.iterate(OutputMenu::FLOW_ENERGY_OPTIMAL_STEP);
            eps = iterator.updateSupDelta();
            N += 1;
        }
        energy = iterator.getEnergy();
        std::cout<<"With length = "<< lengths[2] <<", the number of iterations for optimal step = "
                <<N<<" and terminated with energy = "<<energy<<std::endl;

        std::cout<<std::endl;


        lengths[2] += .1;
    }




}
