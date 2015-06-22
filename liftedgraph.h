#ifndef LIFTEDGRAPH_H
#define LIFTEDGRAPH_H

#include "tools.h"
#include "discretegroup.h"
#include "h2isometry.h"
#include "grouprepresentation.h"


class Word;

class LiftedGraph
{
public:
    uint nbPoints() const;

private:
    DiscreteGroup Gamma;

    uint nbBoundaryPoints, nbInteriorPoints;

    std::vector< std::vector<uint> > neighborsIndices;
    std::vector< std::vector<double> > neighborsWeights;

    std::vector< Word > boundaryPointsNeighborsPairings;
    std::vector< std::vector<uint> > boundaryPointsPartnersIndices;
};



class H2Point;

template <typename Point, typename Map> class LiftedGraphRealization : LiftedGraph
{

private:
    GroupRepresentation<Map> rho;
    std::vector<Point> values;
};


template <typename Point> class TriangularSubdivision; class TriangulationTriangle;

template <typename Point, typename Map> class H2TriangulatedLiftedGraph : LiftedGraphRealization<Point, Map>
{
public:
    void initializePiecewiseLinear(const std::vector<H2Point> &boundaryValues);

private:
    std::vector< TriangularSubdivision<Point> > subdivisions;
    std::vector< std::vector<uint> > indicesInSubdivisions;
    std::vector<TriangulationTriangle> triangles;

};

#endif // LIFTEDGRAPH_H
