#ifndef H2MESH_H
#define H2MESH_H

#include "tools.h"
#include "h2polygon.h"
#include "grouprepresentation.h"
#include "h2meshpoint.h"
#include "h2polygontriangulater.h"
#include "triangularsubdivision.h"

class H2Isometry;
template <typename Point, typename Map> class LiftedGraphFunctionTriangulated;

class H2Mesh
{
    friend class H2MeshConstructor;
    friend class H2MeshFunction;
    friend class H2MeshFunctionIterator;
    friend void swap(H2Mesh &first, H2Mesh & second);
    friend class LiftedGraphFunctionTriangulated<H2Point, H2Isometry>;

public:
    H2Mesh(const GroupRepresentation<H2Isometry> &rho, uint depth);
    H2Mesh() {}
    H2Mesh(const H2Mesh &other);
    H2Mesh & operator=(H2Mesh other);

    bool triangleContaining(const H2Point &point, H2Triangle &outputTriangle, uint &meshIndex1, uint &meshIndex2, uint &meshIndex3) const;

    H2Point getH2Point(uint index) const;
    H2Triangle getH2Triangle(uint index1, uint index2, uint index3) const;
    std::vector<H2Point> getH2Neighbors(uint index) const;
    std::vector<H2Point> getKickedH2Neighbors(uint index) const;
    std::vector<H2Point> getPartnerPoints(uint index) const;
    std::vector<H2Point> getPoints() const;
    std::vector<H2GeodesicArc> getSides() const;
    std::vector<H2Triangle> getTrianglesUp() const;
    GroupRepresentation<H2Isometry> getRepresentation() const;
    uint nbPoints() const;
    uint getDepth() const;
    bool isInteriorPoint(uint index) const;

private:
    void createMeshPointsVector();

    GroupRepresentation<H2Isometry> rho;
    uint depth;

    H2Polygon fundamentalDomain;
    H2SteinerPolygon fundamentalSteinerDomain;
    std::vector<uint> exteriorSidesIndices;

    std::vector< TriangularSubdivision<H2Point> > subdivisions;
    std::vector< std::vector<uint> > meshIndicesInSubdivisions;
    std::vector<TriangulationTriangle> triangles;

    std::vector<H2MeshPoint*> meshPoints;
    std::vector<H2MeshPoint> regularPoints;
    std::vector<H2MeshCutPoint> cutPoints;
    std::vector<H2MeshBoundaryPoint> boundaryPoints;
    std::vector<H2MeshVertexPoint> vertexPoints;
    std::vector<H2MeshSteinerPoint> steinerPoints;

};


#endif // H2MESH_H
