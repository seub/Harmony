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
    TriangulationCut(int vertexIndex1, int vertexIndex2, int leftTriangleIndex, int rightTriangleIndex) :
        vertexIndex1(vertexIndex1), vertexIndex2(vertexIndex2), leftTriangleIndex(leftTriangleIndex), rightTriangleIndex(rightTriangleIndex) {}

    int vertexIndex1, vertexIndex2;
    int leftTriangleIndex, rightTriangleIndex;
};

class TriangulationTriangle
{
    friend class H2PolygonTriangulater;
public:
    bool operator <(const TriangulationTriangle &other) const;
    void getVertices(int &i1, int &i2, int &i3) const;

private:
    TriangulationTriangle() {}
    TriangulationTriangle(int vertexIndex1, int vertexIndex2, int vertexIndex3) :
        vertexIndex1(vertexIndex1), vertexIndex2(vertexIndex2), vertexIndex3(vertexIndex3) {}



    int vertexIndex1, vertexIndex2, vertexIndex3;
};


class H2Triangle;

class H2PolygonTriangulater
{
    friend class H2MeshConstructor;

public:
    H2PolygonTriangulater(const H2Polygon * const polygon);
    std::vector<H2Triangle> getTriangles() const;
    std::vector<TriangulationTriangle> getTriangulationTriangles() const;
    std::vector<TriangulationCut> getCuts() const;
    std::vector<H2GeodesicArc> getH2Cuts() const;
    std::vector<int> nbCutsFromVertex() const;
    void verticesIndices(std::vector< std::vector<int> > &triangleIndices, std::vector< std::vector<int> > &indicesInTriangles) const;

private:
    H2PolygonTriangulater(const H2PolygonTriangulater &);
    H2Polygon & operator=(H2PolygonTriangulater);


    void triangulate();
    std::vector<double> subpolygonAngles(const std::vector<int> &indices) const;
    void findCutInSubpolygon1(const std::vector<int> &indices, int &outputIndex1, int &outputIndex2) const;
    void findCutInSubpolygon2(const std::vector<int> &indices, int &outputIndex1, int &outputIndex2) const;
    void findCutInSubpolygon3(const std::vector<int> &indices, int &outputIndex1, int &outputIndex2) const;

    void triangulateSubpolygon(const std::vector<int> &indices);
    void splitIndicesList(const std::vector<int> & indices, int cut1, int cut2,
                          std::vector<int> &outputList1, std::vector<int> &outputList2) const;
    void sortTriangles();
    void completeCutsAndSides();

    void adjacentSidesIndices(int cutIndex, int &outputIndexLeft1, int &outputIndexLeft2, int &outputIndexRight1, int &outputIndexRight2) const;

    double minAngleOfCutInSubpolygon(const std::vector<int> & indices, unsigned int cut1, unsigned int cut2) const;

    double minTriangleAngle() const;
    double minTriangleSide() const;

    void createSteinerPoints();

    bool sameSide(int fullIndex1, int fullIndex2) const;

    const H2Polygon * const polygon;
    bool orientation;
    std::vector<TriangulationCut> cuts;
    std::vector<TriangulationTriangle> triangles;
    std::vector<int> sideTrianglesIndices;
    std::vector<int> sideTrianglesBoundarySideIndices;
    H2SteinerPolygon steinerPolygon;
    H2Polygon fullPolygon;

};

#endif // H2POLYGONTRIANGULATER_H
