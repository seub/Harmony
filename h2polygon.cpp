#include "h2polygon.h"
#include "h2point.h"
#include "h2geodesic.h"
#include "circle.h"
#include "h2isometry.h"

H2Polygon::H2Polygon()
{
    vertices.clear();
    vertices.reserve(18);
}

H2Polygon::H2Polygon(std::vector<H2Point> vertices) : vertices(vertices)
{

}

void H2Polygon::addVertex(const H2Point &p)
{
    vertices.push_back(p);
}

void H2Polygon::removeLastVertex()
{
    vertices.pop_back();
}

void H2Polygon::clearVertices()
{
    vertices.clear();
}

void H2Polygon::setVertices(const std::vector<H2Point> &newVertices)
{
    vertices = newVertices;
}

uint H2Polygon::nbVertices() const
{
    return vertices.size();
}

H2Point H2Polygon::getVertex(uint index) const
{
    return vertices.at(index);
}

std::vector<H2Point> H2Polygon::getVertices() const
{
    return vertices;
}

std::vector<Complex> H2Polygon::getVerticesInDiskModel() const
{
    uint j, N = vertices.size();
    std::vector<Complex> res(N);

    for(j=0; j<N; ++j)
    {
        res[j] = vertices[j].getDiskCoordinate();
    }
    return res;
}

std::vector<Complex> H2Polygon::getVerticesInKleinModel() const
{
    uint i, N = vertices.size();
    std::vector<Complex> res(N);

    for (i=0; i<N; i++)
    {
        res[i] = vertices[i].getKleinCoordinate();
    }
    return res;
}


void H2Polygon::getExtremalCoordinatesInDiskModel(double &xMin, double &xMax, double &yMin, double &yMax) const
{
    xMin = 1.0;
    xMax = -1.0;
    yMin = 1.0;
    yMax = -1.0;
    
    double x, y;
    Complex z;
    for (const auto &p : vertices)
    {
        z = p.getDiskCoordinate();
        x = real(z);
        y = imag(z);
        if (x < xMin)
        {
            xMin = x;
        }
        if (x > xMax)
        {
            xMax = x;
        }
        if (y < yMin)
        {
            yMin = y;
        }
        if (y > yMax)
        {
            yMax = y;
        }
    }
}


bool H2Polygon::isPositivelyOriented() const
{
    std::vector<Complex> verticesInKleinModel = getVerticesInKleinModel();

    Complex z1 = verticesInKleinModel.back();
    Complex z2 = verticesInKleinModel.front();
    double x = 0.5*(real(z1) + real(z2)), y = 0.5*(imag(z1) + imag(z2));

    Complex w1, w2;
    double x1, x2, y1, y2;

    uint nbIntersections = 0;
    uint i, N = verticesInKleinModel.size();
    for (i=0; i+1!=N; ++i)
    {
        w1 = verticesInKleinModel[i];
        w2 = verticesInKleinModel[i+1];
        x1 = real(w1);
        y1 = imag(w1);
        x2 = real(w2);
        y2 = imag(w2);
        if (x1 == x2)
        {
        }
        else if (y1 > y && y2 > y)
        {
            if ((x1 <= x && x2 > x) || (x1 > x && x2 <= x))
            {
                nbIntersections++;
            }
        }
        else if (y1 > y || y2 > y)
        {
            if ((x1 <= x && x2 > x) || (x1 > x && x2 <= x))
            {
                if ((-x1*y2 + y1*x2 + (y2 - y1)*x)/(x2 - x1) > y)
                {
                    nbIntersections++;
                }
            }
        }
    }

    return (nbIntersections % 2 == 1) ^ (real(z1) > real(z2));
}

H2GeodesicArc H2Polygon::getSide(uint index) const
{
    uint N = vertices.size();
    if(index + 1 < N)
    {
        return H2GeodesicArc(vertices[index],vertices[index + 1]);
    }
    else if(index + 1 == N)
    {
        return H2GeodesicArc(vertices[index],vertices[0]);
    }
    else
    {
        throw(QString("ERROR in H2Polygon::isPositivelyOriented: wrong index"));
    }
}

