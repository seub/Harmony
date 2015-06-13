#include "h2point.h"
#include "h2isometry.h"
#include <Eigen/Dense>

H2Point::H2Point()
{
}

Complex H2Point::getUpperHalfPlaneCoordinate() const
{
    Complex I(0.0, 1.0);
    return -I*((z + I)/(z - I));
}

Complex H2Point::getDiskCoordinate() const
{
    return z;
}

Complex H2Point::getKleinCoordinate() const
{
    //    complex z = getDiskCoordinate();
    return 2.0*z / (1.0 + norm(z));
}

Complex H2Point::getHyperboloidProjection() const
{
    return 2.0*z / (1.0 - norm(z));
}

void H2Point::getHyperboloidCoordinate(double &x, double &y, double &z) const
{
    double s = 1.0 - norm(this->z);
    z = (2.0 / s) - 1.0;
    x = (2.0*real(this->z) / s);
    y = (2.0*imag(this->z) / s);
    return;
}

void H2Point::setUpperHalfPlaneCoordinate(Complex z)
{
    Complex I(0.0, 1.0);
    this->z = I*((z - I)/(z + I));
    return;
}

void H2Point::setDiskCoordinate(Complex z)
{
    this->z = z;
    return;
}

void H2Point::setHyperboloidProjection(Complex z)
{
    this->z = z / (1.0 + sqrt(1.0 + norm(z)));
    return;
}

void H2Point::setKleinCoordinate(Complex z)
{
    this->z = z / (1.0 + sqrt(1.0 - norm(z)));
    return;
}

double H2Point::distance(const H2Point & p1, const H2Point & p2)
{
    double s;
    Complex z1,z2;
    z1 = p1.z;
    z2 = p2.z;
    s = 2.0 * (norm(z1 - z2) / ((1 - norm(z1))*(1 - norm(z2))));

    return acosh(1.0 + s);
}

H2Point H2Point::midpoint(const H2Point &p1, const H2Point &p2)
{
    Complex w1 = (p2.z - p1.z)/(1.0 - conj(p1.z)*p2.z);
    Complex w2 = w1*sqrt((1.0 - sqrt(1.0 - norm(w1)))/(norm(w1)*(1.0 + sqrt(1.0 - norm(w1)))));

    H2Point res;
    res.setDiskCoordinate((w2 + p1.z)/(1.0 + conj(p1.z)*w2));
    return res;
}

H2Point H2Point::proportionalPoint(const H2Point &p1, const H2Point &p2, const double &s)
{
    // assert that 0 < s < 1
    Complex w1 = (p2.z - p1.z)/(1.0 - conj(p1.z)*p2.z);
    double d = log ((1+abs(w1))/(1-abs(w1)));
    Complex w2 = w1*(exp(s*d) - 1.0)/(abs(w1)*(exp(s*d) + 1.0));

    H2Point res;
    res.setDiskCoordinate((w2 + p1.z)/(1.0 + conj(p1.z)*w2));
    return res;
}

double H2Point::angle(const H2Point &previous, const H2Point &point, const H2Point &next)
{
    Complex u = (previous.z - point.z) / (1.0 - conj(point.z)*previous.z);
    Complex v = (next.z - point.z) / (1.0 - conj(point.z)*next.z);
    return Tools::mod2Pi(arg(v*conj(u)));
}

double H2Point::tanHalfAngle(const H2Point &previous, const H2Point &point, const H2Point &next)
{
    Complex u = (previous.z - point.z) / (1.0 - conj(point.z)*previous.z);
    Complex v = (next.z - point.z) / (1.0 - conj(point.z)*next.z);
    Complex zed = v*conj(u) / abs(v*conj(u));
    double x = real(zed), y = imag(zed);
    return (1-x)/y;
}

