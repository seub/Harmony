#include "h2tangentvector.h"
#include "h2isometry.h"

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

H2Point H2TangentVector::exponentiateOld() const
{
    double t=length();
    Complex z1 = tanh(t/2)*vector/std::abs(vector);
    // NB: When translating to the origin via the map (z -z0)/(1 - bar(z0) z), 'vector' is only scaled by a positive factor.

    Complex z0 = root.getDiskCoordinate();
    Complex z2 = (z1 + z0)/(1.0 + conj(z0)*z1);

    assert (norm(z2)<1.0);


    return H2Point::fromDiskCoordinate(z2);
}

H2Point H2TangentVector::exponentiate() const
{
    return exponentiate(1.0);
}

H2Point H2TangentVector::exponentiate(const double & t) const
{
    double L,lambda,absZ,absV;
    Complex z,v,outputV;
    H2Point output;
    z = root.getDiskCoordinate();
    v = vector;
    absZ = std::abs(z);
    absV = std::abs(v);
    L = std::abs(t)*length();

    Complex u = t >= 0 ? v/absV : -1.0*v/absV;
    //lambda = (((1.0-absZ)*exp(L/2.0)) - (1.0+absZ)*exp(-L/2.0))/(((1.0-absZ)*exp(L/2.0)) + (1.0+absZ)*exp(-L/2.0));
    lambda = ((1.0-absZ) - ((1.0+absZ)*exp(-1.0*L)))/((1.0-absZ) + ((1.0+absZ)*exp(-1.0*L)));
    outputV = ((z*((lambda*absZ) + 1.0))+(u*(lambda+absZ)))/((lambda*absZ+1.0)+(u*conj(z)*(lambda+absZ)));
    output.setDiskCoordinate(outputV);
    return output;
}

H2Point H2TangentVector::exponentiate(const double &t, const H2TangentVector &v)
{
    return v.exponentiate(t);
}

std::vector<H2Point> H2TangentVector::exponentiate(const double &t, const std::vector<H2TangentVector> &V)
{
    std::vector<H2Point> out;
    out.reserve(V.size());

    for (const auto & v : V)
    {
        out.push_back(v.exponentiate(t));
    }

    return out;
}

H2TangentVector H2TangentVector::parallelTransportOld(const double &t) const
{
    H2Point y0 = root;
    H2Point yt = exponentiate(t);
    H2Isometry f;
    f.setDiskCoordinates(Complex(1.0,0.0), y0.getDiskCoordinate());

    H2TangentVector u(f*yt, (1.0 - norm((f*yt).getDiskCoordinate())) * ((f*(*this)).vector));

    return (f.inverse())*u;
}


H2TangentVector H2TangentVector::parallelTransport(const double &t) const
{
    H2Point yt = exponentiate(t);
    Complex v,outV;
    double L,absV;

    Complex z = root.getDiskCoordinate();
    v = vector;
    absV = std::abs(v);
    // Absolute value t? NO.
    L = t*length();

    Complex u = v/absV;
    Complex thing = cosh(L/2)+u*conj(z)*sinh(L/2);
    outV = v/(thing*thing);

    H2TangentVector output(yt,outV);
    return output;

}



double H2TangentVector::length() const
{
    return (2.0/(1.0- norm(root.getDiskCoordinate())))*std::abs(vector);
}

double H2TangentVector::lengthSquared() const
{
    double d = (1.0- norm(root.getDiskCoordinate()));
    return (4.0/(d*d))*norm(vector);
}


double H2TangentVector::scalProd(const H2TangentVector &v1, const H2TangentVector &v2)
{
    assert(v1.root == v2.root);
    double d = (1.0- norm(v1.root.getDiskCoordinate()));
    return (4.0/(d*d))*real(v1.vector*conj(v2.vector));
}

double H2TangentVector::scalProd(const std::vector<H2TangentVector> &V1, const std::vector<H2TangentVector> &V2)
{
    assert(V1.size() == V2.size());
    double out = 0.0;
    for (uint j=0;j<V1.size();++j)
    {
        out += scalProd(V1[j],V2[j]);
    }

    return out;
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

H2TangentVector operator*(const double &t, const H2TangentVector &v)
{
    return H2TangentVector(v.root,t*v.vector);
}

H2TangentVector operator*(const H2Isometry &f, const H2TangentVector &v)
{
    Complex u, a;
    f.getDiskCoordinates(u, a);
    Complex z = v.root.getDiskCoordinate();
    Complex df = u*(1 - norm(a))/((1.0 - conj(a)*z)*(1.0 - conj(a)*z));
    return H2TangentVector(f*v.root, df*v.vector);
}

H2TangentVector operator+(const H2TangentVector & v1, const H2TangentVector & v2)
{
    if (!(v1.root == v2.root))
    {
        std::cout << "v1.root = " << v1.root << ", v2.root = " << v2.root << std::endl;
    }
    //assert (v1.root == v2.root);
    return H2TangentVector(v1.root,v1.vector+v2.vector);
}


H2TangentVector H2TangentVector::parallelTransport(const double &t, const H2TangentVector &v)
{
    return v.parallelTransport(t);
}

std::vector<H2TangentVector> H2TangentVector::parallelTransport(const double &t, const std::vector<H2TangentVector> &V)
{
    std::vector<H2TangentVector> out;
    out.reserve(V.size());
    for (const auto & v : V)
    {
        out.push_back(v.parallelTransport(t));
    }
    return out;
}




