#include "h2polygontriangulater.h"

#include "h2polygon.h"
#include "h2geodesic.h"

H2PolygonTriangulater::H2PolygonTriangulater(const H2Polygon * const polygon) : polygon(polygon)
{
    orientation = polygon->isPositivelyOriented();
    triangulate();
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


std::vector<double> H2PolygonTriangulater::subpolygonAngles(const std::vector<uint> &indices) const
{
    std::vector<double> res;
    uint N = indices.size();
    res.resize(N);

    double angle = H2Point::angle(fullPolygon.getVertex(indices.back()),
                                  fullPolygon.getVertex(indices.front()), fullPolygon.getVertex(indices[1]));
    angle = orientation ? 2.0*M_PI - angle : angle;
    res[0] = angle;

    for (uint i=1; i != N-1; ++i)
    {
        angle = H2Point::angle(fullPolygon.getVertex(indices[i - 1]),
                fullPolygon.getVertex(indices[i]), fullPolygon.getVertex(indices[i+1]));
        angle = orientation ? 2.0*M_PI - angle : angle;
        res[i] = angle;
    }

    angle = H2Point::angle(fullPolygon.getVertex(indices[N - 2]),
            fullPolygon.getVertex(indices.back()), fullPolygon.getVertex(indices.front()));
    angle = orientation ? 2.0*M_PI - angle : angle;
    res[N-1] = angle;

    return res;
}


void H2PolygonTriangulater::findCutInSubpolygon1(const std::vector<uint> &indices, uint &outputIndex1, uint &outputIndex2) const
{
    std::vector<double> angles = subpolygonAngles(indices);

    double max = angles.front();
    outputIndex1 = 0;
    uint i, N = indices.size();
    for (i=1; i<N; i++)
    {
        if (angles[i] > max)
        {
            max = angles[i];
            outputIndex1 = i;
        }
    }

    max = 0;
    if (outputIndex1 != N-1 && outputIndex1 != 0 && outputIndex1 != 1)
    {
        max = angles.front();
        outputIndex2 = 0;
    }
    for(i=1; i<N-1; i++)
    {
        if (outputIndex1 != i-1 && outputIndex1 != i && outputIndex1 != i+1)
        {
            if (angles[i] > max)
            {
                max = angles[i];
                outputIndex2 = i;
            }
        }
    }
    if (outputIndex1 != N-2 && outputIndex1 != N-1 && outputIndex1 != 0)
    {
        if (angles.back() > max)
        {
            outputIndex2 = N-1;
        }
    }

    if (outputIndex1 > outputIndex2)
    {
        std::swap(outputIndex1, outputIndex2);
    }
}

void H2PolygonTriangulater::findCutInSubpolygon2(const std::vector<uint> &indices, uint &outputIndex1, uint &outputIndex2) const
{
    uint i,j;
    double max = 0, angle;
    for(j=2;j<indices.size()-1;++j)
    {
        if (!sameSide(indices[0], indices[j]))
        {
            angle = minAngleOfCutInSubpolygon(indices,0,j);
            if(angle>max)
            {
                max = angle;
                outputIndex1 = 0;
                outputIndex2 = j;
            }
        }
    }
    for(i = 1;i+2<indices.size();++i)
    {
        for(j=i+2;j<indices.size();++j)
        {
            if (!sameSide(indices[i], indices[j]))
            {
                angle = minAngleOfCutInSubpolygon(indices,i,j);
                if(angle>max)
                {
                    max = angle;
                    outputIndex1 = i;
                    outputIndex2 = j;
                }
            }
        }
    }
}

void H2PolygonTriangulater::findCutInSubpolygon3(const std::vector<uint> &indices, uint &outputIndex1, uint &outputIndex2) const
{
    std::vector<double> angles = subpolygonAngles(indices);

    double min = angles.front();
    uint minAngleVertex = 0;
    uint i, N = indices.size();
    for (i=1; i!=N; ++i)
    {
        if (angles[i] < min)
        {
            min = angles[i];
            minAngleVertex = i;
        }
    }

    if(minAngleVertex == 0)
    {
        outputIndex1=1;
        outputIndex2 = N -1;
    }
    else if(minAngleVertex == N-1)
    {
        outputIndex1=0;
        outputIndex2 = N -2;
    }
    else
    {
        outputIndex1 = minAngleVertex -1;
        outputIndex2 = minAngleVertex +1;
    }
}

void H2PolygonTriangulater::splitIndicesList(const std::vector<uint> &indices, uint cut1, uint cut2,
                                             std::vector<uint> &outputList1, std::vector<uint> &outputList2) const
{
    uint N = indices.size();
    outputList1.clear();
    outputList2.clear();
    outputList1.reserve(N);
    outputList2.reserve(N);

    uint i = 0;
    do
    {
        outputList1.push_back(indices[i]);
        if (i==cut1)
        {
            i=cut2;
        }
        else
        {
            i++;
        }
    } while (i != N);



    for (i=cut1; i<=cut2; i++)
    {
        outputList2.push_back(indices[i]);
    }
}

void H2PolygonTriangulater::triangulateSubpolygon(const std::vector<uint> &indices)
{
    if (indices.size() == 3)
    {
        triangles.push_back(TriangulationTriangle(indices[0], indices[1], indices[2]));
    }
    else
    {
        uint cut1, cut2;

        findCutInSubpolygon2(indices, cut1, cut2);
        std::vector<uint> list1, list2;
        splitIndicesList(indices, cut1, cut2, list1, list2);

        triangulateSubpolygon(list1);
        triangulateSubpolygon(list2);
    }
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

void H2PolygonTriangulater::triangulate()
{
    createSteinerPoints();

    std::vector<uint> indices;
    uint i, N = fullPolygon.nbVertices();
    indices.resize(N);
    for (i=0; i<N; i++)
    {
        indices[i] = i;
    }

    triangulateSubpolygon(indices);
    sortTriangles();
    completeCutsAndSides();
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

double H2PolygonTriangulater::minTriangleAngle() const
{
    std::vector<double> triangleAngles;
    double angle, minAngle = 10.0;
    for (const auto &T : triangles)
    {
        triangleAngles = subpolygonAngles({T.vertexIndex1, T.vertexIndex2, T.vertexIndex3});
        angle = *std::min_element(triangleAngles.begin(), triangleAngles.end());
        if (angle < minAngle)
        {
            minAngle = angle;
        }
    }
    return minAngle;
}

double H2PolygonTriangulater::minTriangleSide() const
{
    std::vector<H2GeodesicArc> segments = fullPolygon.getSides();
    std::vector<H2GeodesicArc> H2cuts = getH2Cuts();
    segments.insert(segments.end(), H2cuts.begin(), H2cuts.end());

    std::vector<double> segmentLengths;
    for (const auto & segment : segments)
    {
        segmentLengths.push_back(segment.length());
    }

    return *std::min_element(segmentLengths.begin(), segmentLengths.end());
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

bool H2PolygonTriangulater::sameSide(uint fullIndex1, uint fullIndex2) const
{
    return steinerPolygon.lieOnSameActualSide(fullIndex1, fullIndex2);
}
