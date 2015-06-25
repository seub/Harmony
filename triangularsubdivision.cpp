#include "triangularsubdivision.h"
#include "h2point.h"
#include "h2triangle.h"

template <typename Point> TriangularSubdivision<Point>::TriangularSubdivision()
{
    A.reset();
    B.reset();
    C.reset();
    O.reset();
    points.reset();
}

template <typename Point> TriangularSubdivision<Point>::TriangularSubdivision(const TriangularSubdivision<Point> &other)
{
    if (other.points != nullptr)
    {
        points = std::make_shared< std::vector<Point> >(*other.points);
        copy(other, points);
    }
    else
    {
        A.reset();
        B.reset();
        C.reset();
        O.reset();
        points.reset();
    }
}

template <typename Point> void TriangularSubdivision<Point>::copy(const TriangularSubdivision<Point> &other, std::shared_ptr< std::vector<Point> > points)
{
    aIndex = other.aIndex;
    bIndex = other.bIndex;
    cIndex = other.cIndex;
    depth = other.depth;
    totalDepth = other.totalDepth;
    this->points = points;

    if (depth == 0)
    {
        A.reset();
        B.reset();
        C.reset();
        O.reset();
    }
    else
    {
        A = std::unique_ptr<TriangularSubdivision<Point> >(new TriangularSubdivision<Point>);
        B = std::unique_ptr<TriangularSubdivision<Point> >(new TriangularSubdivision<Point>);
        C = std::unique_ptr<TriangularSubdivision<Point> >(new TriangularSubdivision<Point>);
        O = std::unique_ptr<TriangularSubdivision<Point> >(new TriangularSubdivision<Point>);

        /* C++14
        A = std::make_unique< TriangularSubdivision<Point> >();
        B = std::make_unique< TriangularSubdivision<Point> >();
        C = std::make_unique< TriangularSubdivision<Point> >();
        O = std::make_unique< TriangularSubdivision<Point>> ();
        */

        A->copy(*other.A, points);
        B->copy(*other.B, points);
        C->copy(*other.C, points);
        O->copy(*other.O, points);
    }
}

template <typename S> void swap(TriangularSubdivision<S> &first, TriangularSubdivision<S> &second)
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
}

template <typename Point> TriangularSubdivision<Point> & TriangularSubdivision<Point>::operator=(TriangularSubdivision<Point> other)
{
    swap(*this, other);
    return *this;
}

template <typename Point> TriangularSubdivision<Point>::TriangularSubdivision(uint depth)
{
    points = std::make_shared< std::vector<Point> >();
    this->depth = depth;
    totalDepth = depth;
    uint L = nbLines(depth), N = nbPoints(depth);

    uint an = 0, ap = 0, bn = L - 1, bp = 0, cn = L - 1, cp = L - 1;
    aIndex = 0;
    cIndex = N - 1;
    bIndex = cIndex - cp;

    constructIndices(aIndex, bIndex, cIndex, depth, depth, an, bn, cn, ap, bp, cp, points);
}

template <typename Point> void TriangularSubdivision<Point>::constructIndices(uint aIndex, uint bIndex, uint cIndex, uint depth, uint totalDepth,
                                                                              uint an, uint bn, uint cn, uint ap, uint bp, uint cp,
                                                                              std::shared_ptr< std::vector<Point> > points)
{
    this->aIndex = aIndex;
    this->bIndex = bIndex;
    this->cIndex = cIndex;
    this->depth = depth;
    this->totalDepth = totalDepth;
    this->points = points;

    if (depth == 0)
    {
        A.reset();
        B.reset();
        C.reset();
        O.reset();
    }
    else
    {
        uint midabn = (an + bn)/2, midacn = (an + cn)/2, midbcn = (bn + cn)/2;
        uint midabp = (ap + bp)/2, midacp = (ap + cp)/2, midbcp = (bp + cp)/2;
        uint midabIndex = (midabn*(midabn+1))/2 + midabp;
        uint midacIndex = (midacn*(midacn+1))/2 + midacp;
        uint midbcIndex = (midbcn*(midbcn+1))/2 + midbcp;

        A = std::unique_ptr<TriangularSubdivision<Point> >(new TriangularSubdivision<Point>);
        B = std::unique_ptr<TriangularSubdivision<Point> >(new TriangularSubdivision<Point>);
        C = std::unique_ptr<TriangularSubdivision<Point> >(new TriangularSubdivision<Point>);
        O = std::unique_ptr<TriangularSubdivision<Point> >(new TriangularSubdivision<Point>);

        /* C++14
        A = std::make_unique< TriangularSubdivision<Point> >();
        B = std::make_unique< TriangularSubdivision<Point> >();
        C = std::make_unique< TriangularSubdivision<Point> >();
        O = std::make_unique< TriangularSubdivision<Point>> ();
        */


        A->constructIndices(aIndex, midabIndex, midacIndex, depth - 1, totalDepth, an, midabn, midacn, ap, midabp, midacp, points);
        B->constructIndices(midabIndex, bIndex, midbcIndex, depth - 1, totalDepth, midabn, bn, midbcn, midabp, bp, midbcp, points);
        C->constructIndices(midacIndex, midbcIndex, cIndex, depth - 1, totalDepth, midacn, midbcn, cn, midacp, midbcp, cp, points);
        O->constructIndices(midbcIndex, midacIndex, midabIndex, depth - 1, totalDepth, midbcn, midacn, midabn, midbcp, midacp, midabp, points);
    }
}

