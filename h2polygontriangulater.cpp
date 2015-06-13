#include "h2polygontriangulater.h"

#include "h2polygon.h"
#include "h2geodesic.h"

H2PolygonTriangulater::H2PolygonTriangulater(const H2Polygon * const polygon) : polygon(polygon)
{
    orientation = polygon->isPositivelyOriented();
    triangulate();
    //std::cout << "min angle = " << minTriangleAngle() << std::endl;
    //std::vector<double> polygonAngles = polygon->getPositiveInteriorAngles();
    //double minPolygonAngle = *std::min_element(polygonAngles.begin(), polygonAngles.end());
    //std::cout << "min angle in polygon = " << minPolygonAngle << std::endl;
}

double H2PolygonTriangulater::minAngleOfCutInSubpolygon(const std::vector<int> & indices, unsigned int cut1, unsigned int cut2) const
{
    std::vector<double> angles(4);
    angles[1] = H2Point::angle(fullPolygon->getVertex(indices[cut1==0?indices.size()-1:cut1-1]),
            fullPolygon->getVertex(indices[cut1]), fullPolygon->getVertex(indices[cut2]));
    angles[2] = H2Point::angle(fullPolygon->getVertex(indices[cut2==0?indices.size()-1:cut2-1]),
                    fullPolygon->getVertex(indices[cut2]), fullPolygon->getVertex(indices[cut1]));
    angles[3] = H2Point::angle(fullPolygon->getVertex(indices[cut2]),
                    fullPolygon->getVertex(indices[cut1]), fullPolygon->getVertex(indices[cut1+1 >= indices.size()?0:cut1+1]));
    angles[0] = H2Point::angle(fullPolygon->getVertex(indices[cut1]),
                    fullPolygon->getVertex(indices[cut2]), fullPolygon->getVertex(indices[cut2+1 >= indices.size()?0:cut2+1]));
    if(orientation)
    {
        for(auto &a : angles)
        {
            a = 2.0*M_PI - a;
        }
    }

    return *std::min_element(angles.begin(),angles.end());
}


std::vector<double> H2PolygonTriangulater::subpolygonAngles(const std::vector<int> &indices) const
{
    std::vector<double> res;
    int N = indices.size();
    res.resize(N);

    double angle = H2Point::angle(fullPolygon->getVertex(indices.back()),
                                  fullPolygon->getVertex(indices.front()), fullPolygon->getVertex(indices[1]));
    angle = orientation ? 2.0*M_PI - angle : angle;
    res[0] = angle;

    for (int i =1; i < N - 1; i++)
    {
        angle = H2Point::angle(fullPolygon->getVertex(indices[i - 1]),
                fullPolygon->getVertex(indices[i]), fullPolygon->getVertex(indices[i+1]));
        angle = orientation ? 2.0*M_PI - angle : angle;
        res[i] = angle;
    }

    angle = H2Point::angle(fullPolygon->getVertex(indices[N - 2]),
            fullPolygon->getVertex(indices.back()), fullPolygon->getVertex(indices.front()));
    angle = orientation ? 2.0*M_PI - angle : angle;
    res[N-1] = angle;

    return res;
}


void H2PolygonTriangulater::findCutInSubpolygon1(const std::vector<int> &indices, int &outputIndex1, int &outputIndex2) const
{
    std::vector<double> angles = subpolygonAngles(indices);

    double max = angles.front();
    outputIndex1 = 0;
    int i, N = indices.size();
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
    return;
}