void H2Point::computeAffineWeights(const std::vector<H2Point> &neighbors, std::vector<double> &outputWeights) const
{
    H2Point previous, current, next;
    std::vector<double> preNeighborWeights;
    double r, tan1, tan2, sum;
    std::vector<double> distances;

    outputWeights.clear();
    previous = neighbors.back();
    current = neighbors.front();
    next = neighbors[1];
    for(std::vector<H2Point>::size_type j=1; j+1<neighbors.size(); ++j)
    {
        r=H2Point::distance(*this, current);
        distances.push_back(r);
        tan1=H2Point::tanHalfAngle(current, *this, next);
        tan2=H2Point::tanHalfAngle(previous, *this, current);
        preNeighborWeights.push_back((1.0/(3*M_PI*r))*(tan1 + tan2));
        previous = current;
        current = next;
        next = neighbors[j+1];
    }
    r=H2Point::distance(*this, current);
    distances.push_back(r);
    tan1=H2Point::tanHalfAngle(current, *this, next);
    tan2=H2Point::tanHalfAngle(previous, *this, current);
    preNeighborWeights.push_back((1.0/(3*M_PI*r))*(tan1 + tan2));
    previous = current;
    current = next;
    next = neighbors.front();
    r=H2Point::distance(*this, current);
    distances.push_back(r);
    tan1=H2Point::tanHalfAngle(current, *this, next);
    tan2=H2Point::tanHalfAngle(previous, *this, current);
    preNeighborWeights.push_back((1.0/(3*M_PI*r))*(tan1 + tan2));

    sum = 0.0;
    for (auto x : preNeighborWeights)
    {
        sum += x;
    }
    double epsilon = 1/sum;

    for (auto preweight : preNeighborWeights)
    {
        outputWeights.push_back(epsilon*preweight);
    }
}

void H2Point::computeNaiveWeights(const std::vector<H2Point> &neighbors, std::vector<double> &outputWeights) const
{
    H2Point previous, current, next;
    double r, sum;
    std::vector<double> distances, angles;

    outputWeights.clear();
    previous = neighbors.back();
    current = neighbors.front();
    next = neighbors[1];
    for(std::vector<H2Point>::size_type j=1; j+1<neighbors.size(); ++j)
    {
        r=H2Point::distance(*this, current);
        distances.push_back(r);
        angles.push_back(H2Point::angle(previous, *this, next)/2.0);

        previous = current;
        current = next;
        next = neighbors[j+1];
    }
    r=H2Point::distance(*this, current);
    distances.push_back(r);
    angles.push_back(H2Point::angle(previous, *this, next)/2.0);

    previous = current;
    current = next;
    next = neighbors.front();
    r=H2Point::distance(*this, current);
    distances.push_back(r);
    angles.push_back(H2Point::angle(previous, *this, next)/2.0);

    for (std::vector<double>::size_type l=0; l<angles.size(); ++l)
    {
        outputWeights.push_back(angles[l]/(2*M_PI*distances[l]));
    }

    sum = 0.0;
    for (auto neighborWeight : outputWeights)
    {
        sum += neighborWeight;
    }
    for (auto & neighborWeight : outputWeights)
    {
        neighborWeight = neighborWeight/sum;
    }
}

void H2Point::computeQuadraticWeights(const std::vector<H2Point> &neighbors, std::vector<double> &outputWeights) const
{
    if (neighbors.size() != 6)
    {
        std::cout << "Error in H2Point::computeQuadraticWeights: there needs to be 6 neighbors (you gave me "
                     << neighbors.size() << ")" << std::endl;
    }

    std::vector<double> neighborsInTangentSpaceX, neighborsInTangentSpaceY;
    double d, r, xj, yj;
    Complex z;
    using namespace Eigen;
    MatrixXd m(6,6);
    VectorXd vec(6), out(6);


    H2Isometry A;
    outputWeights.clear();
    A.setByMappingPointTo0(*this);
    std::vector<H2Point> translatedNeighbors = A*neighbors;

    double epsilon = -1.0;
    for(const auto & point : translatedNeighbors)
    {
        z = point.getDiskCoordinate();
        d = abs(z);
        r = log((1.0+d)/(1.0-d));
        epsilon = ((epsilon < r) && epsilon > 0) ? epsilon : r;
        neighborsInTangentSpaceX.push_back(r*real(z)/d);
        neighborsInTangentSpaceY.push_back(r*imag(z)/d);
    }


    epsilon *= 0.1;

    vec(0) = 1; vec(1) = 0; vec(2) = 0; vec(3) = 0;
    vec(4) = epsilon*epsilon/4.0; vec(5) = epsilon*epsilon/4.0;

    std::cout << "List of angles:";
    for(int j=0; j<6; ++j)
    {
        xj = neighborsInTangentSpaceX[j];
        yj = neighborsInTangentSpaceY[j];
        m(0,j) = 1;
        m(1,j) = xj;
        m(2,j) = yj;
        m(3,j) = xj*yj;
        m(4,j) = xj*xj;
        m(5,j) = yj*yj;
        std::cout << atan2(yj, xj) << ", ";
    }
    std::cout << std::endl;

    out = m.colPivHouseholderQr().solve(vec);

    for(int j=0; j<6; ++j)
    {
        outputWeights.push_back(out(j));
    }


    //Tests
    int counter=0;
    std::cout << "List of weights: ";
    for(int j=0; j<6; ++j)
    {
        if (out(j)<0.0)
        {
            ++counter;
        }
        std::cout << out(j) << ", ";
    }
    std::cout << std::endl;
    std::cout << "Number of negative weights:" << counter << std::endl;


    /*sum=0.0;
    for(int j=0; j<6; ++j)
    {
        sum += out(j)*neighborsInTangentSpaceX[j];
    }
    std::cout << "A mesh point has weighted sum of X coordinates " << sum << std::endl;

    sum=0.0;
    for(int j=0; j<6; ++j)
    {
        sum += out(j)*neighborsInTangentSpaceY[j];
    }
    std::cout << "A mesh point has weighted sum of Y coordinates " << sum << std::endl;

    sum=0.0;
    for(int j=0; j<6; ++j)
    {
        sum += out(j)*neighborsInTangentSpaceX[j]*neighborsInTangentSpaceY[j];
    }
    std::cout << "A mesh point has weighted sum of X*Y coordinates " << sum << std::endl;

    sum=0.0;
    for(int j=0; j<6; ++j)
    {
        sum += out(j)*neighborsInTangentSpaceX[j]*neighborsInTangentSpaceX[j];
    }
    std::cout << "A mesh point has weighted sum of X^2 coordinates " << sum-(epsilon*epsilon/4.0) << std::endl;

    sum=0.0;
    for(int j=0; j<6; ++j)
    {
        sum += out(j)*neighborsInTangentSpaceY[j]*neighborsInTangentSpaceY[j];
    }
    std::cout << "A mesh point has weighted sum of Y^2 coordinates " << sum-(epsilon*epsilon/4.0) << std::endl;

    sum = 0.0;
    for (auto neighborWeight : outputWeights)
    {
        sum += neighborWeight;
    }
    std::cout << "sum of weights = " << sum << std::endl;*/
}







