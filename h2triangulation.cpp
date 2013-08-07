#include "h2triangulation.h"



H2Triangulation::H2Triangulation(H2Point *a, H2Point *b, H2Point *c, int depth, int maxDepth, int index, bool up) :
    a(a), b(b), c(c), depth(depth), maxDepth(maxDepth), index(index), up(up)
{
    //std::cout << "Entering H2Triangulation::H2Triangulation(H2Point *a, ...) with depth" << std::endl;

    if (maxDepth == -1)
    {
        this->maxDepth = depth;
        maxDepth = depth;
    }
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

        *midbc = H2Point::midpoint(*b, *c);
        *midac = H2Point::midpoint(*a, *c);
        *midab = H2Point::midpoint(*a, *b);

        A = new H2Triangulation(a, midab, midac, depth - 1, maxDepth, 4*index + 1, up);
        B = new H2Triangulation(midab, b, midbc, depth - 1, maxDepth, 4*index + 2, up);
        C = new H2Triangulation(midac, midbc, c, depth - 1, maxDepth, 4*index + 3, up);
        O = new H2Triangulation(midbc, midac, midab, depth - 1, maxDepth, 4*index + 4, !up);
    }

    //std::cout << "Leaving H2Triangulation::H2Triangulation(H2Point *a, ...)" << std::endl;
}

H2Triangulation::H2Triangulation(H2Triangle &T, int depth)
{
    *this = H2Triangulation(&T.a, &T.b, &T.c, depth);
    //std::cout << "Leaving H2Triangulation::H2Triangulation(H2Triangle &T, int depth)" << std::endl;
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
            if (3*index != Tools::exponentiation(4, maxDepth) -1)
            {
                delete a;
            }
            if (bottom())
            {
                if (index != Tools::exponentiation(4, maxDepth) -1)
                {
                    delete c;
                }
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

H2Triangle H2Triangulation::getTriangle() const
{
    return H2Triangle(*a,*b,*c);
}

int H2Triangulation::getDepth() const
{
    return depth;
}

bool H2Triangulation::getUp() const
{
    return up;
}

H2Triangulation* H2Triangulation::getA() const
{
    return A;
}

H2Triangulation* H2Triangulation::getB() const
{
    return B;
}

H2Triangulation* H2Triangulation::getC() const
{
    return C;
}

H2Triangulation* H2Triangulation::getO() const
{
    return O;
}
















