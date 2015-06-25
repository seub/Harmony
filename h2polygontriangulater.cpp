#include "h2polygontriangulater.h"
#include "h2polygon.h"
#include "h2geodesic.h"

H2PolygonTriangulater::H2PolygonTriangulater(const H2Polygon * const polygon) : polygon(polygon)
{
    orientation = polygon->isPositivelyOriented();

    triangulateSimple();

    /*qDebug() << "min angle = " << minTriangleAngle();
    std::vector<double> polygonAngles = polygon->getPositiveInteriorAngles();
    double minPolygonAngle = *std::min_element(polygonAngles.begin(), polygonAngles.end());
    qDebug() << "min angle in polygon = " << minPolygonAngle;*/
}

double H2PolygonTriangulater::minAngleOfCutInSubpolygon(const std::vector<uint> & indices, uint cut1, uint cut2) const
{
    std::vector<double> angles(4);
    angles[1] = H2Point::angle(fullPolygon.getVertex(indices[cut1==0?indices.size()-1:cut1-1]),
            fullPolygon.getVertex(indices[cut1]), fullPolygon.getVertex(indices[cut2]));
    angles[2] = H2Point::angle(fullPolygon.getVertex(indices[cut2==0?indices.size()-1:cut2-1]),
            fullPolygon.getVertex(indices[cut2]), fullPolygon.getVertex(indices[cut1]));
    angles[3] = H2Point::angle(fullPolygon.getVertex(indices[cut2]),
                               fullPolygon.getVertex(indices[cut1]), fullPolygon.getVertex(indices[cut1+1 >= indices.size()?0:cut1+1]));
    angles[0] = H2Point::angle(fullPolygon.getVertex(indices[cut1]),
                               fullPolygon.getVertex(indices[cut2]), fullPolygon.getVertex(indices[cut2+1 >= indices.size()?0:cut2+1]));
    if(orientation)
    {
        for(auto &a : angles)
        {
            a = 2.0*M_PI - a;
        }
    }

    return *std::min_element(angles.begin(),angles.end());
}


bool TriangulationTriangle::operator <(const TriangulationTriangle &other) const
{
    return ( (vertexIndex1 < other.vertexIndex1)
             || ((vertexIndex1 == other.vertexIndex1) && (vertexIndex2 < other.vertexIndex2))
             || ((vertexIndex1 == other.vertexIndex1) && (vertexIndex2 == other.vertexIndex2) && (vertexIndex3 < other.vertexIndex3)) );
}

void H2PolygonTriangulater::sortTriangles()
{
    std::sort(triangles.begin(), triangles.end());
}

void TriangulationTriangle::getVertices(uint &i1, uint &i2, uint &i3) const
{
    i1 = vertexIndex1;
    i2 = vertexIndex2;
    i3 = vertexIndex3;
}

void TriangulationTriangle::getCutNeighbors(uint &c1, uint &c2, uint &c3) const
{
    c1 = cutNeighborIndex1;
    c2 = cutNeighborIndex2;
    c3 = cutNeighborIndex3;
}

bool H2PolygonTriangulater::isSideTriangle(uint triangleIndex) const
{
    uint i,j,k,N=fullPolygon.nbVertices();
    triangles.at(triangleIndex).getVertices(i,j,k);
    return (j==i+1)||(k==j+1)||(i==0 && k==N-1);
}

void H2PolygonTriangulater::getCutsFromTriangle(uint triangleIndex, uint &cutIndex1, uint &cutIndex2, uint &cutIndex3) const
{
    triangles[triangleIndex].getCutNeighbors(cutIndex1, cutIndex2, cutIndex3);
}

std::vector<uint> H2PolygonTriangulater::nbCutsFromVertex() const
{
    std::vector<uint> res(fullPolygon.nbVertices());
    for (const auto &cut : cuts)
    {
        ++res[cut.vertexIndex1];
        ++res[cut.vertexIndex2];
    }
    return res;
}

