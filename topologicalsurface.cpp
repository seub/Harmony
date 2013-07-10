#include "topologicalsurface.h"
#include "discretegroup.h"


TopologicalSurface::TopologicalSurface()
{
}

TopologicalSurface::TopologicalSurface(int genus, int numberOfPunctures) : genus(genus), numberOfPunctures(numberOfPunctures)
{
}

int TopologicalSurface::getGenus() const
{
    return genus;
}

int TopologicalSurface::getNumberOfPunctures() const
{
    return numberOfPunctures;
}

/*
std::vector<DiscreteGroup> TopologicalSurface::getPantsDecomposition() const
{
    std::vector<DiscreteGroup> pantsVector;
    DiscreteGroup P;
    P.setPairOfPants("gamma1down","alpha1up","gamma1up");
    pantsVector.push_back(P);
    for(unsigned int j=0;j<genus-2;j++)
    {
        std::string jString1,jString2;
        std::stringstream out1,out2;
        out1 << j + 1;
        jString1 = out1.str();
        out2 << j + 2;
        jString2 = out2.str();

        P.setPairOfPants("alpha"<<jString1<<"down","gamma"<<jString2<<"up","beta"<<jString1<<"up");
        pantsVector.push_back(P);

        P.setPairOfPants("alpha"<<jString2<<"up","beta"<<jString1<<"down","gamma"<<jString2<<"down");
        pantsVector.push_back(P);
    }
    std::string jString1,jString2;
    std::stringstream out1,out2;
    out1 << genus - 1;
    jString1 = out1.str();
    out2 << genus;
    jString2 = out2.str();
    P.setPairOfPants("alpha"<<jString1<<"down","gamma"<<jString2<<"down","gamma"<<jString2<<"up");
    pantsVector.push_back(P);
    return pantsVector;
}
*/