std::vector<H2GeodesicArc> H2Polygon::getSides() const
{
    uint i, N = nbVertices();

    H2Point current, next = vertices[0];
    std::vector<H2GeodesicArc> res(N);

    for(i=0; i+1<N; ++i)
    {
        current = next;
        next = vertices[i+1];
        res[i] = H2GeodesicArc(current, next);
    }

    current = next;
    next = vertices.front();
    res[i] = H2GeodesicArc(current, next);

    return res;
}

std::vector<H2Geodesic> H2Polygon::getCompletedSides() const
{
    std::vector<H2GeodesicArc> sides = getSides();
    std::vector<H2Geodesic> res(sides.size());

    uint i=0;
    for(const auto &s : sides)
    {
        res[i] = s.getGeodesic();
        ++i;
    }
    return res;
}

double H2Polygon::norm0() const
{
    double res = 0;
    for (const auto &v : vertices)
    {
        res += norm(v.getDiskCoordinate());
    }
    return res;
}

double H2Polygon::norm1() const
{
    double res = norm(vertices.front().getDiskCoordinate() - vertices.back().getDiskCoordinate());
    for (uint i=0; i+1!=vertices.size(); ++i)
    {
        res += norm(vertices[i+1].getDiskCoordinate() - vertices[i].getDiskCoordinate());
    }
    return res;
}


double H2Polygon::norm2() const
{
    std::vector<Complex> verticesInKleinModel = getVerticesInKleinModel();
    double res = 0;
    Complex z1, z2, z3;
    z1= verticesInKleinModel.front();
    z2 = verticesInKleinModel.back();
    Complex u = conj(z2 - z1);
    

    for (const auto &z : verticesInKleinModel)
    {
        res += std::abs(imag((z - z1) *  u));
    }
    return 2.0/res;
}

double H2Polygon::norm3() const
{
    double average = norm1()/vertices.size();

    Complex current = vertices.back().getDiskCoordinate(), next = vertices[0].getDiskCoordinate();
    double diff = norm(next - current) - average;
    double res = diff*diff;
    
    for (uint i=0; i+1!=vertices.size(); ++i)
    {
        current = next;
        next = vertices[i+1].getDiskCoordinate();
        diff = norm(next - current) - average;
        res += diff*diff;
    }
    return res;
}

double H2Polygon::norm4() const
{
    double res = 0;
    for (const auto &v : vertices)
    {
        res += 1.0/(1.0-norm(v.getDiskCoordinate()));
    }
    return res;
}

void H2Polygon::optimalMobius(H2Isometry &output) const
{
    Complex zk;
    double rk;
    double A = 0.0;
    Complex T = 0.0;
    Circle Ck;
    PlanarLine Lk;
    
    std::vector<H2Geodesic> geodesics = getCompletedSides();
    
    uint k;
    for (k=0; k!=geodesics.size(); ++k)
    {
        if (geodesics[k].getCircleInDiskModel(Ck))
        {
            Ck.getCenterAndRadius(zk, rk);
            T += zk/rk;
            A += 2.0/rk;
        }
        else if (geodesics[k].getLineInDiskModel(Lk))
        {
            Complex nextVertex = vertices[(k+2)%vertices.size()].getDiskCoordinate();
            Complex direction = Lk.getDirection();
            if (imag(nextVertex*conj(direction))<0)
            {
                T += I*direction;
            }
            else
            {
                T += -I*direction;
            }
        }
        else
        {
            throw(QString("ERROR in H2Polygon::optimalMobius(): geodesic is neither circle nor line ?!"));
        }
    }
    
    
    Complex Tu = T/abs(T);
    double u = -abs(T);
    
    double B = 0.5*A/u;
    double delta = B*B - 1;
    
    if (delta < 0)
    {
        output.setIdentity();
    }
    else
    {
        double rac = sqrt(delta);
        double rho = -B - rac;
        output.setDiskCoordinates(1.0, rho*Tu);
    }
}

