#ifndef H2TRIANGULATION_H
#define H2TRIANGULATION_H

#include "tools.h"
#include "h2point.h"
#include "h2triangle.h"

class H2Triangulation
{
    friend std::ostream & operator<<(std::ostream & out, const H2Triangulation &T);
    friend void swap(H2Triangulation &first, H2Triangulation &second);

public:
    H2Triangulation(const H2Triangulation &other); // Copy constructor

    H2Triangulation(); // Dummy constructor;
    H2Triangulation(const H2Point &a, const H2Point &b, const H2Point &c, int depth);
    H2Triangulation(const H2Triangle &T, int depth);

    ~H2Triangulation();

    H2Triangulation & operator=(H2Triangulation other); // Copy-assignement operator


    bool bottom() const;
    bool isEmpty() const;
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
    void construct(const H2Point *a, const H2Point *b, const H2Point *c, int depth, int maxDepth, int index, bool up);
    // Pseudo constructor

    const H2Point * a, * b, * c;

    int depth, maxDepth;
    int index;
    bool up, empty;

    H2Triangulation * A, * B, * C, * O;
};

#endif // H2TRIANGULATION_H
