#ifndef H2TRIANGLESUBDIVISION_H
#define H2TRIANGLESUBDIVISION_H

#include "tools.h"
#include "h2point.h"
#include "h2triangle.h"

class H2TriangleSubdivision
{
    friend std::ostream & operator<<(std::ostream & out, const H2Triangulation &T);
    friend void swap(H2TriangleSubdivision &first, H2TriangleSubdivision &second);

public:
    H2TriangleSubdivision(); // Dummy constructor
    H2TriangleSubdivision(const H2TriangleSubdivision &other); // Copy constructor
    H2TriangleSubdivision(const H2Point &a, const H2Point &b, const H2Point &c, int depth);
    H2TriangleSubdivision(const H2Triangle &T, int depth);
    H2TriangleSubdivision & operator=(H2TriangleSubdivision other); // Copy-assignement operator
    ~H2TriangleSubdivision();

    H2Triangle getTriangle() const;

    bool getTriangleContaining(const H2Point &point, int &outputIndex) const;
    bool getTriangleContaining(const H2Point &point, H2Triangle &outputTriangle) const;

private:
    H2TriangleSubdivision *A, *B, *C, *O;
    std::vector<H2Point> *points;
    int aIndex, bIndex, cIndex;
    int depth, totalDepth;
};

#endif // H2TRIANGLESUBDIVISION_H
