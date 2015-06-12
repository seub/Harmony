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

    void createRegularPoints();
    void createCutPoints();
    void createBoundaryPoints();
    void createVertexPoints();

    void createInteriorNeighbors();
    void createCutNeighbors();
    void createSideNeighbors();
    void createRemainingNeighbors();
    void createExteriorNeighbors();
    void createExteriorVertexNeighbors();

    void setEpsilon();
    void reorganizeNeighbors();
    void createWeights();
    void createNaiveWeights();

    bool runTests() const;
    bool checkNumberOfMeshPoints() const;
    bool checkForDuplicateNeighbors() const;
    bool checkNumberOfNeighbors() const;
    bool checkPartnerPoints() const;

    std::vector<int> meshPointsIndicesAlongSide(int side) const;


    H2Mesh *mesh;

    int depth;
    std::vector<H2TriangleSubdivision> *subdivisions;
    std::vector<H2MeshPoint*> *points;
    std::vector<H2MeshPoint> *regularPoints;
    std::vector<H2MeshCutPoint> *cutPoints;
    std::vector<H2MeshBoundaryPoint> *boundaryPoints;
    std::vector<H2MeshVertexPoint> *vertexPoints;


    H2PolygonTriangulater triangulater;
    int nbVertices, nbSubdivisions, nbSubdivisionLines, nbSubdivisionPoints;
    int nextIndex;
    std::vector< std::vector<bool> > boundaryPointInSubdivisions;
    std::vector<int> vertexMeshIndex;
    std::vector< std::vector< std::vector<int> > > neighborsInSubdivisions;
    std::vector<Word> sidePairings;
    int currentIndex;
};

#endif // H2MESHCONSTRUCTOR_H
