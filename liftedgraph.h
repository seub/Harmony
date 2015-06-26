#ifndef LIFTEDGRAPH_H
#define LIFTEDGRAPH_H

#include "tools.h"
#include "discretegroup.h"
#include "grouprepresentation.h"
#include "triangularsubdivision.h"
#include "h2isometry.h"
#include "h2polygontriangulater.h"

class Word; template <typename Point, typename Map> class DiscreteHeatFlowIterator;

class LiftedGraph
{
    friend class DiscreteHeatFlowIterator<H2Point, H2Isometry>;

public:
    LiftedGraph() {}
    std::unique_ptr<LiftedGraph> cloneCopyConstruct() const;
    void cloneCopyAssign(const LiftedGraph *other);
    virtual ~LiftedGraph() {}

    uint getNbPoints() const;
    bool isBoundaryPoint(uint index) const;

protected:
    LiftedGraph(const LiftedGraph &other);
    LiftedGraph & operator=(LiftedGraph);
    virtual LiftedGraph *cloneCopyConstructImpl() const;
    virtual void cloneCopyAssignImpl(const LiftedGraph *other);

    void resetNeighborsWeights(std::vector< std::vector<double> > &newNeighborsWeights);


    DiscreteGroup Gamma;

    uint nbBoundaryPoints,  nbPoints;

    std::vector< std::vector<uint> > neighborsIndices;
    std::vector< std::vector<double> > neighborsWeights;

    std::vector< std::vector<Word> > boundaryPointsNeighborsPairings;
    std::vector< std::vector<uint> > boundaryPointsPartnersIndices;
};



template <typename Point, typename Map> class LiftedGraphFunction : public LiftedGraph
{
    friend class DiscreteHeatFlowIterator<H2Point, H2Isometry>;

public:
    LiftedGraphFunction() {}
    LiftedGraphFunction<Point, Map> & operator=(LiftedGraphFunction<Point, Map>) = delete;
    std::unique_ptr<LiftedGraphFunction<Point, Map> > cloneCopyConstruct() const;
    void cloneCopyAssign(const LiftedGraphFunction<Point, Map> *other);
    virtual ~LiftedGraphFunction() {}

    GroupRepresentation<Map> getRepresentation() const;
    Point getValue(uint index) const;
    std::vector<Point> getValues() const;
    std::vector<Point> getNeighborsValues(uint index) const;
    std::vector<Point> getNeighborsValuesKicked(uint index) const;
    std::vector<Point> getPartnersValues(uint index) const;

protected:
    LiftedGraphFunction(const LiftedGraphFunction &other);
    virtual LiftedGraph *cloneCopyConstructImpl() const override;
    virtual void cloneCopyAssignImpl(const LiftedGraph *other) override;

    void refreshBoundaryPointsNeighborsPairingsValues();
    virtual void resetValues(const std::vector<Point> &newValues);

    GroupRepresentation<Map> rho;
    std::vector< std::vector<Map> > boundaryPointsNeighborsPairingsValues;
    std::vector<Point> values;
};


class H2Mesh; class TriangulationTriangle; class H2Point;

template <typename Point, typename Map> class LiftedGraphFunctionTriangulated : public LiftedGraphFunction<Point, Map>
{
    friend class MathsContainer;
    friend class FenchelNielsenUser;

private:

public:
    LiftedGraphFunctionTriangulated(const LiftedGraphFunctionTriangulated<Point, Map> &other);
    LiftedGraphFunctionTriangulated<Point, Map> & operator=(LiftedGraphFunctionTriangulated<Point, Map>) = delete;
    std::unique_ptr< LiftedGraphFunctionTriangulated<Point, Map> > cloneCopyConstruct() const;
    void cloneCopyAssign(const LiftedGraphFunctionTriangulated<Point, Map> *other);

    std::vector<Point> getBoundary() const;
    std::vector< std::vector<Point> > getTrianglesUp() const;
    std::vector< std::vector<Point> > getAllTriangles() const;
    std::vector<uint> getSteinerWeights() const;
    std::vector<Point> getFirstVertexOrbit() const;
    double getMinEdgeLengthForRegularTriangulation() const;

    LiftedGraphFunctionTriangulated(const LiftedGraphFunctionTriangulated<H2Point, H2Isometry> &domainFunction, const GroupRepresentation<Map> &rhoImage);
    LiftedGraphFunctionTriangulated(const GroupRepresentation<H2Isometry> &rhoDomain, const GroupRepresentation<Map> &rhoImage, uint depth);

    // Specialization to Point = H2Point, Map = H2Isometry
    LiftedGraphFunctionTriangulated(const GroupRepresentation<H2Isometry> &rhoDomain, uint depth);
    std::vector<H2Triangle> getH2TrianglesUp() const;
    std::vector<H2Triangle> getAllH2Triangles() const;
    bool triangleContaining(const H2Point &point, H2Triangle &triangleOut, uint &index1Out, uint &index2Out, uint &index3Out) const;
    H2Triangle getH2Triangle(uint index1, uint index2, uint index3) const;

private:    
    LiftedGraphFunctionTriangulated() {}
    virtual LiftedGraph *cloneCopyConstructImpl() const override;
    virtual void cloneCopyAssignImpl(const LiftedGraph *other) override;

    void initializePiecewiseLinear(const std::vector<Point> &polygonVerticesValues);
    void refreshValuesFromSubdivisions();
    void refreshSubdivisionsFromValues();
    virtual void resetValues(const std::vector<Point> &newValues) override;

    // Specialization to Point = H2Point, Map = H2Isometry
    void constructFromH2Mesh(const H2Mesh &mesh);
    void rearrangeOrderForConstructFromH2Mesh(std::vector<uint> newIndices);



    uint depth;
    std::vector< TriangularSubdivision<Point> > subdivisions;
    std::vector< std::vector<uint> > subdivisionsPointsIndicesInValues;
    std::vector<TriangulationTriangle> triangles;
};

#endif // LIFTEDGRAPH_H
