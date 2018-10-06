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


    std::vector<double> lengths = {2.0, 2.0, 2.6};
    std::vector<double> twists = {-1.5, 2.0, 0.5};



    double eps = 1.0;
    int N = 0;

    int maxIterations = 20000;
    double energy, energyError;

    for(uint i=0; i<20; i++)
    {
        FenchelNielsenConstructor FN(lengths,twists);
        rhoImage = FN.getRepresentation();
        LiftedGraphFunctionTriangulated<H2Point, H2Isometry> graph(rhoDomain, rhoImage, 4);
        DiscreteFlowIterator<H2Point,H2Isometry> iterator(& graph);
        eps = 1.0;
        energyError = -1.0;
        N = 0;

        while(eps > std::pow(10.0,-10) && N < maxIterations)
        {
            iterator.iterate(OutputMenu::FLOW_CENTROID);
            eps = iterator.updateSupDelta();
            energyError = iterator.getEnergyError();
            energy = iterator.getEnergy();
            N += 1;
        }



        energy = iterator.getEnergy();
        std::cout<<"With length = "<< lengths[2] <<", iterations (centroid) = "
                <<N<<", energy = "<<energy<<", energy error ="<<energyError<<std::endl;
        iterator.reset();
        N=0;
        energyError = 1.0;
        eps=1.0;
        while(eps > std::pow(10.0,-10) && N < maxIterations)
        {
            iterator.iterate(OutputMenu::FLOW_ENERGY_CONSTANT_STEP);
            eps = iterator.updateSupDelta();
            energyError = iterator.getEnergyError();
            energy = iterator.getEnergy();
            N += 1;
        }

        energy = iterator.getEnergy();
        std::cout<<"With length = "<< lengths[2] <<", iterations (constant) = "
                <<N<<", energy = "<<energy<<", energy error ="<<energyError<<std::endl;



        iterator.reset();
        N=0;
        energyError = 1.0;
        eps=1.0;




        while(eps > std::pow(10.0,-10) && N < maxIterations)
        {
            iterator.iterate(OutputMenu::FLOW_ENERGY_OPTIMAL_STEP);
            eps = iterator.updateSupDelta();
            energyError = iterator.getEnergyError();
            energy = iterator.getEnergy();
            N += 1;
        }
        energy = iterator.getEnergy();
        std::cout<<"With length = "<< lengths[2] <<", iterations (optimal) = "
                <<N<<", energy = "<<energy<<", energy error ="<<energyError<<std::endl;
        lengths[2] += .1;







        std::cout<<std::endl;

    }



    /*
    double optimalStep;

    std::cout<<"CAUGHT: Energy increased.  New energy = "<<energy<<
               ", energy error = "<<energyError<<", N = "<<N<<std::endl;
    iterator.undoIterate();
    iterator.iterate(OutputMenu::FLOW_ENERGY_CONSTANT_STEP);
    energyError = iterator.getEnergyError();
    energy = iterator.getEnergy();
    std::cout<<"New energy (constant step) would be = "<<energy<<
               ", energy error would be ="<<energyError<<std::endl;

    iterator.undoIterate();
    optimalStep = iterator.lineSearchTest();
    iterator.updateValuesEnergyGivenStep(optimalStep);
    energyError = iterator.getEnergyError();
    energy = iterator.getEnergy();
    std::cout<<"New energy (optimal step) seems to be = "<<energy<<
               ", energy error is ="<<energyError<<std::endl;
    */
}
