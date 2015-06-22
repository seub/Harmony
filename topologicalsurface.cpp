#include "topologicalsurface.h"
#include "discretegroup.h"


TopologicalSurface::TopologicalSurface()
{
}

TopologicalSurface::TopologicalSurface(uint genus, uint numberOfPunctures) : genus(genus), numberOfPunctures(numberOfPunctures)
{
}

uint TopologicalSurface::getGenus() const
{
    return genus;
}

uint TopologicalSurface::getNumberOfPunctures() const
{
    return numberOfPunctures;
}

bool TopologicalSurface::isClosedHyperbolicSurface() const
{
    return ((genus > 1) && (numberOfPunctures==0));
}


std::vector<DiscreteGroup> TopologicalSurface::getPantsDecomposition() const
{
    assert(isClosedHyperbolicSurface());

    std::vector<DiscreteGroup> pantsVector;
    DiscreteGroup P;
    std::string s1,s2,w1,w2,w3;

    P.setPairOfPants("W1up","U1up","W1down");
    pantsVector.push_back(P);

    for(uint j=0; j+2<genus; ++j)
    {
        s1 = Tools::convertToString(j+1);
        s2 = Tools::convertToString(j+2);

        w1.append("U").append(s1).append("down");
        w2.append("V").append(s1).append("up");
        w3.append("W").append(s2).append("up");

        P.setPairOfPants(w1,w2,w3);
        pantsVector.push_back(P);

        w1.clear();
        w2.clear();
        w3.clear();

        w1.append("U").append(s2).append("up");
        w2.append("W").append(s2).append("down");
        w3.append("V").append(s1).append("down");

        P.setPairOfPants(w1,w2,w3);
        pantsVector.push_back(P);

        w1.clear();
        w2.clear();
        w3.clear();
    }
    s1 = Tools::convertToString(genus-1);
    s2 = Tools::convertToString(genus);

    w1.append("U").append(s1).append("down");
    w2.append("W").append(s2).append("up");
    w3.append("W").append(s2).append("down");

    P.setPairOfPants(w1,w2,w3);
    pantsVector.push_back(P);
    return pantsVector;
}

