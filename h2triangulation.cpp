#include "h2triangulation.h"

H2Triangulation::H2Triangulation(const H2Point * a, const H2Point * b, const H2Point * c, int depth, int maxDepth, int index, bool up) :
    depth(depth), maxDepth(maxDepth), a(a), b(b), c(c), index(index), up(up)
{
    if (depth==0)
    {
        A = 0;
        B = 0;
        C = 0;
        O = 0;
    }
    else
    {
        H2Point * midbc = new H2Point();
        H2Point * midac = new H2Point();
        H2Point * midab = new H2Point();

        *midbc = H2Point::midpoint(b, c);
        *midac = H2Point::midpoint(a, c);
        *midab = H2Point::midpoint(a, b);

        A = new H2Triangulation(a, midab, midac, depth - 1, maxDepth, 4*index + 1, up);
        B = new H2Triangulation(midab, b, midbc, depth - 1, maxDepth, 4*index + 2, up);
        C = new H2Triangulation(midac, midbc, c, depth - 1, maxDepth, 4*index + 3, up);
        O = new H2Triangulation(midbc, midac, midab, depth - 1, maxDepth, 4*index + 4, !up);
    }
}

H2Triangulation::~H2Triangulation()
{
    delete A;
    delete B;
    delete C;
    delete O;
    if (depth == 0)
    {
        if (up)
        {
            delete a;
            if (bottom())
            {
                delete c;
            }
        }
    }
}

bool H2Triangulation::bottom() const
{
    int ind = index, newind, digit;
    while (ind > 0)
    {
        newind = ind/4;
        digit = ind - 4*newind;
        if (digit < 2)
        {
            return false;
        }
        ind = newind;
    }
    return true;
}
