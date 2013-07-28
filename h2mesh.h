#ifndef H2MESH_H
#define H2MESH_H

#include "types.h"
#include "h2polygon.h"
#include "grouprepresentation.h"

class H2mesh
{
public:
    ~H2mesh();

    H2mesh(double step, const IsomH2Representation & rho);
    const complex * getMeshPoints() const;
    const bool * getIsInside() const;
    int getNbMeshPoints() const;
    H2Polygon getPolygon() const;
    IsomH2Representation getRho() const;

    int getClosestMeshIndex(const complex & z);

    std::vector<int> getSpecialPoints() const;

private:
    double step;

    int nbPointsX;
    int nbPointsY;
    int nbMeshPoints;


    double xMin,xMax,yMin,yMax;
    H2Polygon polygon;
    IsomH2Representation rho;
    std::vector<H2Isometry> listOfIsoms;

    complex firstMeshPoint;

    complex * meshPoints;
    bool * isInside;

    int * leftNeighborIndices;
    int * rightNeighborIndices;
    int * upNeighborIndices;
    int * downNeighborIndices;


    std::vector<int> specialPoints;

    void fillMeshPoints();
    void fillIsInside();
    void fillNeighbors();
};

#endif // H2MESH_H
