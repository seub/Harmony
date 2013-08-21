#ifndef H2TRIANGLESUBDIVISION_H
#define H2TRIANGLESUBDIVISION_H

#include "tools.h"
#include "h2point.h"
#include "h2triangle.h"

class H2TriangleSubdivision
{
    friend class H2Mesh;
    friend class H2MeshConstructor;
    friend std::ostream & operator<<(std::ostream & out, const H2TriangleSubdivision &T);
    friend void swap(H2TriangleSubdivision &first, H2TriangleSubdivision &second);

public:
    H2TriangleSubdivision(); // Dummy constructor
    H2TriangleSubdivision(const H2TriangleSubdivision &other); // Copy constructor
    H2TriangleSubdivision(const H2Point &a, const H2Point &b, const H2Point &c, int depth);
    H2TriangleSubdivision(const H2Triangle &T, int depth);
    H2TriangleSubdivision & operator=(H2TriangleSubdivision other); // Copy-assignement operator
    ~H2TriangleSubdivision();

    int getTotalDepth() const;
    bool isEmpty() const;

    static int nbOfLines(int depth);
    static int nbOfPoints(int depth);
    static int nbOfInteriorPoints(int depth);
    static int nbOfBoundaryPoints(int depth);
    std::vector<int> sidePointsIndices(int vertexIndex1, int vertexIndex2) const;

    H2Triangle getTriangle() const;
    std::vector<H2Point> getPoints() const;

    bool triangleContaining(const H2Point &point, H2Triangle &outputTriangle) const;



    std::vector< std::vector<int> > neighborsIndices() const;
    std::vector<bool> areBoundaryPoints() const;

private:
    void construct(int aIndex, int bIndex, int cIndex, int depth, int totalDepth, std::vector<H2Point> *points, std::vector<int> *meshIndices,
                   std::vector<bool> &filled, int an, int bn, int cn, int ap, int bp, int cp); // Base (pseudo)constructor;
    void copy(const H2TriangleSubdivision &other, std::vector<H2Point> *points);



    H2TriangleSubdivision *A, *B, *C, *O;
    int aIndex, bIndex, cIndex;
    int depth, totalDepth;
    std::vector<H2Point> *points;
    std::vector<int> *meshIndices;
    bool empty;
};

#endif // H2TRIANGLESUBDIVISION_H
