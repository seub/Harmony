#ifndef H2TRIANGULATION_H
#define H2TRIANGULATION_H

#include "tools.h"
#include "h2point.h"

class H2Triangulation
{
public:
    H2Triangulation(const H2Point *a, const H2Point *b, const H2Point *c, int depth, int maxDepth, int index = 0, bool up = true);
    ~H2Triangulation();

    bool bottom() const;

private:
    bool up;
    int depth, maxDepth;
    int index;

    H2Point * a, * b, * c;
    H2Triangulation * A, * B, * C, * O;
};

#endif // H2TRIANGULATION_H
