#ifndef H2MESHCONSTRUCTOR_H
#define H2MESHCONSTRUCTOR_H

#include "tools.h"
#include "h2mesh.h"
#include "h2polygontriangulater.h"

class H2MeshConstructor
{
public:
    H2MeshConstructor(H2Mesh *mesh);

private:
    void createPoints();
    void createNeighbors();
    void createSubdivisions();
    void createInteriorMeshPoints();
    void createCutMeshPoints();
    void createSideMeshPoints();
    void createInteriorNeighbors();
    void createCutNeighbors();
    void createSideNeighbors();
    void createExteriorNeighbors();


    H2Mesh *mesh;

    int depth;
    std::vector<H2Isometry> *sidePairings;
    std::vector<H2TriangleSubdivision> *subdivisions;
    std::vector<H2MeshPoint> *meshPoints;

    H2PolygonTriangulater triangulater;
    int nbVertices, nbSubdivisions, nbSubdivisionLines, nbSubdivisionPoints;
    int nextIndex;
    std::vector< std::vector<bool> > boundaryPointInSubdivisions;
    std::vector<bool> vertexAdded;
    std::vector<int> vertexMeshIndex;
    std::vector< std::vector< std::vector<int> > > neighborsInSubdivisions;
};

#endif // H2MESHCONSTRUCTOR_H
