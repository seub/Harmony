#include "topologicalsurface.h"
#include "discretegroup.h"


TopologicalSurface::TopologicalSurface(uint genus, uint numberOfPunctures) : genus(genus), nbPunctures(numberOfPunctures)
{
}

uint TopologicalSurface::getGenus() const
{
    return genus;
}

uint TopologicalSurface::getNumberOfPunctures() const
{
    return nbPunctures;
}

bool TopologicalSurface::isClosedHyperbolicSurface() const
{
    return ((genus > 1) && (nbPunctures==0));
}

bool TopologicalSurface::isHyperbolic() const
{
    return eulerCharacteristic() < 0;
}

int TopologicalSurface::eulerCharacteristic() const
{
    return 2 - 2*genus - nbPunctures;
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

bool TopologicalSurface::maxInjectivityRadius(double &out) const
{
    return maxInjectivityRadiusCalculator(*this).get(out);
}





maxInjectivityRadiusCalculator::maxInjectivityRadiusCalculator(const TopologicalSurface &surface)
{
    assert(surface.isHyperbolic());
    g = surface.getGenus();
    n = surface.getNumberOfPunctures();

    tol = 0.000000000000001;
}

bool maxInjectivityRadiusCalculator::get(double &out, int maxNbIterations) const
{
    double error = 1.0 + tol;
    double xk = x0(), previous = xk;

    int k=0;
    while((error>tol) && (k++ < maxNbIterations))
    {
        xk = newtonIterate(xk);
        error = std::abs(xk - previous);
        previous = xk;
    }
    out = xk;
    return error<tol;
}

double maxInjectivityRadiusCalculator::f(const double &x) const
{
    double sechx = 1.0/cosh(x);
    return -2*M_PI + 6*(4*g + n - 2)*asin(sechx/2.0) + 2*n*asin(sechx);
}

double maxInjectivityRadiusCalculator::df(const double &x) const
{
    double coshx2 = cosh(x), sinhx = sinh(x);
    coshx2 = coshx2*coshx2;
    double rad1 = sqrt(1 - 0.25/coshx2);
    double rad2 = sqrt(1 - 1.0/coshx2);
    return -sinhx*(3*(4*g+n-2)/rad1 + 2*n/rad2)/coshx2;
}

double maxInjectivityRadiusCalculator::x0() const
{
    return 1.0;
}

double maxInjectivityRadiusCalculator::newtonIterate(const double &xk) const
{
    return xk - f(xk)/df(xk);
}
