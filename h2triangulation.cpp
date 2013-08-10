#include "h2triangulation.h"

H2Triangulation::H2Triangulation()
{
    empty = true;
}

void H2Triangulation::construct(const H2Point *a, const H2Point *b, const H2Point *c, int depth, int maxDepth, int index, bool up)
{
    this->depth = depth;
    this->maxDepth = maxDepth;
    this->index = index;
    this->up = up;

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

        A = new H2Triangulation;
        B = new H2Triangulation;
        C = new H2Triangulation;
        O = new H2Triangulation;

        A->construct(this->a, midab, midac, depth - 1, maxDepth, 4*index + 1, up);
        B->construct(midab, this->b, midbc, depth - 1, maxDepth, 4*index + 2, up);
        C->construct(midac, midbc, this->c, depth - 1, maxDepth, 4*index + 3, up);
        O->construct(midbc, midac, midab, depth - 1, maxDepth, 4*index + 4, !up);
    }

    empty = false;
    return;
}

H2Triangulation::H2Triangulation(const H2Triangulation &other) :empty(other.empty)
{
    //std::cout << "Entering H2Triangulation::H2Triangulation(const H2Triangulation &other)" << std::endl;

    if (!empty)
    {
        if (depth != maxDepth)
        {
            std::cout << "WARNING in H2Triangulation copy constructor: being called for a non-root triangulation" << std::endl;
            H2Point * aTemp = new H2Point(*(other.a));
            H2Point * bTemp = new H2Point(*(other.b));
            H2Point * cTemp = new H2Point(*(other.c));
            construct(aTemp, bTemp, cTemp, other.depth, other.maxDepth, other.index, other.up);
        }
        else
        {
            construct(other.a, other.b, other.c, other.depth, other.maxDepth, other.index, other.up);
        }
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
    swap(first.empty, second.empty);

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

H2Triangulation::H2Triangulation(const H2Point &a, const H2Point &b, const H2Point &c, int depth)
{
    construct(&a, &b, &c, depth, depth, 0, true);
}

H2Triangulation::H2Triangulation(const H2Triangle &T, int depth)
{
    construct(&T.a, &T.b, &T.c, depth, depth, 0, true);
}

H2Triangulation::~H2Triangulation()
{
    //std::cout << "Entering H2Triangulation::~H2Triangulation with this->depth = " << depth << std::endl;
    if (!empty)
    {

        if (depth == 0)
        {
            if (up)
            {
                delete a;

                if (bottom())
                {
                    if (3*index == 2*(Tools::exponentiation(4, maxDepth) -1))
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
    }
    //std::cout << "Leaving H2Triangulation::~H2Triangulation" << std::endl;
}

bool H2Triangulation::isEmpty() const
{
    return empty;
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


std::ostream & operator<<(std::ostream & out, const H2Triangulation &T)
{
    out << "Triangulation of depth " << T.depth << " and maxDepth " << T.maxDepth << std::endl;
    out << "a = " << *T.a << ", b = " << *T.b << ", c = " << *T.c << std::endl;
    return out;
}

