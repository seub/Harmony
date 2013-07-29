#ifndef H2TRIANGULATION_H
#define H2TRIANGULATION_H

#include "tools.h"
#include "h2point.h"
#include "h2triangle.h"

class H2Triangulation
{
public:
    H2Triangulation(H2Point *a, H2Point *b, H2Point *c, int depth, int maxDepth = -1, int index = 0, bool up = true);
    H2Triangulation(const H2Triangle & T, int depth, int maxDepth = -1);
    ~H2Triangulation();

    bool bottom() const;
    H2Triangle getTriangle() const;
    int getDepth() const;
    bool getUp() const;
    H2Triangulation* getA() const;
    H2Triangulation* getB() const;
    H2Triangulation* getC() const;
    H2Triangulation* getO() const;

private:

    H2Point * a, * b, * c;

    int depth, maxDepth;
    int index;
    bool up;

    H2Triangulation * A, * B, * C, * O;
};

#endif // H2TRIANGULATION_H