void H2PolygonTriangulater::findCutInSubpolygon2(const std::vector<int> &indices, int &outputIndex1, int &outputIndex2) const
{
    unsigned int i,j;
    double max = 0, angle;
    for(j=2;j<indices.size()-1;++j)
    {
        angle = minAngleOfCutInSubpolygon(indices,0,j);
        if(angle>max)
        {
            max = angle;
            outputIndex1 = 0;
            outputIndex2 = j;
        }
    }
    for(i = 1;i+2<indices.size();++i)
    {
        for(j=i+2;j<indices.size();++j)
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
    return;
}

void H2PolygonTriangulater::findCutInSubpolygon3(const std::vector<int> &indices, int &outputIndex1, int &outputIndex2) const
{
    std::vector<double> angles = subpolygonAngles(indices);

    double min = angles.front();
    int minAngleVertex = 0;
    int i, N = indices.size();
    for (i=1; i<N; i++)
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
    return;
}

void H2PolygonTriangulater::splitIndicesList(const std::vector<int> &indices, int cut1, int cut2,
                                             std::vector<int> &outputList1, std::vector<int> &outputList2) const
{
    int N = indices.size();
    outputList1.clear();
    outputList2.clear();
    outputList1.reserve(N);
    outputList2.reserve(N);

    int i = 0;
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

    return;
}

void H2PolygonTriangulater::triangulateSubpolygon(const std::vector<int> &indices)
{
    if (indices.size() == 3)
    {
        triangles.push_back(TriangulationTriangle(indices[0], indices[1], indices[2]));
        return;
    }
    else
    {
        int cut1, cut2;

        findCutInSubpolygon2(indices, cut1, cut2);
        std::vector<int> list1, list2;
        splitIndicesList(indices, cut1, cut2, list1, list2);

        triangulateSubpolygon(list1);
        triangulateSubpolygon(list2);
    }

    return;
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
    return;
}

void TriangulationTriangle::getVertices(int &i1, int &i2, int &i3) const
{
    i1 = vertexIndex1;
    i2 = vertexIndex2;
    i3 = vertexIndex3;
    return;
}

std::vector<int> H2PolygonTriangulater::nbCutsFromVertex() const
{
    std::vector<int> res(fullPolygon->nbVertices());
    for (const auto &cut : cuts)
    {
        ++res[cut.vertexIndex1];
        ++res[cut.vertexIndex2];
    }
    return res;
}

void H2PolygonTriangulater::completeCutsAndSides()
{
    int N = fullPolygon->nbVertices();
    sideTrianglesIndices.resize(N);
    sideTrianglesBoundarySideIndices.resize(N);

    std::vector<int> left;
    std::vector<int> right;
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



    int i, j, k, M = triangles.size();
    int i1, i2, i3;
    for (i = 0; i<M; i++)
    {
        triangles[i].getVertices(i1, i2, i3);
        if (i2 == i1 + 1)
        {
            if (filledSides[i1])
            {
                std::cout << "ERROR in H2PolygonTriangulater::completeCutsAndSides: side already affected" << std::endl;
            }
            sideTrianglesIndices[i1] = i;
            sideTrianglesBoundarySideIndices[i1] = 0;
            filledSides[i1] = true;
        }
        else
        {
            if (filledRight[N*i1 + i2])
            {
                std::cout << "ERROR in H2PolygonTriangulater::completeCutsAndSides: right already affected" << std::endl;
            }
            right[N*i1 + i2] = i;
            filledRight[N*i1 + i2] = true;
        }
        if (i3 == i2 + 1)
        {
            if (filledSides[i2])
            {
                std::cout << "ERROR in H2PolygonTriangulater::completeCutsAndSides: side already affected" << std::endl;
            }
            sideTrianglesIndices[i2] = i;
            sideTrianglesBoundarySideIndices[i2] = 1;
            filledSides[i2] = true;
        }
        else
        {
            if (filledRight[N*i2 + i3])
            {
                std::cout << "ERROR in H2PolygonTriangulater::completeCutsAndSides: right already affected" << std::endl;
            }
            right[N*i2 + i3] = i;
            filledRight[N*i2 + i3] = true;
        }
        if (i1 == 0 && i3 == N-1)
        {
            if (filledSides[N-1])
            {
                std::cout << "ERROR in H2PolygonTriangulater::completeCutsAndSides: side already affected" << std::endl;
            }
            sideTrianglesIndices[N-1] = i;
            sideTrianglesBoundarySideIndices[N-1] = 2;
            filledSides[N-1] = true;
        }
        else
        {
            if (filledLeft[N*i1 + i3])
            {
                std::cout << "ERROR in H2PolygonTriangulater::completeCutsAndSides: left already affected" << std::endl;
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
                std::cout << "ERROR in H2PolygonTriangulater::completeCutsAndSides: not supposed to be filled" << std::endl;
            }
            if (filledLeft[k] ^ filledRight[k])
            {
                std::cout << "ERROR in H2PolygonTriangulater::completeCutsAndSides: left and right don't match" << std::endl;
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
            std::cout << "ERROR in H2PolygonTriangulater::completeCutsAndSides: not all sides were filled" << std::endl;
        }
    }

    if (triangles.size() != 1 + cuts.size())
    {
        std::cout << "ERROR in H2PolygonTriangulater::completeCutsAndSides: Euler test failed" << std::endl;
    }

    return;
}

void H2PolygonTriangulater::triangulate()
{

    std::vector<int> indices;
    int i, N = fullPolygon->nbVertices();
    indices.resize(N);
    for (i=0; i<N; i++)
    {
        indices[i] = i;
    }

    triangulateSubpolygon(indices);
    sortTriangles();
    completeCutsAndSides();
    return;
}

std::vector<H2Triangle> H2PolygonTriangulater::getTriangles() const
{
    std::vector<H2Triangle> res;
    res.reserve(triangles.size());
    int i1, i2, i3;
    for (unsigned int i=0; i<triangles.size(); i++)
    {
        triangles[i].getVertices(i1, i2, i3);
        res.push_back(H2Triangle(fullPolygon->getVertex(i1), fullPolygon->getVertex(i2), fullPolygon->getVertex(i3)));
    }
    return res;
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
        output.push_back(H2GeodesicArc(fullPolygon->getVertex(cut.vertexIndex1), fullPolygon->getVertex(cut.vertexIndex2)));
    }
    return output;
}

void H2PolygonTriangulater::adjacentSidesIndices(int cutIndex, int &outputIndexLeft1, int &outputIndexLeft2,
                                                    int &outputIndexRight1, int &outputIndexRight2) const
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
        std::cout << "ERROR in H2PolygonTriangulater::getAdjacentSidesIndices: failed" << std::endl;
    }
    return;
}

void H2PolygonTriangulater::verticesIndices(std::vector< std::vector<int> > &triangleIndices,
                                            std::vector< std::vector<int> > &indicesInTriangles) const
{
    int i, nbTriangles = triangles.size();
    int N = fullPolygon->nbVertices();
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
    return;
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
    std::vector<H2GeodesicArc> segments = fullPolygon->getSides();
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
    std::vector<int> nbSteinerPoints;
    for(int j=0; j<polygon->nbVertices(); ++j)
    {
        nbSteinerPoints.push_back(1);
    }
    steinerPolygon = H2SteinerPolygon(nbSteinerPoints);
    fullPolygon = steinerPolygon.getFullPolygon();
}
