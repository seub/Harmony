#include "h2triangulation.h"



H2Triangulation::H2Triangulation(const H2Point *a, const H2Point *b, const H2Point *c, int depth, int maxDepth, int index, bool up) :
    depth(depth), maxDepth(maxDepth), index(index), up(up)
{
    //std::cout << "Entering H2Triangulation::H2Triangulation(H2Point *a, ...)" << std::endl;

    if (maxDepth == depth)
    {
        this->a = new H2Point(*a);
        this->b = new H2Point(*b);
        this->c = new H2Point(*c);
    }
    else
    {
        this->a = a;
        this->b = b;
        this->c = c;
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
        H2Point *midbc = new H2Point(H2Point::midpoint(*b, *c));
        H2Point *midac = new H2Point(H2Point::midpoint(*a, *c));
        H2Point *midab = new H2Point(H2Point::midpoint(*a, *b));

        A = new H2Triangulation(this->a, midab, midac, depth - 1, maxDepth, 4*index + 1, up);
        B = new H2Triangulation(midab, this->b, midbc, depth - 1, maxDepth, 4*index + 2, up);
        C = new H2Triangulation(midac, midbc, this->c, depth - 1, maxDepth, 4*index + 3, up);
        O = new H2Triangulation(midbc, midac, midab, depth - 1, maxDepth, 4*index + 4, !up);
    }

    //std::cout << "Leaving H2Triangulation::H2Triangulation(H2Point *a, ...)" << std::endl;
}

H2Triangulation::H2Triangulation(const H2Triangulation &other) :
    depth(other.depth), maxDepth(other.maxDepth), index(other.index), up(other.up)
{
    //std::cout << "Entering H2Triangulation::H2Triangulation(const H2Triangulation &other)" << std::endl;
    if (depth==0)
    {
        A = 0;
        B = 0;
        C = 0;
        O = 0;

        if (up)
        {
            a = new H2Point(*other.a);
            if (bottom())
            {
                if (3*index == 2*Tools::exponentiation(4, maxDepth) -1)
                {
                    b = new H2Point(*other.b);
                }
                c = new H2Point(*other.c);
            }
        }
    }
    else
    {
        A = new H2Triangulation(*other.A);
        B = new H2Triangulation(*other.B);
        C = new H2Triangulation(*other.C);
        O = new H2Triangulation(*other.O);

        a = A->a;
        b = B->b;
        c = C->c;
    }
    //std::cout << "Leaving H2Triangulation::H2Triangulation(const H2Triangulation &other)" << std::endl;
}

void swap(H2Triangulation &first, H2Triangulation &second)
{
    //std::cout << "Entering swap(H2Triangulation &first, H2Triangulation &second)" << std::endl;
    using std::swap;

    swap(first.a, second.a);
    swap(first.b, second.b);
    swap(first.c, second.c);

    swap(first.A, second.A);
    swap(first.B, second.B);
    swap(first.C, second.C);
    swap(first.O, second.O);

    swap(first.index, second.index);
    swap(first.depth, second.depth);
    swap(first.maxDepth, second.maxDepth);
    swap(first.up, second.up);

    //std::cout << "Leaving swap(H2Triangulation &first, H2Triangulation &second)" << std::endl;
    return;
}

H2Triangulation& H2Triangulation::operator=(H2Triangulation other)
{
    //std::cout << "Entering H2Triangulation::operator=" << std::endl;
    swap(*this, other);

    //std::cout << "Leaving H2Triangulation::operator=" << std::endl;
    return *this;
}

H2Triangulation::H2Triangulation(const H2Point &a, const H2Point &b, const H2Point &c, int depth) :
    H2Triangulation(&a, &b, &c, depth, depth, 0, true)
{
}

H2Triangulation::H2Triangulation(const H2Triangle &T, int depth) :
    H2Triangulation(T.a, T.b, T.c, depth)
{
}

H2Triangulation::~H2Triangulation()
{
    //std::cout << "Entering H2Triangulation::~H2Triangulation with this->depth = " << depth << std::endl;
    if (depth == 0)
    {
        if (up)
        {
            delete a;

            if (bottom())
            {
                if (3*index == 2*Tools::exponentiation(4, maxDepth) -1)
                {
                    delete b;
                }
                delete c;
            }
        }
    }
    else
    {
        delete A;
        delete B;
        delete C;
        delete O;
    }
    //std::cout << "Leaving H2Triangulation::~H2Triangulation" << std::endl;
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


bool H2Triangulation::getTriangleContaining(const H2Point &point, int &outputIndex) const
{
    H2Triangle T = getTriangle();
    if (T.contains(point))
    {
        if (depth == 0)
        {
            outputIndex = index;
        }
        else if ( (!A->getTriangleContaining(point, outputIndex)) && (!B->getTriangleContaining(point, outputIndex))
                  && (!C->getTriangleContaining(point, outputIndex)) && (!O->getTriangleContaining(point, outputIndex)) )
        {
            std::cout << "ERROR in H2Triangulation::getTriangleContaining: this is not supposed to happen" << std::endl;
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool H2Triangulation::getTriangleContaining(const H2Point &point, H2Triangle &outputTriangle) const
{
    H2Triangle T = getTriangle();
    if (T.contains(point))
    {
        if (depth == 0)
        {
            outputTriangle = T;
        }
        else if ( (!A->getTriangleContaining(point, outputTriangle)) && (!B->getTriangleContaining(point, outputTriangle))
                  && (!C->getTriangleContaining(point, outputTriangle)) && (!O->getTriangleContaining(point, outputTriangle)) )
        {
            std::cout << "ERROR in H2Triangulation::getTriangleContaining: this is not supposed to happen" << std::endl;
        }
        return true;
    }
    else
    {
        return false;
    }
}




