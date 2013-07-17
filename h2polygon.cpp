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
    return;
}

void H2Polygon::removeLastVertex()
{
    vertices.pop_back();
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

std::vector<double> H2Polygon::getEuclideanSideLengths() const
{
    std::vector<double> lengths;
    lengths.reserve(vertices.size());
    for(unsigned int i=0; i<vertices.size()-1; i++)
    {
        lengths.push_back( abs( vertices[i].getDiskCoordinate() - vertices[i+1].getDiskCoordinate() ) );
    }
    lengths.push_back( abs( vertices.back().getDiskCoordinate() - vertices[0].getDiskCoordinate() ) );
    return lengths;
}

double H2Polygon::normLargestEuclideanSideLength() const
{
    std::vector<double> lengths = getEuclideanSideLengths();
    double max;
    max = *std::max_element(lengths.begin(),lengths.end());
    return max;
}

double H2Polygon::normReciprocalSmallestEuclideanSideLength()
{
    std::vector<double> lengths = getEuclideanSideLengths();
    double smallest = *std::min_element(lengths.begin(),lengths.end());
    return 1/smallest;
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


std::ostream & operator<<(std::ostream & out, const H2Polygon &P)
{
    for(unsigned int i=0; i<(P.vertices).size();i++)
    {
        out << (P.vertices)[i] << std::endl;
    }
    return out;
}


