#ifndef H2POLYGONTRIANGULATERCOPY_H
#define H2POLYGONTRIANGULATERCOPY_H


#include "tools.h"
#include "h2polygon.h"

class H2GeodesicArc;



class TriangulationCut
{
    friend class H2PolygonTriangulater;
    friend class H2MeshConstructor;

private:
    TriangulationCut() {}
    TriangulationCut(uint vertexIndex1, uint vertexIndex2, uint leftTriangleIndex, uint rightTriangleIndex) :
        vertexIndex1(vertexIndex1), vertexIndex2(vertexIndex2), leftTriangleIndex(leftTriangleIndex), rightTriangleIndex(rightTriangleIndex) {}

    uint vertexIndex1, vertexIndex2;
    uint leftTriangleIndex, rightTriangleIndex;
};

class TriangulationTriangle
{
    friend class H2PolygonTriangulater;
public:
    bool operator <(const TriangulationTriangle &other) const;
    void getVertices(uint &i1, uint &i2, uint &i3) const;

    void setCutNeighbors(uint cutNeighborIndex1, uint cutNeighborIndex2, uint cutNeighborIndex3);
    void getCutNeighbors(uint &c1, uint &c2, uint &c3) const;

private:
    TriangulationTriangle() {}
    TriangulationTriangle(uint vertexIndex1, uint vertexIndex2, uint vertexIndex3,
                              uint cutNeighborIndex1, uint cutNeighborIndex2, uint cutNeighborIndex3) :
        vertexIndex1(vertexIndex1), vertexIndex2(vertexIndex2), vertexIndex3(vertexIndex3),
        cutNeighborIndex1(cutNeighborIndex1), cutNeighborIndex2(cutNeighborIndex2), cutNeighborIndex3(cutNeighborIndex3) {}


    uint vertexIndex1, vertexIndex2, vertexIndex3;
    uint cutNeighborIndex1, cutNeighborIndex2, cutNeighborIndex3;

};


class H2Triangle;

class H2PolygonTriangulater
{
    friend class H2MeshConstructor;

public:
    H2PolygonTriangulater(const H2Polygon * const polygon);
    H2PolygonTriangulater() = delete;
    H2PolygonTriangulater & operator=(H2PolygonTriangulater) = delete;


    std::vector<H2Triangle> getTriangles() const;
    std::vector<TriangulationTriangle> getTriangulationTriangles() const;
    std::vector<TriangulationCut> getCuts() const;
    std::vector<H2GeodesicArc> getH2Cuts() const;
    std::vector<uint> nbCutsFromVertex() const;
    void verticesIndices(std::vector< std::vector<uint> > &triangleIndices, std::vector< std::vector<uint> > &indicesInTriangles) const;




private:
    H2PolygonTriangulater(const H2PolygonTriangulater &);

    void triangulateSimple();

    void sortTriangles();
    void completeCutsAndSides();

    void adjacentSidesIndices(uint cutIndex, uint &outputIndexLeft1, uint &outputIndexLeft2, uint &outputIndexRight1, uint &outputIndexRight2) const;

    double minAngleOfCutInSubpolygon(const std::vector<uint> & indices, uint cut1, uint cut2) const;

    void createSteinerPoints();
    void createSteinerPointsDetailed();

    bool sameSide(uint fullIndex1, uint fullIndex2) const;

    void getCutsFromTriangle(uint triangleIndex, uint &cutIndex1, uint &cutIndex2, uint &cutIndex3) const;
    bool isSideTriangle(uint triangleIndex) const;
    bool attemptFlip();

    const H2Polygon * const polygon;
    bool orientation;
    std::vector<TriangulationCut> cuts;
    std::vector<TriangulationTriangle> triangles;
    std::vector<uint> sideTrianglesIndices;
    std::vector<uint> sideTrianglesBoundarySideIndices;
    H2SteinerPolygon steinerPolygon;
    H2Polygon fullPolygon;

};



#endif // H2POLYGONTRIANGULATER_H
