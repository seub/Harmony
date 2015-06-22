#ifndef H2MESHCONSTRUCTOR_H
#define H2MESHCONSTRUCTOR_H

#include "tools.h"
#include "h2mesh.h"
#include "h2polygontriangulater.h"

class H2MeshConstructor
{
    friend H2Mesh::H2Mesh(const GroupRepresentation<H2Isometry> &, uint);

public:
    H2MeshConstructor() = delete;
    H2MeshConstructor(const H2MeshConstructor &) = delete;
    H2MeshConstructor & operator=(H2MeshConstructor) = delete;

private:
    explicit H2MeshConstructor(H2Mesh *mesh);

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
    void createRemainingNonExteriorNeighbors();
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

    std::vector<uint> meshPointsIndicesAlongSide(uint side) const;
    std::vector<uint> meshPointsIndicesAlongFullSide(uint side) const;

    static bool compareTriples(const std::tuple<H2Point, H2Point, uint> & t1, const std::tuple<H2Point, H2Point, uint> & t2);


    H2Mesh *mesh;

    uint depth;
    std::vector< TriangularSubdivision<H2Point> > *subdivisions;
    std::vector<std::vector<uint>> *meshIndicesInSubdivisions;
    std::vector<H2MeshPoint*> *points;
    std::vector<H2MeshPoint> *regularPoints;
    std::vector<H2MeshCutPoint> *cutPoints;
    std::vector<H2MeshBoundaryPoint> *boundaryPoints;
    std::vector<H2MeshVertexPoint> *vertexPoints;
    std::vector<H2MeshSteinerPoint> *steinerPoints;


    H2PolygonTriangulater triangulater;
    uint nbVertices, nbSteinerPoints, nbSubdivisions, nbSubdivisionLines, nbSubdivisionPoints;
    uint nextIndex;
    std::vector< std::vector<bool> > boundaryPointInSubdivisions;
    std::vector<uint> vertexMeshIndex, steinerPointsMeshIndex;
    std::vector< std::vector< std::vector<uint> > > neighborsInSubdivisions;
    std::vector<Word> sidePairings;
};

#endif // H2MESHCONSTRUCTOR_H