template <typename Point> void TriangularSubdivision<Point>::initializeSubdivisionByMidpoints(const Point &a, const Point &b, const Point &c)
{
    uint N = nbPoints(depth);
    uint L = nbLines(depth);
    points->resize(N);
    std::vector<bool> filled;
    filled.resize(N);
    std::fill(filled.begin(), filled.end(), false);

    points->at(aIndex) = a;
    points->at(bIndex) = b;
    points->at(cIndex) = c;
    filled[aIndex] = true;
    filled[bIndex] = true;
    filled[cIndex] = true;

    uint an = 0, ap = 0, bn = L - 1, bp = 0, cn = L - 1, cp = L - 1;

    constructMidpoints(an, bn, cn, ap, bp, cp, filled);
}


template <typename Point> void TriangularSubdivision<Point>::constructMidpoints(uint an, uint bn, uint cn,
                                                                                uint ap, uint bp, uint cp, std::vector<bool> &filled)
{
    if (depth != 0)
    {
        uint midabn = (an + bn)/2, midacn = (an + cn)/2, midbcn = (bn + cn)/2;
        uint midabp = (ap + bp)/2, midacp = (ap + cp)/2, midbcp = (bp + cp)/2;
        uint midabIndex = (midabn*(midabn+1))/2 + midabp;
        uint midacIndex = (midacn*(midacn+1))/2 + midacp;
        uint midbcIndex = (midbcn*(midbcn+1))/2 + midbcp;

        if (!filled[midabIndex])
        {
            points->at(midabIndex) = Point::midpoint(points->at(aIndex), points->at(bIndex));
            filled[midabIndex] = true;
        }
        if (!filled[midacIndex])
        {
            points->at(midacIndex) = Point::midpoint(points->at(aIndex), points->at(cIndex));
            filled[midacIndex] = true;
        }
        if (!filled[midbcIndex])
        {
            points->at(midbcIndex) = Point::midpoint(points->at(bIndex), points->at(cIndex));
            filled[midbcIndex] = true;
        }

        A->constructMidpoints(an, midabn, midacn, ap, midabp, midacp, filled);
        B->constructMidpoints(midabn, bn, midbcn, midabp, bp, midbcp, filled);
        C->constructMidpoints(midacn, midbcn, cn, midacp, midbcp, cp, filled);
        O->constructMidpoints(midbcn, midacn, midabn, midbcp, midacp, midabp, filled);
    }
}



template <typename Point> TriangularSubdivision<Point>::TriangularSubdivision(const std::vector<Point> &points, uint depth)
{
    if (nbPoints(depth) == points.size())
    {
        this->points = std::make_shared< std::vector<Point> >(points);
        totalDepth = depth;
        uint L = nbLines(depth), N = nbPoints(depth);

        uint an = 0, ap = 0, bn = L - 1, bp = 0, cn = L - 1, cp = L - 1;
        aIndex = 0;
        cIndex = N - 1;
        bIndex = cIndex - cp;

        constructIndices(aIndex, bIndex, cIndex, depth, depth, an, bn, cn, ap, bp, cp, this->points);
    }
    else
    {
        throw(QString("Error in H2TriangleSubdivision::H2TriangleSubdivision(const std::vector<H2Point> &points, int depth): points.size() does not match expected size"));
    }
}


template <typename Point> uint TriangularSubdivision<Point>::nbPoints(uint depth)
{
    uint L = nbLines(depth);
    return L*(L+1)/2;
}

template <typename Point> uint TriangularSubdivision<Point>::nbBoundaryPoints(uint depth)
{
    return 3*(nbLines(depth) - 1);
}

template <typename Point> uint TriangularSubdivision<Point>::nbInteriorPoints(uint depth)
{
    return nbPoints(depth) - nbBoundaryPoints(depth);
}

template <typename Point> uint TriangularSubdivision<Point>::nbLines(uint depth)
{
    return 1 + Tools::exponentiation(2, depth);
}

template <typename Point> uint TriangularSubdivision<Point>::getTotalDepth() const
{
    return totalDepth;
}

template <typename Point> bool TriangularSubdivision<Point>::isRoot() const
{
    return depth == totalDepth;
}

template <typename Point> std::vector<Point> TriangularSubdivision<Point>::getPoints() const
{
    return *points;
}

template <typename Point> Point TriangularSubdivision<Point>::getPoint(uint index) const
{
    return points->at(index);
}

template <typename Point> void TriangularSubdivision<Point>::getBigTriangle(Point &outA, Point &outB, Point &outC) const
{
    outA = points->at(aIndex);
    outB = points->at(bIndex);
    outC = points->at(cIndex);
}

