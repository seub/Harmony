#include "h2polygon.h"
#include "h2point.h"
#include "h2geodesic.h"
#include "circle.h"
#include "h2isometry.h"

H2Polygon::H2Polygon()
{
    vertices.clear();
    vertices.reserve(12);
}

void H2Polygon::addVertex(const H2Point &p)
{
    vertices.push_back(p);
    verticesInKleinModel.push_back(p.getKleinCoordinate());
    return;
}

void H2Polygon::removeLastVertex()
{
    vertices.pop_back();
    verticesInKleinModel.pop_back();
    return;
}

void H2Polygon::clearVertices()
{
    vertices.clear();
    verticesInKleinModel.clear();
    return;
}

int H2Polygon::getNumberOfVertices() const
{
    return vertices.size();
}

H2Point H2Polygon::getVertex(int index) const
{
    return vertices[index];
}

std::vector<H2Point> H2Polygon::getVertices() const
{
    return vertices;
}

std::vector<complex> H2Polygon::getVerticesInDiskModel() const
{
    int sizeList = getNumberOfVertices();
    std::vector<complex> complexList(sizeList);
    for(int j=0;j<=sizeList - 1;j++)
    {
        complexList[j] = vertices[j].getDiskCoordinate();
    }
    return complexList;
}

std::vector<complex> H2Polygon::getVerticesInKleinModel() const
{
    return verticesInKleinModel;
}

std::vector<complex> H2Polygon::getVerticesInHyperboloidProjection() const
{
    std::vector<complex> output;
    for (unsigned int i=0; i<vertices.size(); i++)
    {
        output.push_back(vertices[i].getHyperboloidProjection());
    }
    return output;
}

/*void H2Polygon::getExtremalCoordinatesInHyperboloidProjection(double &xMin, double &yMin, double &xMax, double &yMax) const
{
    complex z = vertices[0].getHyperboloidProjection();
    xMin = real(z);
    xMax = real(z);
    yMin = imag(z);
    yMax = imag(z);
    double xmin,xmax,ymin,ymax;
    std::vector<H2GeodesicArc> sides = getSides();
    for (unsigned int j=0; j<vertices.size(); j++)
    {
        sides[j].getExtremalCoordinatesInHyperboloidProjection(xmin,xmax,ymin,ymax);
        xMin = std::min(xmin,xMin);
        yMin = std::min(ymin,yMin);
        xMax = std::max(xmax,xMax);
        yMax = std::max(ymax,yMax);
    }
    re*/

void H2Polygon::getExtremalCoordinatesInHyperboloidProjection(double &xMin, double &xMax, double &yMin, double &yMax) const
{
    complex z = vertices[0].getHyperboloidProjection();
    xMin = real(z);
    xMax = real(z);
    yMin = imag(z);
    yMax = imag(z);
    for (unsigned int j=1; j<vertices.size(); j++)
    {
        z = vertices[j].getHyperboloidProjection();
        //std::cout << "z = " << z << std::endl;
        if (real(z) < xMin)
        {
            xMin = real(z);
        }
        if (imag(z) < yMin)
        {
            yMin = imag(z);
        }
        if (real(z) > xMax)
        {
            xMax = real(z);
        }
        if (imag(z) > yMax)
        {
            yMax = imag(z);
        }
        //std::cout << "xMin, xMax, yMin, yMax: " << xMin << ", " << xMax << ", " << yMin << ", " << yMax << std::endl;
    }
    return;
}

