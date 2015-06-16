#ifndef H2MESHPOINT_H
#define H2MESHPOINT_H

#include "tools.h"
#include "h2isometry.h"
#include "word.h"

class H2MeshPoint
{
    friend class H2Mesh;
    friend class H2MeshConstructor;
    friend class H2MeshFunction;
    friend class H2MeshFunctionIterator;

protected:
    virtual bool isCutPoint() const {return false;}
    virtual bool isBoundaryPoint() const {return false;}
    virtual bool isVertexPoint() const {return false;}
    virtual bool isSteinerPoint() const {return false;}
    bool isInteriorPoint() const {return !(isBoundaryPoint() || isVertexPoint() || isSteinerPoint());}

    H2MeshPoint(int subdivisionIndex, int indexInSubdivision, int index) : subdivisionIndex(subdivisionIndex), indexInSubdivision(indexInSubdivision), index(index) {}

    int subdivisionIndex, indexInSubdivision;
    std::vector<int> neighborsIndices;
    std::vector<double> neighborsWeights;
    double weight;
    int index;
};


class H2MeshCutPoint : public H2MeshPoint
{
    friend class H2Mesh;
    friend class H2MeshConstructor;

private:
    H2MeshCutPoint(int subdivisionIndexLeft, int indexInSubdivisionLeft, int subdivisionIndexRight, int indexInSubdivisionRight, int index) :
        H2MeshPoint(subdivisionIndexLeft, indexInSubdivisionLeft, index),
        subdivisionIndexRight(subdivisionIndexRight), indexInSubdivisionRight(indexInSubdivisionRight) {}
    bool isCutPoint() const {return true;}

    int subdivisionIndexRight, indexInSubdivisionRight;
};



class H2MeshBoundaryPoint : public H2MeshPoint
{
    friend class H2Mesh;
    friend class H2MeshConstructor;
    friend class H2MeshFunctionIterator;
    friend class H2MeshFunction;

private:
    H2MeshBoundaryPoint(int subdivisionIndex, int indexInSubdivision, int side, int index) :
        H2MeshPoint(subdivisionIndex, indexInSubdivision, index), side(side) {}
    bool isBoundaryPoint() const {return true;}

    int side;
    int partnerPointIndex;
    std::vector<Word> neighborsPairings;
};


class H2MeshVertexPoint : public H2MeshPoint
{
    friend class H2Mesh;
    friend class H2MeshConstructor;
    friend class H2MeshFunctionIterator;
    friend class H2MeshFunction;

private:
    H2MeshVertexPoint(int subdivisionIndex, int indexInSubdivision, int vertexIndex,
                      std::vector<int> subdivisionIndices, std::vector<int> indicesInSubdivisions, int index) :
        H2MeshPoint(subdivisionIndex, indexInSubdivision, index), vertexIndex(vertexIndex),
        subdivisionIndices(subdivisionIndices), indicesInSubdivisions(indicesInSubdivisions) {}
    bool isVertexPoint() const {return true;}

    int vertexIndex;
    std::vector<Word> neighborsPairings;
    std::vector<int> subdivisionIndices, indicesInSubdivisions;
};

class H2MeshSteinerPoint : public H2MeshPoint
{
    friend class H2Mesh;
    friend class H2MeshConstructor;
    friend class H2MeshFunctionIterator;
    friend class H2MeshFunction;

private:
    H2MeshSteinerPoint(int subdivisionIndex, int indexInSubdivision, int side,
                      std::vector<int> subdivisionIndices, std::vector<int> indicesInSubdivisions, int index) :
        H2MeshPoint(subdivisionIndex, indexInSubdivision, index), side(side),
        subdivisionIndices(subdivisionIndices), indicesInSubdivisions(indicesInSubdivisions) {}
    bool isSteinerPoint() const {return true;}

    int side;
    int partnerPointIndex;
    std::vector<Word> neighborsPairings;
    std::vector<int> subdivisionIndices, indicesInSubdivisions;
};


#endif // H2MESHPOINT_H