void H2PolygonTriangulater::completeCutsAndSides()
{
    cuts.clear();
    uint N = fullPolygon.nbVertices();
    sideTrianglesIndices.resize(N);
    sideTrianglesBoundarySideIndices.resize(N);

    std::vector<uint> left;
    std::vector<uint> right;
    std::vector<bool> filledLeft;
    std::vector<bool> filledRight;
    std::vector<bool> filledSides;
    left.resize(N*N);
    right.resize(N*N);
    filledLeft.resize(N*N);
    filledRight.resize(N*N);
    filledSides.resize(N);
    std::fill(filledLeft.begin(), filledLeft.end(), false);
    std::fill(filledRight.begin(), filledRight.end(), false);
    std::fill(filledSides.begin(), filledSides.end(), false);



    uint i, j, k, M = triangles.size();
    uint i1, i2, i3;
    for (i = 0; i<M; i++)
    {
        triangles[i].getVertices(i1, i2, i3);
        if (i2 == i1 + 1)
        {
            if (filledSides[i1])
            {
                throw(QString("Error in H2PolygonTriangulater::completeCutsAndSides: side already affected"));
            }
            sideTrianglesIndices[i1] = i;
            sideTrianglesBoundarySideIndices[i1] = 0;
            filledSides[i1] = true;
        }
        else
        {
            if (filledRight[N*i1 + i2])
            {
                throw(QString("Error in H2PolygonTriangulater::completeCutsAndSides: right already affected"));
            }
            right[N*i1 + i2] = i;
            filledRight[N*i1 + i2] = true;
        }
        if (i3 == i2 + 1)
        {
            if (filledSides[i2])
            {
                throw(QString("Error in H2PolygonTriangulater::completeCutsAndSides: side already affected"));
            }
            sideTrianglesIndices[i2] = i;
            sideTrianglesBoundarySideIndices[i2] = 1;
            filledSides[i2] = true;
        }
        else
        {
            if (filledRight[N*i2 + i3])
            {
                throw(QString("Error in H2PolygonTriangulater::completeCutsAndSides: right already affected"));
            }
            right[N*i2 + i3] = i;
            filledRight[N*i2 + i3] = true;
        }
        if (i1 == 0 && i3 == N-1)
        {
            if (filledSides[N-1])
            {
                throw(QString("Error in H2PolygonTriangulater::completeCutsAndSides: side already affected"));
            }
            sideTrianglesIndices[N-1] = i;
            sideTrianglesBoundarySideIndices[N-1] = 2;
            filledSides[N-1] = true;
        }
        else
        {
            if (filledLeft[N*i1 + i3])
            {
                throw(QString("Error in H2PolygonTriangulater::completeCutsAndSides: left already affected"));
            }

            left[N*i1 + i3] = i;
            filledLeft[N*i1 + i3] = true;
        }
    }

    for (i=0; i<N; i++)
    {
        for (j=0; j<N; j++)
        {
            k = N*i + j;
            if (j < i + 2 && (filledLeft[k] || filledRight[k]))
            {
                throw(QString("Error in H2PolygonTriangulater::completeCutsAndSides: not supposed to be filled"));
            }
            if (filledLeft[k] ^ filledRight[k])
            {
                throw(QString("Error in H2PolygonTriangulater::completeCutsAndSides: left and right don't match"));
            }
            if (filledLeft[k] && filledRight[k])
            {
                cuts.push_back(TriangulationCut(i, j, left[k], right[k]));
            }
        }
    }

    for (i=0; i<N; i++)
    {
        if (!filledSides[i])
        {
            throw(QString("Error in H2PolygonTriangulater::completeCutsAndSides: not all sides were filled"));
        }
    }

    if (triangles.size() != 1 + cuts.size())
    {
        throw(QString("Error in H2PolygonTriangulater::completeCutsAndSides: Euler test failed"));
    }
}


