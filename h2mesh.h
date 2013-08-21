#ifndef H2MESH_H
#define H2MESH_H

#include "tools.h"
#include "h2polygon.h"
#include "grouprepresentation.h"

class H2TriangleSubdivision;
class H2Isometry;

class H2MeshPoint
{
    friend class H2Mesh;
    friend class H2MeshConstructor;

public:
    H2MeshPoint(int subdivisionIndex, int indexInSubdivision, bool cutPoint = false, int subdivisionIndexRight = -1,
                int indexInSubdivisionRight = -1, bool boundaryPoint = false, int side = -1, bool vertexPoint = false, int vertexIndex = -1);

private:
    int subdivisionIndex;
    int indexInSubdivision;
    std::vector<int> neighborsIndices;

    // For points on cuts
    bool cutPoint;
    int subdivisionIndexRight;
    int indexInSubdivisionRight;

    // For boundary points
    bool boundaryPoint;
    int side;
    std::vector<bool> isNeighborOutside;
    std::vector<int> neighborsSidePairings;

    // For vertex points
    bool vertexPoint;
    int vertexIndex;
};


class H2Mesh
{
    friend class H2MeshConstructor;

public:
    H2Mesh();
    H2Mesh(const IsomH2Representation &rho, int depth);

    bool triangleContaining(const H2Point &point, H2Triangle &outputTriangle) const;
    const std::vector<H2TriangleSubdivision> &getSubdivisions() const;

private:    
    IsomH2Representation rho;
    int depth;

    H2Polygon fundamentalDomain;
    std::vector<H2Isometry> sidePairings;

    std::vector<H2TriangleSubdivision> subdivisions;

    std::vector<H2MeshPoint> meshPoints;
};


#endif // H2MESH_H
