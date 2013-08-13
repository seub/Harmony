#include "h2polygontriangulater.h"

#include "h2polygon.h"

H2PolygonTriangulater::H2PolygonTriangulater(const H2Polygon * const polygon) : polygon(polygon)
{
    orientation = polygon->isPositivelyOriented();
}

std::vector<double> H2PolygonTriangulater::subpolygonAngles(const std::vector<int> &indices) const
{
    std::vector<double> res;
    int N = indices.size();
    res.resize(N);

    double angle = H2Point::angle(polygon->getVertex(indices.back()),
                                       polygon->getVertex(indices.front()), polygon->getVertex(indices[1]));
    angle = orientation ? angle : 2.0*M_PI - angle;
    res[0] = angle;

    for (int i =1; i < N - 1; i++)
    {
        angle = H2Point::angle(polygon->getVertex(indices[i - 1]),
                polygon->getVertex(indices[i]), polygon->getVertex(indices[i+1]));
        angle = orientation ? angle : 2.0*M_PI - angle;
        res[i] = angle;
    }

    angle = H2Point::angle(polygon->getVertex(indices[N - 2]),
            polygon->getVertex(indices.back()), polygon->getVertex(indices.front()));
    angle = orientation ? angle : 2.0*M_PI - angle;
    res[N-1] = angle;

    return res;
}

void H2PolygonTriangulater::findCutInSubpolygon(const std::vector<int> &indices, int &outputIndex1, int &outputIndex2) const
{
    std::vector<double> angles = subpolygonAngles(indices);

    double max = angles.front();
    outputIndex1 = 0;
    unsigned int i, N = indices.size();
    for (i=1; i<N; i++)
    {
        if (angles[i] > max)
        {
            max = angles[i];
            outputIndex1 = i;
        }
    }

    max = 0;
    if (outputIndex1 != indices.back() && outputIndex1 != indices.front() && outputIndex1 != indices[1])
    {
        max = angles.front();
        outputIndex2 = 0;
    }
    for(i = indices[1]; i < indices.size() - 1; i++)
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
    if (outputIndex1 != indices[N - 2] && outputIndex1 != indices.back() && outputIndex1 != indices.front())
    {
        if (angles.back() > max)
        {
            outputIndex2 = N - 1;
        }
    }

    if (outputIndex1 > outputIndex2)
    {
        std::swap(outputIndex1, outputIndex2);
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

    int i;
    for (i=cut1; i<=cut2; i++)
    {
        outputList1.push_back(indices[i]);
    }

    i = cut2;
    while (i <= cut1)
    {
        outputList2.push_back(indices[i]);
        i++;
        if (i == N)
        {
            i = 0;
        }
    }
    return;
}

void H2PolygonTriangulater::triangulateSubpolygon(const std::vector<int> &indices)
{
    int cut1, cut2;
    return;
}
