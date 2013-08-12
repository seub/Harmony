#ifndef H2MESH_H
#define H2MESH_H

#include "tools.h"
#include "h2polygon.h"
#include "grouprepresentation.h"
#include "h2isometry.h"
#include "h2trianglesubdivision.h"

class H2MeshPoint
{
public:

private:
    int SubdivisionIndex;
    int IndexInSubdivision;
    std::vector<int> neighborsIndices;

    // For exterior points
    bool exterior;
    int SidePairingIndex;
};


class H2Mesh
{
public:
    H2Mesh(const IsomH2Representation & rho, int depth);



private:    
    IsomH2Representation rho;
    int depth;

    H2Polygon fundamentalDomain;
    std::vector<H2Isometry> sidePairings;

    std::vector<H2TriangleSubdivision> subdivisions;
    std::vector<H2MeshPoint> meshPoints;
};


#endif // H2MESH_H
