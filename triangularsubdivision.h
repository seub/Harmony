#ifndef TRIANGULARSUBDIVISION_H
#define TRIANGULARSUBDIVISION_H

#include "tools.h"

class H2Point; class H2Triangle;

template <typename Point> class TriangularSubdivision
{
    friend class H2Mesh;
    friend class H2MeshConstructor;
    template <typename S> friend void swap(TriangularSubdivision<S> &first, TriangularSubdivision<S> &second);

public:
    TriangularSubdivision();
    TriangularSubdivision(const TriangularSubdivision<Point> &other);
    TriangularSubdivision(uint depth);
    TriangularSubdivision(const std::vector<Point> &points, uint depth);
    TriangularSubdivision<Point> & operator=(TriangularSubdivision<Point> other);

    void initializeSubdivisionByMidpoints(const Point &a, const Point &b, const Point &c);

    uint getTotalDepth() const;
    bool isRoot() const;

    static uint nbLines(uint depth);
    static uint nbPoints(uint depth);
    static uint nbInteriorPoints(uint depth);
    static uint nbBoundaryPoints(uint depth);
    std::vector<uint> sidePointsIndices(uint vertexIndex1, uint vertexIndex2) const;

    void getBigTriangle(Point &outA, Point &outB, Point &outC) const;
    std::vector<Point> getPoints() const;
    Point getPoint(uint index) const;

    bool triangleContaining(const H2Point &point, Point &outA, Point &outB, Point &outC, uint &index1, uint&index2, uint &index3) const;


    std::vector<std::vector<uint> > neighborsIndices() const;
    std::vector<bool> areBoundaryPoints() const;


private:
    void constructIndices(uint aIndex, uint bIndex, uint cIndex, uint depth, uint totalDepth,
                          uint an, uint bn, uint cn, uint ap, uint bp, uint cp, std::shared_ptr< std::vector<Point> > points);
    void constructMidpoints( uint an, uint bn, uint cn, uint ap, uint bp, uint cp, std::vector<bool> &filled);
    void constructWithGivenPoints(uint aIndex, uint bIndex, uint cIndex, uint depth, uint totalDepth, std::vector<H2Point> *points,
                   std::vector<bool> &filled, uint an, uint bn, uint cn, uint ap, uint bp, uint cp);
    void copy(const TriangularSubdivision<Point> &other,  std::shared_ptr< std::vector<Point> > points);



    std::unique_ptr< TriangularSubdivision<Point> > A, B, C, O;
    uint aIndex, bIndex, cIndex;
    uint depth, totalDepth;

    std::shared_ptr< std::vector<Point> > points;
};
#endif // TRIANGULARSUBDIVISION_H