void H2PolygonTriangulater::triangulateSimple()
{
    clock_t start,end;
    start = clock();

    createSteinerPoints();

    uint i, N = fullPolygon.nbVertices();

    sideTrianglesIndices.reserve(N);
    sideTrianglesBoundarySideIndices.reserve(N);
    cuts.reserve(N-3);
    triangles.reserve(N-2);

    TriangulationCut cut;

    sideTrianglesIndices.push_back(0);
    sideTrianglesBoundarySideIndices.push_back(0);
    sideTrianglesIndices.push_back(0);
    sideTrianglesBoundarySideIndices.push_back(1);
    for (i=2; i<N-1; ++i)
    {
        cut = TriangulationCut(0,i,i-2,i-1);
        cuts.push_back(cut);
        triangles.push_back(TriangulationTriangle(0,i-1,i,i-3,i-2,-1));
        sideTrianglesIndices.push_back(i-1);
        sideTrianglesBoundarySideIndices.push_back(1);
    }
    triangles.push_back(TriangulationTriangle(0,N-2,N-1,N-4,-1,-1));
    sideTrianglesIndices.push_back(N-3);
    sideTrianglesBoundarySideIndices.push_back(2);

    end = clock();
    std::cout << (end - start)*1.0/CLOCKS_PER_SEC << " seconds spent before flipping in H2PolygonTriangulater::triangulateSimple" << std::endl;

    while (attemptFlip()) {}

    end = clock();
    std::cout << (end - start)*1.0/CLOCKS_PER_SEC << " seconds spent after flipping in H2PolygonTriangulater::triangulateSimple" << std::endl;

    sortTriangles();
    completeCutsAndSides();

    end = clock();
    std::cout << (end - start)*1.0/CLOCKS_PER_SEC << " seconds spent after rearranging in H2PolygonTriangulater::triangulateSimple" << std::endl;
}

std::vector<H2Triangle> H2PolygonTriangulater::getTriangles() const
{
    std::vector<H2Triangle> out;
    out.reserve(triangles.size());
    uint i1, i2, i3;
    for (const auto & triangle : triangles)
    {
        triangle.getVertices(i1, i2, i3);
        out.push_back(H2Triangle(fullPolygon.getVertex(i1), fullPolygon.getVertex(i2), fullPolygon.getVertex(i3)));
    }
    return out;
}

std::vector<TriangulationTriangle> H2PolygonTriangulater::getTriangulationTriangles() const
{
    return triangles;
}

std::vector<TriangulationCut> H2PolygonTriangulater::getCuts() const
{
    return cuts;
}

std::vector<H2GeodesicArc> H2PolygonTriangulater::getH2Cuts() const
{
    std::vector<H2GeodesicArc> output;
    for (const auto &cut : cuts)
    {
        output.push_back(H2GeodesicArc(fullPolygon.getVertex(cut.vertexIndex1), fullPolygon.getVertex(cut.vertexIndex2)));
    }
    return output;
}

void H2PolygonTriangulater::adjacentSidesIndices(uint cutIndex, uint &outputIndexLeft1, uint &outputIndexLeft2,
                                                     uint &outputIndexRight1, uint &outputIndexRight2) const
{
    bool failLeft = true;

    TriangulationCut cut = cuts[cutIndex];
    TriangulationTriangle triangleLeft = triangles[cut.leftTriangleIndex];
    TriangulationTriangle triangleRight = triangles[cut.rightTriangleIndex];
    if (triangleLeft.vertexIndex1 == cut.vertexIndex1)
    {
        outputIndexLeft1 = 0;
        if (triangleLeft.vertexIndex3 == cut.vertexIndex2)
        {
            outputIndexLeft2 = 2;
            failLeft = false;
        }
    }


    bool failRight = true;

    if (triangleRight.vertexIndex1 == cut.vertexIndex1)
    {
        outputIndexRight1 = 0;
        if (triangleRight.vertexIndex2 == cut.vertexIndex2)
        {
            outputIndexRight2 = 1;
            failRight = false;
        }
    }
    else if (triangleRight.vertexIndex2 == cut.vertexIndex1)
    {
        outputIndexRight1 = 1;
        if (triangleRight.vertexIndex3 == cut.vertexIndex2)
        {
            outputIndexRight2 = 2;
            failRight = false;
        }
    }

    if (failLeft || failRight)
    {
        qDebug() << "ERROR in H2PolygonTriangulater::getAdjacentSidesIndices: failed";
    }
}