bool H2Polygon::constainsInDiskModel(const Complex &z) const
{
    if (norm(z)>1.0)
    {
        return false;
    }
    return containsInKleinModel(2.0*z/(1.0 + norm(z)));
}

bool H2Polygon::contains(const H2Point &point) const
{
    return containsInKleinModel(point.getKleinCoordinate());
}

bool H2Polygon::containsInKleinModel(const Complex &z) const
{
    uint nbIntersections = 0;
    double xLeft, yLeft, xRight, yRight;
    
    std::vector<Complex> verticesInKleinModel = getVerticesInKleinModel();
    uint n = verticesInKleinModel.size();
    
    if (norm(z)>1.0)
    {
        return false;
    }
    
    uint i;
    for (i=0; i!=n; ++i)
    {
        xLeft = real(verticesInKleinModel[i] - z);
        yLeft = imag(verticesInKleinModel[i]- z);
        xRight = real(verticesInKleinModel[i+1==n? 0 : i+1] - z);
        yRight = imag(verticesInKleinModel[i+1==n? 0 : i+1] - z);
        if (xLeft*xRight<0)
        {
            if (yLeft + xLeft*(yRight - yLeft)/(xLeft - xRight) > 0)
            {
                nbIntersections++;
            }
        }
        else if((xLeft<xRight?xLeft:xRight) == 0)
        {
            if (xLeft==0)
            {
                if (yLeft>0)
                {
                    nbIntersections++;
                }
            }
            else
            {
                if (yRight >0)
                {
                    nbIntersections++;
                }
            }
        }
    }
    return (nbIntersections%2 == 1);
}

std::vector<double> H2Polygon::getAngles() const
{
    std::vector<double> res;

    res.push_back(H2Point::angle(vertices.back(), vertices.front(), vertices[1]));
    for (uint i=1; i+1!=vertices.size(); i++)
    {
        res.push_back(H2Point::angle(vertices[i-1], vertices[i], vertices[i+1]));
    }
    res.push_back(H2Point::angle(vertices[vertices.size() - 2], vertices.back(), vertices.front()));
    
    return res;
}

std::vector<double> H2Polygon::getPositiveInteriorAngles() const
{
    std::vector<double> angles = getAngles();
    if (isPositivelyOriented())
    {
        for (auto &x : angles)
        {
            x = 2.0*M_PI - x;
        }
    }
    return angles;
}

bool H2Polygon::isConvex() const
{
    std::vector<Complex> verticesInKleinModel = getVerticesInKleinModel();
    uint i, N = verticesInKleinModel.size();

    Complex previous = verticesInKleinModel.back() - verticesInKleinModel.front(), next = verticesInKleinModel[1] - verticesInKleinModel.front();
    int sign = Tools::sign(imag(next*conj(previous)));

    for (i=1; i+1<N; i++)
    {
        previous = -next;
        next = verticesInKleinModel[i+1] - verticesInKleinModel[i];
        if (Tools::sign(imag(next*conj(previous))) != sign) return false;
    }

    previous = -next;
    next = verticesInKleinModel.front() - verticesInKleinModel.back();
    if (Tools::sign(imag(next*conj(previous))) != sign) return false;

    return true;
}

std::ostream & operator<<(std::ostream & out, const H2Polygon &P)
{
    for(const auto &v : P.vertices)
    {
        out << v << std::endl;
    }
    return out;
}

void H2Polygon::insertMidpoints()
{
    H2Point current, next;
    std::vector<H2Point> newVertices;
    current = vertices.front();
    for(std::vector<H2Point>::size_type i=0; i+1<vertices.size(); ++i)
    {
        next = vertices[i+1];
        newVertices.push_back(current);
        newVertices.push_back(H2Point::midpoint(current,next));
        current = next;
    }
    next = vertices[0];
    newVertices.push_back(current);
    newVertices.push_back(H2Point::midpoint(current,next));
    vertices = newVertices;
}

