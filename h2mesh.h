#ifndef H2MESH_H
#define H2MESH_H

#include "tools.h"
#include "h2polygon.h"
#include "grouprepresentation.h"
#include "h2meshpoint.h"
#include "h2polygontriangulater.h"
#include "h2trianglesubdivision.h"

class H2Isometry;
//class TriangulationTriangle;

class H2Mesh
{
    friend class H2MeshConstructor;
    friend class H2MeshFunction;
    friend class H2MeshFunctionIterator;

public:
    H2Mesh();
    H2Mesh(const IsomH2Representation &rho, int depth);

    bool triangleContaining(const H2Point &point, H2Triangle &outputTriangle) const;
    bool triangleContaining(const H2Point &point, H2Triangle &outputTriangle, int &meshIndex1, int &meshIndex2, int &meshIndex3) const;

    const H2Point & getH2Point(int index) const;
    const std::vector<H2TriangleSubdivision> &getSubdivisions() const;
    std::vector<H2Point> getH2Neighbors(int index) const;
    std::vector<H2Point> getKickedH2Neighbors(int index) const;
    IsomH2Representation getRepresentation() const;
    int nbPoints() const;

private:
    IsomH2Representation rho;
    int depth;

    H2Polygon fundamentalDomain;

    std::vector<H2TriangleSubdivision> subdivisions;
    std::vector<TriangulationTriangle> triangles;

    std::vector<H2MeshPoint*> meshPoints;
    std::vector<H2MeshPoint> regularPoints;
    std::vector<H2MeshCutPoint> cutPoints;
    std::vector<H2MeshBoundaryPoint> boundaryPoints;
    std::vector<H2MeshVertexPoint> vertexPoints;

};


#endif // H2MESH_H