void H2PolygonTriangulater::verticesIndices(std::vector< std::vector<uint> > &triangleIndices,
                                                std::vector< std::vector<uint> > &indicesInTriangles) const
{
    uint i, nbTriangles = triangles.size();
    uint N = fullPolygon.nbVertices();
    triangleIndices.resize(N);
    indicesInTriangles.resize(N);

    TriangulationTriangle T;
    for (i=0; i!=nbTriangles; ++i)
    {
        T = triangles[i];

        triangleIndices[T.vertexIndex1].push_back(i);
        indicesInTriangles[T.vertexIndex1].push_back(0);

        triangleIndices[T.vertexIndex2].push_back(i);
        indicesInTriangles[T.vertexIndex2].push_back(1);

        triangleIndices[T.vertexIndex3].push_back(i);
        indicesInTriangles[T.vertexIndex3].push_back(2);
    }
}


void H2PolygonTriangulater::createSteinerPoints()
{
    std::vector<uint> nbSteinerPoints;
    std::vector<H2GeodesicArc> sides = polygon->getSides();
    std::vector<double> sideLengths;
    for (const auto & side : sides)
    {
        sideLengths.push_back(side.length());
    }
    double minlength = *std::min_element(sideLengths.begin(), sideLengths.end());
    for (const auto & sideLength : sideLengths)
    {
        nbSteinerPoints.push_back(Tools::intRound(2*sideLength/minlength)-1);
    }

    steinerPolygon = H2SteinerPolygon(polygon->getVertices(), nbSteinerPoints);
    fullPolygon = steinerPolygon.getFullPolygon();
}

void H2PolygonTriangulater:: createSteinerPointsDetailed()
{
    std::vector<uint> nbSteinerPoints;
    std::vector<H2GeodesicArc> sides = polygon->getSides();
    std::vector<double> sideDistances, sideLengths;
    for (const auto & side : sides)
    {
        for (const auto & otherSide : sides)
        {
            if (!H2GeodesicArc::shareEndpoint(side,otherSide))
            {
                sideDistances.push_back(H2Geodesic::distanceGeodesics(side.getGeodesic(),otherSide.getGeodesic()));
            }
        }
        sideLengths.push_back(side.length());
    }
    double minDistance;
    minDistance = *std::min_element(sideDistances.begin(),sideDistances.end());
    for (const auto & sideLength : sideLengths)
    {
        nbSteinerPoints.push_back(Tools::intRound(2*sideLength/minDistance)-1);
    }
    steinerPolygon = H2SteinerPolygon(polygon->getVertices(), nbSteinerPoints);
    fullPolygon = steinerPolygon.getFullPolygon();
}

bool H2PolygonTriangulater::sameSide(uint fullIndex1, uint fullIndex2) const
{
    return steinerPolygon.lieOnSameActualSide(fullIndex1, fullIndex2);
}

