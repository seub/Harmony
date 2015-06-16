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
    void createVertexAndSteinerPoints();

    void createInteriorNeighbors();
    void createCutNeighbors();
    void createSideNeighbors();
    void createRemainingNeighbors();
    void createExteriorNeighbors();
    void createExteriorVertexNeighbors();
    void createMeshExteriorIndices();

    void reorganizeNeighbors();
    void createPiecewiseAffineWeights();
    void createNaiveWeights();
    void createQuadraticWeights();

    bool runTests() const;
    bool checkNumberOfMeshPoints() const;
    bool checkForDuplicateNeighbors() const;
    bool checkNumberOfNeighbors() const;
    bool checkPartnerPoints() const;
    bool checkCurrentIndex() const;

    std::vector<int> meshPointsIndicesAlongSide(int side) const;
    std::vector<int> meshPointsIndicesAlongFullSide(int side) const;


    H2Mesh *mesh;

    int depth;
    std::vector<H2TriangleSubdivision> *subdivisions;
    std::vector<std::vector<int>> *meshIndicesInSubdivisions;
    std::vector<H2MeshPoint*> *points;
    std::vector<H2MeshPoint> *regularPoints;
    std::vector<H2MeshCutPoint> *cutPoints;
    std::vector<H2MeshBoundaryPoint> *boundaryPoints;
    std::vector<H2MeshVertexPoint> *vertexPoints;
    std::vector<H2MeshSteinerPoint> *steinerPoints;


    H2PolygonTriangulater triangulater;
    int nbVertices, nbSteinerPoints, nbSubdivisions, nbSubdivisionLines, nbSubdivisionPoints;
    int nextIndex;
    std::vector< std::vector<bool> > boundaryPointInSubdivisions;
    std::vector<int> vertexMeshIndex, steinerPointsMeshIndex;
    std::vector< std::vector< std::vector<int> > > neighborsInSubdivisions;
    std::vector<Word> sidePairings;
};

#endif // H2MESHCONSTRUCTOR_H
