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

    H2MeshPoint(int subdivisionIndex, int indexInSubdivision) : subdivisionIndex(subdivisionIndex), indexInSubdivision(indexInSubdivision) {}

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
    H2MeshCutPoint(int subdivisionIndexLeft, int indexInSubdivisionLeft, int subdivisionIndexRight, int indexInSubdivisionRight) :
        H2MeshPoint(subdivisionIndexLeft, indexInSubdivisionLeft),
        subdivisionIndexRight(subdivisionIndexRight), indexInSubdivisionRight(indexInSubdivisionRight) {}
    bool isCutPoint() const {return true;}

    int subdivisionIndexRight, indexInSubdivisionRight;
};



class H2MeshBoundaryPoint : public H2MeshPoint
{
    friend class H2Mesh;
    friend class H2MeshConstructor;
    friend class H2MeshFunctionIterator;

private:
    H2MeshBoundaryPoint(int subdivisionIndex, int indexInSubdivision, int side) :
        H2MeshPoint(subdivisionIndex, indexInSubdivision), side(side) {}
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

private:
    H2MeshVertexPoint(int subdivisionIndex, int indexInSubdivision, int vertexIndex,
                      std::vector<int> subdivisionIndices, std::vector<int> indicesInSubdivisions) :
        H2MeshPoint(subdivisionIndex, indexInSubdivision), vertexIndex(vertexIndex),
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

private:
    H2MeshSteinerPoint(int subdivisionIndex, int indexInSubdivision, int vertexIndex, int side,
                      std::vector<int> subdivisionIndices, std::vector<int> indicesInSubdivisions) :
        H2MeshPoint(subdivisionIndex, indexInSubdivision), vertexIndex(vertexIndex), side(side),
        subdivisionIndices(subdivisionIndices), indicesInSubdivisions(indicesInSubdivisions) {}
    bool isSteinerPoint() const {return true;}

    int vertexIndex;
    int side;
    int partnerPointIndex;
    std::vector<Word> neighborsPairings;
    std::vector<int> subdivisionIndices, indicesInSubdivisions;
};


#endif // H2MESHPOINT_H