bool H2PolygonTriangulater::attemptFlip()
{
    // assert polygon is triangulated

    // For each cut, build a quadrilateral out of its pair of neighboring triangles. Check if the opposite
    // cut does a better job of maximizing minimum angle. If it does, replace the cut with its opposite, replace
    // the triangles with the rotated copies (keeping left/right from point of view of the cut), and adjust the
    // neighboring sides so they know who their new triangle neighbors are.

    TriangulationTriangle T1,T2;
    TriangulationCut newCut;
    uint sharedIndex1, sharedIndex2, unsharedIndex1, unsharedIndex2, leftTriangleIndex, rightTriangleIndex, l=0;
    bool output=0;
    std::vector<uint> quadrilateralIndices;
    double minAngle1,minAngle2;
    quadrilateralIndices.reserve(4);
    uint l1,l2,l3,k1,k2,k3;

    bool rightTriangleCheats=0; // True iff the first vertex of the rightTriangle is less than the first vertex of the cut.


    while (!output && l<cuts.size())
    {
        quadrilateralIndices.clear();
        sharedIndex1 = cuts[l].vertexIndex1;
        sharedIndex2 = cuts[l].vertexIndex2;
        leftTriangleIndex = cuts[l].leftTriangleIndex;
        rightTriangleIndex = cuts[l].rightTriangleIndex;
        T1 = triangles[leftTriangleIndex];
        T2 = triangles[rightTriangleIndex];
        T1.getVertices(sharedIndex1,unsharedIndex1,sharedIndex2);
        T2.getVertices(l1,l2,l3);
        if (l1==cuts[l].vertexIndex1)
        {
            rightTriangleCheats = 0;
            T2.getVertices(sharedIndex1,sharedIndex2,unsharedIndex2);
        }
        else if (l2==cuts[l].vertexIndex1)
        {
            rightTriangleCheats = 1;
            T2.getVertices(unsharedIndex2,sharedIndex1,sharedIndex2);
        }
        else if (l1!=cuts[l].vertexIndex1 && l2!=cuts[l].vertexIndex1)
        {
            std::cout << "ERROR in attemptFlip: A triangle abutting a cut doesn't have vertices compatible with the desired cut" << std::endl;
        }

        quadrilateralIndices.push_back(sharedIndex1);
        quadrilateralIndices.push_back(unsharedIndex1);
        quadrilateralIndices.push_back(sharedIndex2);
        quadrilateralIndices.push_back(unsharedIndex2);

        minAngle1 = minAngleOfCutInSubpolygon(quadrilateralIndices,0,2);
        minAngle2 = minAngleOfCutInSubpolygon(quadrilateralIndices,1,3);

        if (minAngle1 < minAngle2)
        {
            getCutsFromTriangle(leftTriangleIndex,l1,l2,l3);
            getCutsFromTriangle(rightTriangleIndex,k1,k2,k3);

            if (l1 < cuts.size())
            {
                cuts[l1].rightTriangleIndex = rightTriangleIndex;
            } else
            {
                sideTrianglesIndices[sharedIndex1] = rightTriangleIndex;
                sideTrianglesBoundarySideIndices[sharedIndex1] = 0;
            }
            if (l3 < cuts.size())
            {
                cuts[l3].rightTriangleIndex = leftTriangleIndex;
            } else
            {
                sideTrianglesIndices[unsharedIndex1] = leftTriangleIndex;
                sideTrianglesBoundarySideIndices[unsharedIndex1] = 0;
            }
            if (!rightTriangleCheats)
            {
                if (k2 < cuts.size())
                {
                    cuts[k2].leftTriangleIndex = rightTriangleIndex;
                } else
                {
                    sideTrianglesIndices[unsharedIndex2] = rightTriangleIndex;
                    sideTrianglesBoundarySideIndices[unsharedIndex2] = 2;
                }
                if (k3 < cuts.size())
                {
                    cuts[k3].rightTriangleIndex = leftTriangleIndex;
                } else
                {
                    sideTrianglesIndices[sharedIndex2] = leftTriangleIndex;
                    sideTrianglesBoundarySideIndices[unsharedIndex2] = 1;
                }
            } else
            {
                if (k1 < cuts.size())
                {
                    cuts[k1].rightTriangleIndex = rightTriangleIndex;
                } else
                {
                    sideTrianglesIndices[unsharedIndex2] = rightTriangleIndex;
                    sideTrianglesBoundarySideIndices[unsharedIndex2] = 0;
                }
                if (k2 < cuts.size())
                {
                    cuts[k2].leftTriangleIndex = leftTriangleIndex;
                } else
                {
                    sideTrianglesIndices[sharedIndex2] = leftTriangleIndex;
                    sideTrianglesBoundarySideIndices[unsharedIndex2] = 2;
                }
            }
            if (!rightTriangleCheats)
            {
                triangles[leftTriangleIndex] = TriangulationTriangle(unsharedIndex1,sharedIndex2,unsharedIndex2,
                                                                         l3, l, k3);
                triangles[rightTriangleIndex] = TriangulationTriangle(sharedIndex1,unsharedIndex1,unsharedIndex2,
                                                                          l1, k2, l);
            } else
            {
                triangles[leftTriangleIndex] = TriangulationTriangle(unsharedIndex2,unsharedIndex1,sharedIndex2,
                                                                         l, k2, l3);
                triangles[rightTriangleIndex] = TriangulationTriangle(unsharedIndex2,sharedIndex1,unsharedIndex1,
                                                                          k1, l, l1);
            }
            if (!rightTriangleCheats)
            {
                cuts[l] = TriangulationCut(unsharedIndex1,unsharedIndex2,leftTriangleIndex,rightTriangleIndex);
            } else
            {
                cuts[l] = TriangulationCut(unsharedIndex2,unsharedIndex1,rightTriangleIndex,leftTriangleIndex);
            }
            return true;
        }
        ++l;
    }
    return false;
}