std::ostream & operator<<(std::ostream & out, const H2Point &p)
{
    out << "H2Point with disk coordinate " << p.z;
    return out;
}

bool operator ==(H2Point & p1, H2Point & p2)
{
    return p1.z == p2.z;
}

H2Point H2Point::centroid(const std::vector<H2Point> &points, const std::vector<double> &weights)
{
    //assert sum weights = 1

    if (points.size() != weights.size())
    {
        std::cout << "error in centroid" << std::endl;
    }

    std::vector<double> X,Y,Z;
    double a,b,c,xout=0.0,yout=0.0,zout=0.0;
    H2Point out;
    X.reserve(points.size());
    Y.reserve(points.size());
    Z.reserve(points.size());
    for(const auto & p: points)
    {
        p.getHyperboloidCoordinate(a,b,c);
        X.push_back(a);
        Y.push_back(b);
        Z.push_back(c);
    }

    for(unsigned int j=0; j<points.size(); ++j)
    {
        xout += weights[j]*X[j];
        yout += weights[j]*Y[j];
        zout += weights[j]*Z[j];
    }
    double s = 1/ sqrt(zout*zout - xout*xout - yout*yout);
    xout = s*xout;
    yout = s*yout;
    out.setHyperboloidProjection(Complex(xout,yout));
    return out;
}

bool H2Point::compareAngles(const H2Point &p1, const H2Point &p2)
{
    Complex z1 = p1.z - z;
    Complex z2 = p2.z - z;
    double x1,x2,y1,y2;
    x1 = real(z1); y1 = imag(z1);
    x2 = real(z2); y2 = imag(z2);
    if (x1>=0.0)
    {
        if (y1>=0.0)
        {
            if (x2>=0.0 && y2>=0.0)
            {
                return (y1*x2 < x1*y2);
            }
            else return true;
        }
        else return (x2>0.0 && y2<0.0 && (y1*x2 < x1*y2));
    }
    else if (x2<0.0)
    {
        return (y1*x2 < x1*y2);
    }
    else return (y2<0.0);
}

bool compareTriples(const triple &t1, const triple &t2)
{
    //assert that std::get<0>(t1) and std::get<0>(t2) are the same.
    H2Point p(std::get<0>(t1)), p1(std::get<1>(t1)), p2(std::get<1>(t2));
    return p.compareAngles(p1, p2);
}

//bool compareQuadruples(const quadruple &q1, const quadruple &q2)
//{
//assert that std::get<0>(q1) and std::get<0>(q2) are the same.
//    return std::get<0>(q1).compareAngles(std::get<1>(q1),std::get<1>(q2));
//}
