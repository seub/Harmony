#ifndef H2POLYGONTRIANGULATER_H
#define H2POLYGONTRIANGULATER_H

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
    static bool shareSide(const TriangulationTriangle &T1, const TriangulationTriangle &T2,
                          uint &sharedIndex1, uint &sharedIndex2, uint &unsharedIndex1, uint &unsharedIndex2);

private:
    TriangulationTriangle() {}
    TriangulationTriangle(uint vertexIndex1, uint vertexIndex2, uint vertexIndex3) :
        vertexIndex1(vertexIndex1), vertexIndex2(vertexIndex2), vertexIndex3(vertexIndex3) {}


    uint vertexIndex1, vertexIndex2, vertexIndex3;
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

    void triangulate();
    std::vector<double> subpolygonAngles(const std::vector<uint> &indices) const;
    void findCutInSubpolygon1(const std::vector<uint> &indices, uint &outputIndex1, uint &outputIndex2) const;
    void findCutInSubpolygon2(const std::vector<uint> &indices, uint &outputIndex1, uint &outputIndex2) const;
    void findCutInSubpolygon3(const std::vector<uint> &indices, uint &outputIndex1, uint &outputIndex2) const;

    void triangulateSubpolygon(const std::vector<uint> &indices);
    void splitIndicesList(const std::vector<uint> & indices, uint cut1, uint cut2,
                          std::vector<uint> &outputList1, std::vector<uint> &outputList2) const;
    void sortTriangles();
    void completeCutsAndSides();

    void adjacentSidesIndices(uint cutIndex, uint &outputIndexLeft1, uint &outputIndexLeft2, uint &outputIndexRight1, uint &outputIndexRight2) const;

    double minAngleOfCutInSubpolygon(const std::vector<uint> & indices, uint cut1, uint cut2) const;

    double minTriangleAngle() const;
    double minTriangleSide() const;

    void createSteinerPoints();
    void createSteinerPointsDetailed();

    bool sameSide(uint fullIndex1, uint fullIndex2) const;
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