H2SteinerPolygon::H2SteinerPolygon(std::vector<H2Point> vertices, std::vector<uint> nbSteinerPoints) : H2Polygon(vertices), steinerWeights(nbSteinerPoints)
{
}

std::vector<H2Point> H2SteinerPolygon::getPointsOnSide(uint side) const
{
    H2GeodesicArc a = getSide(side);
    return a.getEvenSubdivision(steinerWeights[side]);
}


H2Polygon H2SteinerPolygon::getFullPolygon() const
{
    std::vector<H2Point> verticesOut, sidePoints;
    for(uint j=0; j<nbVertices(); ++j)
    {
        sidePoints = getPointsOnSide(j);
        verticesOut.insert(verticesOut.end(),sidePoints.begin(),sidePoints.end());
        verticesOut.pop_back();
    }
    H2Polygon output;
    output.setVertices(verticesOut);
    return output;
}

uint H2SteinerPolygon::getTotalNbSteinerPoints() const
{
    uint sum=0;
    for(const auto & num : steinerWeights)
    {
        sum += num;
    }
    return sum;
}

uint H2SteinerPolygon::getTotalNbPoints() const
{
    return getTotalNbSteinerPoints() + nbVertices();
}



uint H2SteinerPolygon::getNbSteinerPointsOnSide(uint side) const
{
    return steinerWeights[side];
}

uint H2SteinerPolygon::getIndexOfFullVertex(uint vertexIndex) const
{
    uint sum = 0;
    for(uint j=0; j!=vertexIndex; ++j)
    {
        sum += steinerWeights[j] + 1;
    }
    return sum;
}

std::vector<uint> H2SteinerPolygon::getVectorNbSteinerPoints() const
{
    return steinerWeights;
}

uint H2SteinerPolygon::getActualSide(uint vertexInFullPolygon) const
{
    uint side = 0;
    uint actualVertexIndexInFullPolygon = 0;
    while (actualVertexIndexInFullPolygon <= vertexInFullPolygon)
    {
        actualVertexIndexInFullPolygon += steinerWeights[side] + 1;
        ++side;
    }
    return side-1;
}

bool H2SteinerPolygon::lieOnSameActualSide(uint vertexInFullPolygon1, uint vertexInFullPolygon2) const
{
    uint side1 = 0;
    uint actualVertexIndexInFullPolygon = 0;
    while (actualVertexIndexInFullPolygon <= vertexInFullPolygon1)
    {
        actualVertexIndexInFullPolygon += steinerWeights[side1] + 1;
        ++side1;
    }
    --side1;

    uint vertex1 = actualVertexIndexInFullPolygon - (steinerWeights[side1] + 1);

    uint side2 = 0;
    actualVertexIndexInFullPolygon = 0;
    while (actualVertexIndexInFullPolygon <= vertexInFullPolygon2)
    {
        actualVertexIndexInFullPolygon += steinerWeights[side2] + 1;
        ++side2;
    }
    --side2;
    uint vertex2 = actualVertexIndexInFullPolygon - (steinerWeights[side2] + 1);

    if (vertexInFullPolygon1 == vertex1)
    {
        return ((side2 == side1) || ((side1 == 0) && (side2 == nbVertices()-1)) || ((side1 != 0) && (side2 == side1-1)) || ((vertexInFullPolygon2 == vertex2) && (side2 == side1+1)));
    }
    else if (vertexInFullPolygon2 == vertex2)
    {
        return ((side1 == side2) || ((side2 == 0) && (side1 == nbVertices()-1)) || ((side2 != 0) && (side1 == side2-1)));
    }
    else
    {
        return side1 == side2;
    }
}
