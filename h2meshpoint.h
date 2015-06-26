#ifndef H2MESHPOINT_H
#define H2MESHPOINT_H

#include "tools.h"
#include "h2isometry.h"
#include "word.h"

template <typename Point, typename Map> class LiftedGraphFunctionTriangulated;

class H2MeshPoint
{
    friend class H2Mesh;
    friend class H2MeshConstructor;
    friend class H2MeshFunction;
    friend class H2MeshFunctionIterator;
    friend class LiftedGraphFunctionTriangulated<H2Point, H2Isometry>;

public:
    H2MeshPoint() = delete;
    virtual ~H2MeshPoint() {}

protected:
    virtual bool isCutPoint() const {return false;}
    virtual bool isBoundaryPoint() const {return false;}
    virtual bool isVertexPoint() const {return false;}
    virtual bool isSteinerPoint() const {return false;}
    bool isInteriorPoint() const {return !(isBoundaryPoint() || isVertexPoint() || isSteinerPoint());}

    H2MeshPoint(uint subdivisionIndex, uint indexInSubdivision, uint index) : subdivisionIndex(subdivisionIndex), indexInSubdivision(indexInSubdivision), index(index) {}

    uint subdivisionIndex, indexInSubdivision;
    std::vector<uint> neighborsIndices;
    std::vector<double> neighborsWeights;
    double weight;
    uint index;
};


class H2MeshCutPoint : public H2MeshPoint
{
    friend class H2Mesh;
    friend class H2MeshConstructor;
    friend class LiftedGraphFunctionTriangulated<H2Point, H2Isometry>;

private:
    H2MeshCutPoint(uint subdivisionIndexLeft, uint indexInSubdivisionLeft, uint subdivisionIndexRight, uint indexInSubdivisionRight, uint index) :
        H2MeshPoint(subdivisionIndexLeft, indexInSubdivisionLeft, index),
        subdivisionIndexRight(subdivisionIndexRight), indexInSubdivisionRight(indexInSubdivisionRight) {}
    bool isCutPoint() const override final {return true;}

    uint subdivisionIndexRight, indexInSubdivisionRight;
};



class H2MeshBoundaryPoint : public H2MeshPoint
{
    friend class H2Mesh;
    friend class H2MeshConstructor;
    friend class H2MeshFunctionIterator;
    friend class H2MeshFunction;
    friend class LiftedGraphFunctionTriangulated<H2Point, H2Isometry>;

private:
    H2MeshBoundaryPoint(uint subdivisionIndex, uint indexInSubdivision, uint side, uint index) :
        H2MeshPoint(subdivisionIndex, indexInSubdivision, index), side(side) {}
    bool isBoundaryPoint() const override final {return true;}

    uint side;
    uint partnerPointIndex;
    std::vector<Word> neighborsPairings;
};


class H2MeshVertexPoint : public H2MeshPoint
{
    friend class H2Mesh;
    friend class H2MeshConstructor;
    friend class H2MeshFunctionIterator;
    friend class H2MeshFunction;
    friend class LiftedGraphFunctionTriangulated<H2Point, H2Isometry>;

private:
    H2MeshVertexPoint(uint subdivisionIndex, uint indexInSubdivision, uint vertexIndex,
                      std::vector<uint> subdivisionIndices, std::vector<uint> indicesInSubdivisions, uint index) :
        H2MeshPoint(subdivisionIndex, indexInSubdivision, index), vertexIndex(vertexIndex),
        subdivisionIndices(subdivisionIndices), indicesInSubdivisions(indicesInSubdivisions) {}
    bool isVertexPoint() const override final {return true;}

    uint vertexIndex;
    std::vector<Word> neighborsPairings;
    std::vector<uint> subdivisionIndices, indicesInSubdivisions;
};

class H2MeshSteinerPoint : public H2MeshPoint
{
    friend class H2Mesh;
    friend class H2MeshConstructor;
    friend class H2MeshFunctionIterator;
    friend class H2MeshFunction;
    friend class LiftedGraphFunctionTriangulated<H2Point, H2Isometry>;

private:
    H2MeshSteinerPoint(uint subdivisionIndex, uint indexInSubdivision, uint side,
                      std::vector<uint> subdivisionIndices, std::vector<uint> indicesInSubdivisions, uint index) :
        H2MeshPoint(subdivisionIndex, indexInSubdivision, index), side(side),
        subdivisionIndices(subdivisionIndices), indicesInSubdivisions(indicesInSubdivisions) {}
    bool isSteinerPoint() const override final {return true;}

    uint side;
    uint partnerPointIndex;
    std::vector<Word> neighborsPairings;
    std::vector<uint> subdivisionIndices, indicesInSubdivisions;
};


#endif // H2MESHPOINT_H
