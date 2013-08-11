#include "h2trianglesubdivision.h"

H2TriangleSubdivision::H2TriangleSubdivision()
{
    empty = true;
}

H2TriangleSubdivision::H2TriangleSubdivision(const H2Triangle &T, int depth)
{
    *this = H2TriangleSubdivision(T.a, T.b, T.c, depth);
}

H2TriangleSubdivision::H2TriangleSubdivision(const H2TriangleSubdivision &other)
{
    empty = other.empty;
    if (!empty)
    {
        points = new std::vector<H2Point>(*other.points);
        copy(other, points);
    }
}

void H2TriangleSubdivision::copy(const H2TriangleSubdivision &other, std::vector<H2Point> *points)
{
    aIndex = other.aIndex;
    bIndex = other.bIndex;
    cIndex = other.cIndex;
    depth = other.depth;
    totalDepth = other.totalDepth;
    this->points = points;
    empty = false;

    if (depth == 0)
    {
        A = 0;
        B = 0;
        C = 0;
        O = 0;
    }
    else
    {
        A = new H2TriangleSubdivision;
        B = new H2TriangleSubdivision;
        C = new H2TriangleSubdivision;
        O = new H2TriangleSubdivision;

        A->copy(*other.A, points);
        B->copy(*other.B, points);
        C->copy(*other.C, points);
        O->copy(*other.O, points);
    }

    return;
}

void swap(H2TriangleSubdivision &first, H2TriangleSubdivision &second)
{
    std::swap(first.A, second.A);
    std::swap(first.B, second.B);
    std::swap(first.C, second.C);
    std::swap(first.O, second.O);

    std::swap(first.aIndex, second.aIndex);
    std::swap(first.bIndex, second.bIndex);
    std::swap(first.cIndex, second.cIndex);

    std::swap(first.depth, second.depth);
    std::swap(first.totalDepth, second.totalDepth);
    std::swap(first.points, second.points);
    std::swap(first.empty, second.empty);
}

H2TriangleSubdivision & H2TriangleSubdivision::operator=(H2TriangleSubdivision other)
{
    swap(*this, other);
    return *this;
}

H2TriangleSubdivision::H2TriangleSubdivision(const H2Point &a, const H2Point &b, const H2Point &c, int depth)
{
    int L = nbOfLines(), N = nbOfPoints();
    points = new std::vector<H2Point>;
    points->resize(N);
    std::vector<bool> filled;
    std::fill(filled.begin(), filled.end(), false);

    int an = 0, ap = 0, bn = L - 1, bp = 0, cn = L - 1, cp = L - 1;
    aIndex = 0;
    cIndex = N - 1;
    bIndex = cIndex - cp;

    (*points)[aIndex] = a;
    (*points)[bIndex] = b;
    (*points)[cIndex] = c;
    filled[aIndex] = true;
    filled[bIndex] = true;
    filled[cIndex] = true;

    construct(aIndex, bIndex, cIndex, depth, depth, points, filled, an, bn, cn, ap, bp, cp);

    // Filled count check
    int counter = 0;
    for (unsigned int i=0; i<filled.size(); i++)
    {
        if (filled[i])
        {
            counter++;
        }
    }
    std::cout << "nb of points filled: " << counter << std::endl;
    std::cout << "nb of points in triangulation: " << N << std::endl;
}

void H2TriangleSubdivision::construct(int aIndex, int bIndex, int cIndex, int depth, int totalDepth, std::vector<H2Point> *points,
                                      std::vector<bool> &filled, int an, int bn, int cn, int ap, int bp, int cp)
{
    this->aIndex = aIndex;
    this->bIndex = bIndex;
    this->cIndex = cIndex;
    this->depth = depth;
    this->totalDepth = totalDepth;
    this->points = points;

    if (depth == 0)
    {
        A = 0;
        B = 0;
        C = 0;
        O = 0;
    }
    else
    {
        int midabn = (an + bn)/2, midacn = (an + cn)/2, midbcn = (bn + cn)/2;
        int midabp = (ap + bp)/2, midacp = (ap + cp)/2, midbcp = (bp + cp)/2;
        int midabIndex = (midabn*(midabn+1))/2 + midabp;
        int midacIndex = (midacn*(midacn+1))/2 + midacp;
        int midbcIndex = (midbcn*(midbcn+1))/2 + midbcp;

        if (!filled[midabIndex])
        {
            (*points)[midabIndex] = H2Point::midpoint((*points)[aIndex], (*points)[bIndex]);
            filled[midabIndex] = true;
        }
        if (!filled[midacIndex])
        {
            (*points)[midacIndex] = H2Point::midpoint((*points)[aIndex], (*points)[cIndex]);
            filled[midacIndex] = true;
        }
        if (!filled[midbcIndex])
        {
            (*points)[midbcIndex] = H2Point::midpoint((*points)[bIndex], (*points)[cIndex]);
            filled[midbcIndex] = true;
        }

        A = new H2TriangleSubdivision;
        B = new H2TriangleSubdivision;
        C = new H2TriangleSubdivision;
        O = new H2TriangleSubdivision;

        A->construct(aIndex, midabIndex, midacIndex, depth - 1, totalDepth, points, filled, an, midabn, midacn, ap, midabp, midacp);
        B->construct(midabIndex, bIndex, midbcIndex, depth - 1, totalDepth, points, filled, midabn, bn, midbcn, midabp, bp, midbcp);
        C->construct(midacIndex, midbcIndex, cIndex, depth - 1, totalDepth, points, filled, midacn, midbcn, cn, midacp, midbcp, cp);
        O->construct(midbcIndex, midacIndex, midabIndex, depth - 1, totalDepth, points, filled, midbcn, midacn, midabn, midbcp, midacp, midabp);
    }

    empty = false;
    return;
}


H2TriangleSubdivision::~H2TriangleSubdivision()
{
    if (!empty)
    {
        if (depth == totalDepth)
        {
            delete points;
        }

        if (depth != 0)
        {
            delete A;
            delete B;
            delete C;
            delete O;
        }
    }
}

int H2TriangleSubdivision::nbOfPoints() const
{
    int L = nbOfLines();
    return L*(L+1)/2;
}

int H2TriangleSubdivision::nbOfLines() const
{
    return 1 + Tools::exponentiation(2, totalDepth);
}

H2Triangle H2TriangleSubdivision::getTriangle() const
{
    return H2Triangle((*points)[aIndex], (*points)[bIndex], (*points)[cIndex]);
}

std::ostream & operator<<(std::ostream & out, const H2TriangleSubdivision &T)
{
    out << "Triangle subdivision of " << T.getTriangle() << " with depth " << T.depth << std::endl;
    return out;
}
