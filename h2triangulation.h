#ifndef H2TRIANGULATION_H
#define H2TRIANGULATION_H

#include "tools.h"
#include "h2point.h"
#include "h2triangle.h"

class H2Triangulation
{
    friend void swap(H2Triangulation &first, H2Triangulation &second);

public:
    H2Triangulation(const H2Triangulation &other); // Copy constructor


    H2Triangulation(const H2Point &a, const H2Point &b, const H2Point &c, int depth); // Public derived constructor 1
    H2Triangulation(const H2Triangle &T, int depth); // Public derived constructor 2

    ~H2Triangulation();

    H2Triangulation & operator=(H2Triangulation other); // Copy-assignement operator


    bool bottom() const;
    H2Triangle getTriangle() const;
    int getDepth() const;
    bool getUp() const;
    H2Triangulation* getA() const;
    H2Triangulation* getB() const;
    H2Triangulation* getC() const;
    H2Triangulation* getO() const;

    bool getTriangleContaining(const H2Point & point, int & outputIndex) const;
    bool getTriangleContaining(const H2Point &point, H2Triangle & outputTriangle) const;

private:    
    H2Triangulation(H2Point *a, H2Point *b, H2Point *c, int depth, int maxDepth, int index, bool up); // Base constructor

    H2Point * a, * b, * c;

    int depth, maxDepth;
    int index;
    bool up;

    H2Triangulation * A, * B, * C, * O;
};

#endif // H2TRIANGULATION_H
