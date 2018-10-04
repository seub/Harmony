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

    int maxIterations = 12000;
    double energy, energyError=100.0;

    for(uint i=0; i<25; i++)
    {
        FenchelNielsenConstructor FN(lengths,twists);
        rhoImage = FN.getRepresentation();
        LiftedGraphFunctionTriangulated<H2Point, H2Isometry> graph(rhoDomain, rhoImage, 4);
        DiscreteFlowIterator<H2Point,H2Isometry> iterator(& graph);
        eps = 1.0;
        energyError = 1.0;
        N = 0;

        while(eps > std::pow(10.0,-10) && N < maxIterations && energyError > std::pow(10.0,-8))
        {
            iterator.iterate(OutputMenu::FLOW_CENTROID);
            eps = iterator.updateSupDelta();
            energyError = iterator.updateEnergyError();
            energyError = std::abs(energyError);
            N += 1;
        }
        energy = iterator.getEnergy();
        std::cout<<"With length = "<< lengths[2] <<", iterations (optimal step) = "
                <<N<<", energy = "<<energy<<", energy error ="<<energyError<<std::endl;
        iterator.reset();
        N=0;
        energyError = 1.0;
        eps=1.0;
        while(eps > std::pow(10.0,-10) && N < maxIterations && energyError > std::pow(10.0,-8))
        {
            iterator.iterate(OutputMenu::FLOW_ENERGY_CONSTANT_STEP);
            eps = iterator.updateSupDelta();
            energyError = iterator.updateEnergyError();
            energyError = std::abs(energyError);
            N += 1;
        }
        energy = iterator.getEnergy();
        std::cout<<"With length = "<< lengths[2] <<", iterations (optimal step) = "
                <<N<<", energy = "<<energy<<", energy error ="<<energyError<<std::endl;
        iterator.reset();
        N=0;
        energyError = 1.0;
        eps=1.0;

        while(eps > std::pow(10.0,-10) && N < maxIterations && energyError > std::pow(10.0,-8))
        {
            iterator.iterate(OutputMenu::FLOW_ENERGY_OPTIMAL_STEP);
            eps = iterator.updateSupDelta();
            energyError = iterator.updateEnergyError();
            energyError = std::abs(energyError);
            N += 1;
        }
        energy = iterator.getEnergy();
        std::cout<<"With length = "<< lengths[2] <<", iterations (optimal step) = "
                <<N<<", energy = "<<energy<<", energy error ="<<energyError<<std::endl;

        std::cout<<std::endl;


        lengths[2] += .1;
    }




}
