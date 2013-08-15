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
                T += complex(0.0, 1.0)*direction;
            }
            else
            {
                T += -complex(0.0, 1.0)*direction;
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

bool H2Polygon::constainsInDiskModel(const complex &z) const
{
    if (norm(z)>1.0)
    {
        return false;
    }
    complex w = 2.0*z/(1.0 + norm(z));
    return containsInKleinModel(w);
}

bool H2Polygon::contains(const H2Point &point) const
{
    complex w = point.getKleinCoordinate();
    return containsInKleinModel(w);
}

bool H2Polygon::containsInKleinModel(const complex &z) const
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


    res.push_back(H2Point::angle(vertices.back(), vertices.front(), vertices[1]));
    for (unsigned int i=1; i+1<vertices.size(); i++)
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
        for (unsigned int j=0; j<angles.size(); j++)
        {
            angles[j] = 2.0*M_PI - angles[j];
        }
    }
    return angles;
}

std::ostream & operator<<(std::ostream & out, const H2Polygon &P)
{
    for(unsigned int i=0; i<(P.vertices).size();i++)
    {
        out << (P.vertices)[i] << std::endl;
    }
    return out;
}


