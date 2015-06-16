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
    friend class EquivariantHarmonicMapsFactory;
    friend void swap(H2Mesh &first, H2Mesh & second); // swap function

public:
    H2Mesh(const IsomH2Representation &rho, int depth); // Main constructor

    bool triangleContaining(const H2Point &point, H2Triangle &outputTriangle) const;
    bool triangleContaining(const H2Point &point, H2Triangle &outputTriangle, int &meshIndex1, int &meshIndex2, int &meshIndex3) const;

    H2Point getH2Point(int index) const;
    H2Triangle getH2Triangle(int index1, int index2, int index3) const;
    const std::vector<H2TriangleSubdivision> &getSubdivisions() const;
    std::vector<H2Point> getH2Neighbors(int index) const;
    std::vector<H2Point> getKickedH2Neighbors(int index) const;
    std::vector<H2Point> getPartnerPoints(int index) const;
    std::vector<H2Point> getPoints() const;
    std::vector<H2GeodesicArc> getSides() const;
    std::vector<H2Triangle> getTrianglesUp() const;
    IsomH2Representation getRepresentation() const;
    int nbPoints() const;
    bool isInteriorPoint(int index) const;

private:
    H2Mesh() {} // Dummy constructor
    H2Mesh(const H2Mesh &other); // Copy constructor
    H2Mesh & operator=(H2Mesh other); //Copy-assignment operator
    void createMeshPointsVector();

    IsomH2Representation rho;
    int depth;

    H2Polygon fundamentalDomain;
    H2SteinerPolygon fundamentalSteinerDomain;
    std::vector<int> exteriorSidesIndices;

    std::vector<H2TriangleSubdivision> subdivisions;
    std::vector<std::vector<int>> meshIndicesInSubdivisions;
    std::vector<TriangulationTriangle> triangles;

    std::vector<H2MeshPoint*> meshPoints;
    std::vector<H2MeshPoint> regularPoints;
    std::vector<H2MeshCutPoint> cutPoints;
    std::vector<H2MeshBoundaryPoint> boundaryPoints;
    std::vector<H2MeshVertexPoint> vertexPoints;
    std::vector<H2MeshSteinerPoint> steinerPoints;

};


#endif // H2MESH_H
