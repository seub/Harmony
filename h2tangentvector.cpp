#include "h2tangentvector.h"

H2TangentVector::H2TangentVector()
{
}

H2TangentVector::H2TangentVector(const H2Point &root, const Complex &vector) : root(root), vector(vector)
{
}

H2TangentVector::H2TangentVector(const H2Point &from, const H2Point &to)
{
    Complex fromZ, toZ;
    fromZ = from.getDiskCoordinate();
    toZ = to.getDiskCoordinate();
    Complex u = (toZ - fromZ)/(1.0 - conj(fromZ)*toZ);
    double distance = H2Point::distance(from,to);
    vector = distance*u/std::abs(u);
    root = from;
}

H2Point H2TangentVector::exponentiate() const
{
    double t=length();

    Complex z1 = tanh(t/2)*vector/std::abs(vector);
    // NB: When translating to the origin via the map (z -z0)/(1 - bar(z0) z), 'vector' is only scaled by a positive factor.
    Complex z0 = root.getDiskCoordinate();
    Complex z2 = (z1 + z0)/(1.0 + conj(z0)*z1);
    return H2Point::fromDiskCoordinate(z2);
}

double H2TangentVector::length() const
{
    return (2.0/(1.0- norm(root.getDiskCoordinate())))*std::abs(vector);
}

H2Point H2TangentVector::getRoot() const
{
    return root;
}

std::ostream & operator<<(std::ostream & out, const H2TangentVector &p)
{
    out << "H2TangentVector with disk coordinate " << p;
    return out;
}