template <> bool TriangularSubdivision<H2Point>::triangleContaining(const H2Point &point, H2Point &outA, H2Point &outB, H2Point &outC,
                                                                    uint &index1, uint&index2, uint &index3) const
{
    getBigTriangle(outA, outB, outC);
    index1 = aIndex;
    index2 = bIndex;
    index3 = cIndex;

    if (H2Triangle(outA, outB, outC).contains(point))
    {
        if (depth == 0)
        {
            return true;
        }
        else
        {
            if (A->triangleContaining(point, outA, outB, outC, index1, index2, index3) ||
                    B->triangleContaining(point, outA, outB, outC, index1, index2, index3) ||
                    C->triangleContaining(point, outA, outB, outC, index1, index2, index3) ||
                    O->triangleContaining(point, outA, outB, outC, index1, index2, index3))
            {
                return true;
            }
            else
            {
                qDebug() << "Warning in H2TriangleSubdivision::getTriangleContaining: I could not find a smallest triangle";
                return false;
            }
        }
    }
    else
    {
        return false;
    }
}

template <typename Point> std::vector<bool> TriangularSubdivision<Point>::areBoundaryPoints() const
{
    if (depth != totalDepth)
    {
        throw(QString("ERROR in H2TriangleSubdivision::areBoundaryPoints : not a root subdivision"));
    }

    std::vector<bool> res(points->size());
    std::fill(res.begin(), res.end(), false);


    uint i, k=0, L = nbLines(totalDepth);
    res[0] = true;
    for (i=1; i<L-1; i++)
    {
        k++;
        res[k] = true;
        k += i;
        res[k] = true;
    }

    k++;
    std::fill(res.begin() + k, res.end(), true);

    return res;
}

template <typename Point> std::vector<std::vector<uint> > TriangularSubdivision<Point>::neighborsIndices() const
{
    if (depth != totalDepth)
    {
        throw(QString("ERROR in H2TriangleSubdivision::neighborsIndices : not a root subdivision"));
    }
    if (points == nullptr)
    {
        throw(QString("ERROR in H2TriangleSubdivision::neighborsIndices : empty subdivision"));
    }

    std::vector< std::vector<uint> > out;
    out.reserve(points->size());

    std::vector<uint> neighbors = {1, 2};
    out.push_back(neighbors);
    uint i, j, k = 0, L = nbLines(totalDepth);
    for (i=1; i!=L-1; ++i)
    {
        k += i;
        neighbors = {k+1, k-i, k+i+1, k+i+2};
        out.push_back(neighbors);
        for (j=k+1; j<k+i; j++)
        {
            neighbors = {j+1, j-i, j-i-1, j-1, j+i+1, j+i+2};
            out.push_back(neighbors);
        }
        neighbors = {j+1, j-1, j+i+1, j+i+2};
        out.push_back(neighbors);
    }

    k += i;
    neighbors = {k+1, k-i};
    out.push_back(neighbors);
    for (j=k+1; j!=k+i; ++j)
    {
        neighbors = {j+1, j-i, j-i-1, j-1};
        out.push_back(neighbors);
    }
    neighbors = {j-i-1, j-1};
    out.push_back(neighbors);

    return out;
}

template <typename Point> std::vector<uint> TriangularSubdivision<Point>::sidePointsIndices(uint vertexIndex1, uint vertexIndex2) const
{
    if (depth != totalDepth)
    {
        qDebug() << "WARNING in H2TriangleSubdivision::SidePointsIndices: not a root subdivision";
    }

    std::vector<uint> out;
    uint L = nbLines(totalDepth);
    out.resize(L);

    uint i, k;
    if (vertexIndex1 == 0 && vertexIndex2 == 1)
    {
        k = 0;
        for (i=0; i<L; i++)
        {
            k += i;
            out[i] = k;
        }
    }
    else if (vertexIndex1 == 1 && vertexIndex2 == 0)
    {
        k = 0;
        for (i=0; i<L; i++)
        {
            k += i;
            out[L-1-i] = k;
        }
    }
    else if (vertexIndex1 == 0 && vertexIndex2 == 2)
    {
        k = -1;
        for (i=0; i<L; i++)
        {
            k += i + 1;
            out[i] = k;
        }
    }
    else if (vertexIndex1 == 2 && vertexIndex2 == 0)
    {
        k = -1;
        for (i=0; i<L; i++)
        {
            k += i + 1;
            out[L-1-i] = k;
        }
    }
    else if (vertexIndex1 == 1 && vertexIndex2 == 2)
    {
        k = ((L-1)*L)/2;
        for (i=0; i<L; i++)
        {
            out[i] = k;
            k++;
        }
    }
    else if (vertexIndex1 == 2 && vertexIndex2 == 1)
    {
        k = ((L-1)*L)/2;
        for (i=0; i<L; i++)
        {
            out[L-1-i] = k;
            k++;
        }
    }
    else
    {
        QString errorMessage = QString("ERROR in H2TriangleSubdivision::SidePointsIndices: wrong indices %1 and %2")
                .arg(QString::number(vertexIndex1)).arg(QString::number(vertexIndex2));
        throw(errorMessage);
    }

    return out;
}


template class TriangularSubdivision<H2Point>;
