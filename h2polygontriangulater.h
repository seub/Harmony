#ifndef H2POLYGONTRIANGULATER_H
#define H2POLYGONTRIANGULATER_H

#include "tools.h"


class H2Polygon;

class H2PolygonTriangulater
{
    friend class H2Polygon;

private:
    H2PolygonTriangulater(const H2Polygon * const polygon);
    H2PolygonTriangulater(const H2PolygonTriangulater &);
    H2Polygon & operator=(H2PolygonTriangulater);

    std::vector<double> subpolygonAngles(const std::vector<int> &indices) const;
    void findCutInSubpolygon(const std::vector<int> &indices, int &outputIndex1, int &outputIndex2) const;
    void addCutsForSubpolygon(const std::vector<int> &indices);
    void splitIndicesList(const std::vector<int> & indices, int cut1, int cut2,
                          std::vector<int> &outputList1, std::vector<int> &outputList2) const;


    const H2Polygon * const polygon;
    bool orientation;
    std::vector<int> cuts; // even number of entries: cuts = [i1, j1, i2, j2 ... ]

};

#endif // H2POLYGONTRIANGULATER_H
