#include "mainapplication.h"
#include "h2polygontriangulater.h"
#include "tools.h"
#include "h2point.h"
#include "h2tangentvector.h"
#include "fenchelnielsenconstructor.h"
#include "outputmenu.h"

int main(int argc, char *argv[])
{



    return MainApplication(argc, argv).exec();

    DiscreteGroup Gamma(TopologicalSurface(2, 0));

    GroupRepresentation<H2Isometry> rhoDomain(Gamma);
    GroupRepresentation<H2Isometry> rhoImage(Gamma);



    std::vector<double> lengths = {2.0,2.0,2.5};
    std::vector<double> twists = {-1.5, 2.0, .5};
    FenchelNielsenConstructor FN(lengths,twists);


    rhoDomain = FN.getRepresentation();
    rhoImage.setNiceRepresentation();


    LiftedGraphFunctionTriangulated<H2Point, H2Isometry> graph(rhoDomain, rhoImage, 4);
    DiscreteFlowIterator<H2Point,H2Isometry> iterator(& graph);

    double eps = 1.0;
    uint N = 0;


/*
    for(uint i=0; i<24; i++)
    {
        eps=1.0;
        while(eps > std::pow(10.0,-10))
        {
            iterator.iterate(OutputMenu::FLOW_CENTROID);
            eps = iterator.updateSupDelta();
            std::cout<<"eps = "<<eps<<std::endl;
            N += 1;
        }
        std::cout<<"With i ="<<i<<", the number of iterations for constant step = "<<N<<std::endl;
        std::cout<<std::endl;

        lengths[3] += .1;
        FenchelNielsenConstructor FN(lengths,twists);
        rhoDomain = FN.getRepresentation();
        LiftedGraphFunctionTriangulated<H2Point, H2Isometry> graph(rhoDomain, rhoImage, 4);
        DiscreteFlowIterator<H2Point,H2Isometry> iterator(& graph);
    }
*/
/*
    eps=1.0;
    while(eps > std::pow(10.0,-10))
    {
        iterator.iterate(OutputMenu::FLOW_ENERGY_CONSTANT_STEP);
        eps = iterator.updateSupDelta();
        std::cout<<"eps = "<<eps<<std::endl;
        N += 1;
    }
    std::cout<<"The number of iterations for constant step = "<<N<<std::endl;
    std::cout<<std::endl;
*/


/*
    for(uint i=0;i<24;i++)
    {
        eps=1.0;
        while(eps > std::pow(10.0,-10))
        {
            iterator.iterate(OutputMenu::FLOW_ENERGY_OPTIMAL_STEP);
            eps=iterator.updateSupDelta();
            N+=1;
        }
        std::cout<<"With i ="<<i<<", the number of iterations for optimal step = "<<N<<std::endl;
        std::cout<<std::endl;

        lengths[3] += .1;
        FenchelNielsenConstructor FN(lengths,twists);
        rhoDomain = FN.getRepresentation();
        LiftedGraphFunctionTriangulated<H2Point, H2Isometry> graph(rhoDomain, rhoImage, 4);
        DiscreteFlowIterator<H2Point,H2Isometry> iterator(& graph);
    }
*/

}