void H2Polygon::getExtremalCoordinatesInDiskModel(double &xMin, double &xMax, double &yMin, double &yMax) const
{
    xMin = 1.0;
    xMax = -1.0;
    yMin = 1.0;
    yMax = -1.0;
    
    double x, y;
    complex z;
    for (unsigned int i=0; i<vertices.size(); i++)
    {
        z = vertices[i].getDiskCoordinate();
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
    return;
}

bool H2Polygon::isPositivelyOriented() const
{
    complex z1 = verticesInKleinModel.back();
    complex z2 = verticesInKleinModel.front();
    double x = 0.5*(real(z1) + real(z2)), y = 0.5*(imag(z1) + imag(z2));

    complex w1, w2;
    double x1, x2, y1, y2;

    int nbIntersections = 0;
    unsigned int i;
    for (i=0; i+1<verticesInKleinModel.size(); i++)
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

H2GeodesicArc H2Polygon::getSide(int index) const
{
    int sizeList = getNumberOfVertices();
    if(index < sizeList - 1)
    {
        return H2GeodesicArc(vertices[index],vertices[index + 1]);
    }
    else if(index == sizeList - 1)
    {
        return H2GeodesicArc(vertices[index],vertices[0]);
    }
    else
    {
        std::cout << "Whoa this polygon doesn't have that many sides!" << std::endl;
        throw(0);
    }
}

std::vector<H2GeodesicArc> H2Polygon::getSides() const
{
    int sizeList = getNumberOfVertices();
    std::vector<H2GeodesicArc> arcList(sizeList);
    for(int j=0;j<=sizeList - 1;j++)
    {
        arcList[j] = getSide(j);
    }
    return arcList;
}

std::vector<H2Geodesic> H2Polygon::getCompletedSides() const
{
    int sizeList = getNumberOfVertices();
    std::vector<H2Geodesic> arcList(sizeList);
    for(int j=0;j<=sizeList - 1;j++)
    {
        arcList[j] = getSide(j).getGeodesic();
    }
    return arcList;
}

double H2Polygon::norm0() const
{
    double res = 0;
    for (unsigned int i=0; i<vertices.size(); i++)
    {
        res += norm(vertices[i].getDiskCoordinate());
    }
    return res;
}

double H2Polygon::norm1() const
{
    double res = norm(vertices.front().getDiskCoordinate() - vertices.back().getDiskCoordinate());
    for (unsigned int i=0; i<vertices.size()-1; i++)
    {
        res += norm(vertices[i+1].getDiskCoordinate() - vertices[i].getDiskCoordinate());
    }
    return res;
}

double H2Polygon::norm2() const
{
    double res = 0;
    complex z1, z2, z3;
    z1= vertices.front().getKleinCoordinate();
    z2 = vertices.back().getKleinCoordinate();
    complex u = conj(z2 - z1);
    
    
    unsigned int i;
    for (i=0 ; i <vertices.size()-1; i++)
    {
        z3 = vertices[i].getKleinCoordinate();
        res += std::abs(imag((z3 - z1) *  u));
    }
    return 2.0/res;
}

double H2Polygon::norm3() const
{
    double average = norm1()/vertices.size();
    
    double diff = norm(vertices.front().getDiskCoordinate() - vertices.back().getDiskCoordinate()) - average;
    double res = diff*diff;
    
    for (unsigned int i=1; i<vertices.size()-1; i++)
    {
        diff = norm(vertices[i+1].getDiskCoordinate() - vertices[i].getDiskCoordinate()) - average;
        res += diff*diff;
    }
    return res;
}

double H2Polygon::norm4() const
{
    double res = 0;
    for (unsigned int i=0; i<vertices.size(); i++)
    {
        res += 1.0/(1.0-norm(vertices[i].getDiskCoordinate()));
    }
    return res;
}

void H2Polygon::optimalMobius(H2Isometry &output) const
{
    complex zk;
    double rk;
    double A = 0.0;
    complex T = 0.0;
    Circle Ck;
    PlanarLine Lk;
    
    
    std::vector<H2Geodesic> geodesics = getCompletedSides();
    
    unsigned int k;
    for (k=0; k<geodesics.size(); k++)
    {
        if (geodesics[k].getCircleInDiskModel(Ck))
        {
            Ck.getCenterAndRadius(zk, rk);
            T += zk/rk;
            A += 2.0/rk;
        }
        else if (geodesics[k].getLineInDiskModel(Lk))
        {
            complex nextVertex = vertices[(k+2)%vertices.size()].getDiskCoordinate();
            complex direction = Lk.getDirection();
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
            std::cout << "ERROR in H2Polygon::optimalMobius(): geodesic is neither circle nor line ?!" << std::endl;
        }
    }
    
    
    complex Tu = T/abs(T);
    double u = -abs(T);
    
    double B = 0.5*A/u;
    double delta = B*B - 1;
    
    if (delta < 0)
    {
        //std::cout << "ERROR in Graphe<Circle>::optimal_mobius(): negative discriminant!" << std::endl;
        output.setIdentity();
    }
    else
    {
        double rac = sqrt(delta);
        double rho = -B - rac;
        output.setDiskCoordinates(1.0, rho*Tu);
    }
}

bool H2Polygon::isInsideInDiskModel(const complex &z) const
{
    if (norm(z)>1.0)
    {
        return false;
    }
    complex w = 2.0*z/(1.0 + norm(z));
    return isInsideInKleinModel(w);
}

bool H2Polygon::isInside(const H2Point &point) const
{
    complex w = point.getKleinCoordinate();
    return isInsideInKleinModel(w);
}

bool H2Polygon::isInsideInKleinModel(const complex &z) const
{
    unsigned int nbIntersections = 0;
    double xLeft, yLeft, xRight, yRight;
    
    unsigned int n = verticesInKleinModel.size();
    
    if (norm(z)>1.0)
    {
        return false;
    }
    
    unsigned int i;
    for (i=0; i<n; i++)
    {
        xLeft = real(verticesInKleinModel[i] - z);
        yLeft = imag(verticesInKleinModel[i]- z);
        xRight = real(verticesInKleinModel[(i+1)%n] - z);
        yRight = imag(verticesInKleinModel[(i+1)%n] - z);
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
    
    complex u, v;
    
    u = (vertices.back().getDiskCoordinate() - vertices.front().getDiskCoordinate()) /
            (1.0 - conj(vertices.front().getDiskCoordinate())*vertices.back().getDiskCoordinate());
    v = (vertices[1].getDiskCoordinate() - vertices.front().getDiskCoordinate()) /
            (1.0 - conj(vertices.front().getDiskCoordinate())*vertices[1].getDiskCoordinate());
    res.push_back(Tools::mod2Pi(arg(v*conj(u))));
    
    for (unsigned int i=1; i+1<vertices.size(); i++)
    {
        u = (vertices[i-1].getDiskCoordinate() - vertices[i].getDiskCoordinate()) /
                (1.0 - conj(vertices[i].getDiskCoordinate())*vertices[i-1].getDiskCoordinate());
        v = (vertices[i+1].getDiskCoordinate() - vertices[i].getDiskCoordinate()) /
                (1.0 - conj(vertices[i].getDiskCoordinate())*vertices[i+1].getDiskCoordinate());
        res.push_back(Tools::mod2Pi(arg(v*conj(u))));
    }
    
    u = (vertices[vertices.size()-2].getDiskCoordinate() - vertices.back().getDiskCoordinate()) /
            (1.0 - conj(vertices.back().getDiskCoordinate())*vertices[vertices.size()-2].getDiskCoordinate());
    v = (vertices.front().getDiskCoordinate() - vertices.back().getDiskCoordinate()) /
            (1.0 - conj(vertices.back().getDiskCoordinate())*vertices.front().getDiskCoordinate());
    res.push_back(Tools::mod2Pi(arg(v*conj(u))));
    
    return res;
}

std::vector<double> H2Polygon::getPositiveInteriorAngles() const
{
    std::vector<double> angles = getAngles();
    if (isPositivelyOriented())
    {
        std::vector<double> output;
        for (unsigned int j=0; j<angles.size(); j++)
        {
            output.push_back(2.0*M_PI - angles[j]);
        }
        angles = output;
    }
    return angles;
}

std::vector<H2Triangle> H2Polygon::triangulate() const
{
    std::vector<H2Triangle> output;
    if (vertices.size() < 3)
    {
        std::cout << "Can't triangulate a polygon with less than 3 sides, dumbass!" << std::endl;
        return output;
    }
    else if (vertices.size() == 3)
    {
        H2Triangle T;
        T.setPoints(vertices[0],vertices[1],vertices[2]);
        output.push_back(T);
        return output;
    }
    else
    {
        std::vector<H2Triangle> output2;
        H2Polygon P1, P2;
        subdivideByBiggestAngles(P1,P2);
        output = P1.triangulate();
        output2 = P2.triangulate();
        output.insert(output.end(), output2.begin(), output2.end());
        return output;
    }
}

void H2Polygon::subdivideByBiggestAngles(H2Polygon &P1, H2Polygon &P2) const
{
    P1.clearVertices();
    P2.clearVertices();
    unsigned int biggestAngleIndex = 0, nextBiggestAngleIndex = 0;
    double biggestAngle = 0.0, nextBiggestAngle = 0.0;
    std::vector<double> angles = getPositiveInteriorAngles();
    unsigned int j;
    for (j=0; j< vertices.size(); j++)
    {
        if (biggestAngle < angles[j])
        {
            biggestAngle = angles[j];
            biggestAngleIndex = j;
        }
    }

    for (j=0; j<vertices.size(); j++)
    {
        if ((biggestAngleIndex == 0 && j>1 && j<(vertices.size()-1)) ||
                (biggestAngleIndex == vertices.size()-1 && j>0 && j<vertices.size()-2) ||
                (biggestAngleIndex>0 && biggestAngleIndex<vertices.size()-1 &&
                 j!=biggestAngleIndex-1 && j!=biggestAngleIndex+1 && j!=biggestAngleIndex))
        {
            if (nextBiggestAngle < angles[j])
            {
                nextBiggestAngle = angles[j];
                nextBiggestAngleIndex = j;
            }
        }
    }
    unsigned int smallInd = nextBiggestAngleIndex, bigInd = biggestAngleIndex;
    if (biggestAngleIndex < nextBiggestAngleIndex)
    {
        smallInd =  biggestAngleIndex;
        bigInd = nextBiggestAngleIndex;
    }
    for (j=smallInd; j<=bigInd; j++)
    {
        P1.addVertex(vertices[j]);
    }
    for (j=bigInd; j<vertices.size(); j++)
    {
        P2.addVertex(vertices[j]);
    }
    for (j=0; j<= smallInd; j++)
    {
        P2.addVertex(vertices[j]);
    }
    return;
}

std::ostream & operator<<(std::ostream & out, const H2Polygon &P)
{
    for(unsigned int i=0; i<(P.vertices).size();i++)
    {
        out << (P.vertices)[i] << std::endl;
    }
    return out;
}


