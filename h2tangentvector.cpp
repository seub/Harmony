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
    root = from;
    vector = distance*u*(1.0-norm(fromZ))/(2.0*std::abs(u));
}

H2TangentVector::H2TangentVector(const H2Point &root) : root(root)
{
    vector = Complex(0.0,0.0);
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

Complex H2TangentVector::getVector() const
{
    return vector;
}

std::ostream & operator<<(std::ostream & out, const H2TangentVector & vec)
{
    out << "H2TangentVector has root " << vec.getRoot() << " and tangent vector " << vec.getVector();
    return out;
}

H2TangentVector operator*(const double & scale, const H2TangentVector & vec)
{
    return H2TangentVector(vec.root,scale*vec.vector);
}

H2TangentVector operator+(const H2TangentVector & v1, const H2TangentVector & v2)
{
    assert (v1.root == v2.root);
    return H2TangentVector(v1.root,v1.vector+v2.vector);
}
