#ifndef H2MESH_H
#define H2MESH_H

#include "types.h"
#include "h2polygon.h"

class H2mesh
{
public:
    H2mesh();
    ~H2mesh();

    H2mesh(double step, const H2Polygon & polygon);
    const complex * getMeshPoints() const;
    const bool * getIsInside() const;
    int getNbMeshPoints() const;

    int getClosestMeshIndex(const complex & z) const;


private:
    double step;

    int nbPointsX;
    int nbPointsY;
    int nbMeshPoints;


    double xMin,xMax,yMin,yMax;
    H2Polygon polygon;

    complex firstMeshPoint;

    complex * meshPoints;
    bool * isInside;

    void fillIsInside();
};

#endif // H2MESH_H
