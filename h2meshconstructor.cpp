#include "h2meshconstructor.h"
#include "triangularsubdivision.h"
#include <Eigen/Dense>
#include <Eigen/LU>



H2MeshConstructor::H2MeshConstructor(H2Mesh *mesh) :
    mesh(mesh), depth(mesh->depth),
    subdivisions(&(mesh->subdivisions)), meshIndicesInSubdivisions(&(mesh->meshIndicesInSubdivisions)), points(&(mesh->meshPoints)),
    regularPoints(&(mesh->regularPoints)), cutPoints(&(mesh->cutPoints)),
    boundaryPoints(&(mesh->boundaryPoints)), vertexPoints(&(mesh->vertexPoints)), steinerPoints(&(mesh->steinerPoints)),
    triangulater(H2PolygonTriangulater(&(mesh->fundamentalDomain)))
{
    mesh->fundamentalSteinerDomain = triangulater.steinerPolygon;
    nbVertices = mesh->fundamentalDomain.nbVertices();
    nbSteinerPoints = mesh->fundamentalSteinerDomain.getTotalNbSteinerPoints();
    nbSubdivisions = triangulater.triangles.size();
    nbSubdivisionLines = TriangularSubdivision<H2Point>::nbLines(depth);
    nbSubdivisionPoints = TriangularSubdivision<H2Point>::nbPoints(depth);
    nextIndex = 0;
    sidePairings = mesh->rho.getDiscreteGroup().getSidePairingsClosedSurface();

    createSubdivisions();
    createPoints();
    createNeighbors();

    reorganizeNeighbors();

    createPiecewiseAffineWeights();

    createMeshExteriorIndices();

    runTests();

}

void H2MeshConstructor::createPoints()
{
    createRegularPoints();
    createCutPoints();
    createBoundaryPoints();
    createVertexAndSteinerPoints();
    mesh->createMeshPointsVector();
}

void H2MeshConstructor::createNeighbors()
{
    createInteriorNeighbors();
    createCutNeighbors();
    createSideNeighbors();
    createRemainingNonExteriorNeighbors();
    createExteriorNeighbors();
    createExteriorVertexNeighbors();
}

void H2MeshConstructor::createSubdivisions()
{
    std::vector<H2Triangle> triangles = triangulater.getTriangles();
    mesh->triangles = triangulater.getTriangulationTriangles();

    uint nbPointsPerSubdivision = TriangularSubdivision<H2Point>::nbPoints(depth);
    TriangularSubdivision<H2Point> TS(depth);
    H2Point A, B, C;
    for (const auto &triangle : triangles)
    {
        triangle.getPoints(A, B, C);
        TS.initializeSubdivisionByMidpoints(A, B, C);
        subdivisions->push_back(TS);
        meshIndicesInSubdivisions->push_back(std::vector<uint>(nbPointsPerSubdivision));
    }

    neighborsInSubdivisions.reserve(nbSubdivisions);
    for (const auto &S : *subdivisions)
    {
        neighborsInSubdivisions.push_back(S.neighborsIndices());
    }
}

void H2MeshConstructor::createRegularPoints()
{
    boundaryPointInSubdivisions.reserve(nbSubdivisions);
    for (const auto & S : *subdivisions)
    {
        boundaryPointInSubdivisions.push_back(S.areBoundaryPoints());
    }


    points->reserve(nbSubdivisions*nbSubdivisionPoints);
    regularPoints->reserve(nbSubdivisions*(nbSubdivisionPoints - boundaryPointInSubdivisions[0].size()));

    uint i, j;
    for (i=0; i!=nbSubdivisions; ++i)
    {
        for (j=0; j!=nbSubdivisionPoints; ++j)
        {
            if (!boundaryPointInSubdivisions[i][j])
            {
                regularPoints->push_back(H2MeshPoint(i, j, nextIndex));
                meshIndicesInSubdivisions->at(i)[j] = nextIndex;
                ++nextIndex;
            }
        }
    }
}

void H2MeshConstructor::createCutPoints()
{
    std::vector<uint> indicesLeft, indicesRight;
    uint i, j, nbCuts = triangulater.cuts.size();
    uint vertexIndexLeft1, vertexIndexLeft2, vertexIndexRight1, vertexIndexRight2;
    TriangulationCut cut ;

    cutPoints->reserve(nbCuts*(nbSubdivisionLines-2));

    for (i=0; i!=nbCuts; ++i)
    {
        cut = triangulater.cuts[i];
        triangulater.adjacentSidesIndices(i, vertexIndexLeft1, vertexIndexLeft2, vertexIndexRight1, vertexIndexRight2);
        indicesLeft = (*subdivisions)[cut.leftTriangleIndex].sidePointsIndices(vertexIndexLeft1, vertexIndexLeft2);
        indicesRight = (*subdivisions)[cut.rightTriangleIndex].sidePointsIndices(vertexIndexRight1, vertexIndexRight2);

        for (j=1; j+1!=nbSubdivisionLines; ++j)
        {
            cutPoints->push_back(H2MeshCutPoint(cut.leftTriangleIndex, indicesLeft[j], cut.rightTriangleIndex, indicesRight[j], nextIndex));
            meshIndicesInSubdivisions->at(cut.leftTriangleIndex)[indicesLeft[j]] = nextIndex;
            meshIndicesInSubdivisions->at(cut.rightTriangleIndex)[indicesRight[j]] = nextIndex;
            ++nextIndex;
        }
    }
}

void H2MeshConstructor::createBoundaryPoints()
{
    boundaryPoints->reserve(nbVertices*(nbSubdivisionLines-1));

    uint triangleIndex, triangleSideIndex;
    std::vector<uint> indices;

    uint i, j, side = 0, indexOnSide=0, nbSteinerPointsOnSide = mesh->fundamentalSteinerDomain.getNbSteinerPointsOnSide(0);
    for (i=0; i!=nbVertices+nbSteinerPoints; ++i)
    {
        if (indexOnSide == nbSteinerPointsOnSide+1)
        {
            indexOnSide = 0;
            ++side;
            nbSteinerPointsOnSide = mesh->fundamentalSteinerDomain.getNbSteinerPointsOnSide(side);
        }


        triangleIndex = triangulater.sideTrianglesIndices[i];
        triangleSideIndex = triangulater.sideTrianglesBoundarySideIndices[i];
        if (triangleSideIndex == 2)
        {
            indices = (*subdivisions)[triangleIndex].sidePointsIndices(2, 0);
        }
        else
        {
            indices = (*subdivisions)[triangleIndex].sidePointsIndices(triangleSideIndex, triangleSideIndex + 1);
        }

        for (j=1; j+1!=nbSubdivisionLines; ++j)
        {
            boundaryPoints->push_back(H2MeshBoundaryPoint(triangleIndex, indices[j], side, nextIndex));
            meshIndicesInSubdivisions->at(triangleIndex)[indices[j]] = nextIndex;
            ++nextIndex;
        }

        ++indexOnSide;
    }
}

void H2MeshConstructor::createVertexAndSteinerPoints()
{
    vertexPoints->reserve(nbVertices);
    steinerPoints->reserve(nbSteinerPoints);
    vertexMeshIndex.resize(nbVertices);
    steinerPointsMeshIndex.resize(nbSteinerPoints);

    std::vector< std::vector<uint> > subdivisionIndices, indicesInTriangles, indicesInSubdivisions;
    triangulater.verticesIndices(subdivisionIndices, indicesInTriangles);
    uint index2 = nbSubdivisionPoints-1, index1 = index2 - nbSubdivisionLines + 1;

    indicesInSubdivisions.resize(nbVertices + nbSteinerPoints);
    uint i, indexInTriangle;
    uint j;
    for (i=0; i!=nbVertices + nbSteinerPoints; ++i)
    {
        for (j=0; j!=indicesInTriangles[i].size(); ++j)
        {
            indexInTriangle = indicesInTriangles[i][j];
            indicesInSubdivisions[i].push_back((indexInTriangle==1)*index1 + (indexInTriangle==2)*index2);
        }
    }


    uint side=0, indexOnSide = 0;
    uint vertexIndex = 0;
    uint steinerPointIndex = 0;
    for (i=0; i!=nbVertices+nbSteinerPoints; ++i)
    {
        if (indexOnSide == mesh->fundamentalSteinerDomain.getNbSteinerPointsOnSide(side) + 1)
        {
            indexOnSide = 0;
            ++side;
        }

        if (indexOnSide==0)
        {
            vertexPoints->push_back(H2MeshVertexPoint(subdivisionIndices[i][0], indicesInSubdivisions[i][0], side,
                    subdivisionIndices[i], indicesInSubdivisions[i], nextIndex + vertexIndex));
            for (j=0; j!=subdivisionIndices[i].size(); ++j)
            {
                meshIndicesInSubdivisions->at(subdivisionIndices[i][j])[indicesInSubdivisions[i][j]] = nextIndex + vertexIndex;
            }
            vertexMeshIndex[vertexIndex] = nextIndex + vertexIndex;
            ++vertexIndex;
        }
        else
        {
            steinerPoints->push_back(H2MeshSteinerPoint(subdivisionIndices[i][0], indicesInSubdivisions[i][0], side,
                    subdivisionIndices[i], indicesInSubdivisions[i], nextIndex + nbVertices + steinerPointIndex));
            for (j=0; j!=subdivisionIndices[i].size(); ++j)
            {
                meshIndicesInSubdivisions->at(subdivisionIndices[i][j])[indicesInSubdivisions[i][j]] = nextIndex + nbVertices + steinerPointIndex;
            }
            steinerPointsMeshIndex[steinerPointIndex] = nextIndex + nbVertices + steinerPointIndex;
            ++steinerPointIndex;
        }
        ++indexOnSide;
    }
    nextIndex += nbVertices + nbSteinerPoints;
}



void H2MeshConstructor::createInteriorNeighbors()
{
    uint i, j, index, neighborIndex;
    for (i=0; i<nbSubdivisions; i++)
    {
        for (j=0; j<nbSubdivisionPoints; j++)
        {
            if (!boundaryPointInSubdivisions[i][j])
            {
                index = meshIndicesInSubdivisions->at(i)[j];
                (*points)[index]->neighborsIndices.reserve(6);
                for (auto k : neighborsInSubdivisions[i][j])
                {
                    neighborIndex = meshIndicesInSubdivisions->at(i)[k];
                    (*points)[index]->neighborsIndices.push_back(neighborIndex);
                    if (boundaryPointInSubdivisions[i][k])
                    {
                        (*points)[neighborIndex]->neighborsIndices.push_back(index);
                    }
                }
            }
        }
    }
}

void H2MeshConstructor::createCutNeighbors()
{
    uint i, j, index, previousIndex, nextIndex, nbCuts = triangulater.cuts.size();
    uint vertexIndexLeft1, vertexIndexLeft2, vertexIndexRight1, vertexIndexRight2;
    std::vector<uint> indicesLeft;
    TriangulationCut cut ;

    for (i=0; i!=nbCuts; ++i)
    {
        cut = triangulater.cuts[i];
        triangulater.adjacentSidesIndices(i, vertexIndexLeft1, vertexIndexLeft2, vertexIndexRight1, vertexIndexRight2);
        indicesLeft = (*subdivisions)[cut.leftTriangleIndex].sidePointsIndices(vertexIndexLeft1, vertexIndexLeft2);

        index = meshIndicesInSubdivisions->at(cut.leftTriangleIndex)[indicesLeft[0]];
        nextIndex = meshIndicesInSubdivisions->at(cut.leftTriangleIndex)[indicesLeft[1]];

        (*points)[index]->neighborsIndices.push_back(nextIndex);

        for (j=1; j<nbSubdivisionLines-1; j++)
        {
            previousIndex = index;
            index = nextIndex;
            nextIndex = meshIndicesInSubdivisions->at(cut.leftTriangleIndex)[indicesLeft[j+1]];
            (*points)[index]->neighborsIndices.push_back(previousIndex);
            (*points)[index]->neighborsIndices.push_back(nextIndex);
        }

        (*points)[nextIndex]->neighborsIndices.push_back(index);
    }
}

void H2MeshConstructor::createSideNeighbors()
{
    uint previousIndex, index, nextIndex;
    std::vector<uint> indices;

    uint i, j;
    for (i=0; i!=nbVertices+nbSteinerPoints; ++i)
    {
        indices = meshPointsIndicesAlongSide(i);

        index = indices[0];
        nextIndex = indices[1];
        (*points)[index]->neighborsIndices.push_back(nextIndex);

        for (j=1; j+1!=nbSubdivisionLines; ++j)
        {
            previousIndex = index;
            index = nextIndex;
            nextIndex = indices[j+1];
            (*points)[index]->neighborsIndices.push_back(previousIndex);
            (*points)[index]->neighborsIndices.push_back(nextIndex);
        }

        (*points)[nextIndex]->neighborsIndices.push_back(index);
    }
}

void H2MeshConstructor::createRemainingNonExteriorNeighbors()
{
    if (depth != 0)
    {
        uint index1, index2;
        for (uint i=0; i != nbSubdivisions; ++i)
        {
            index1 = meshIndicesInSubdivisions->at(i)[1];
            index2 = meshIndicesInSubdivisions->at(i)[2];
            (*points)[index1]->neighborsIndices.push_back(index2);
            (*points)[index2]->neighborsIndices.push_back(index1);

            index1 = meshIndicesInSubdivisions->at(i)[((nbSubdivisionLines-1)*(nbSubdivisionLines-2))/2];
            index2 = meshIndicesInSubdivisions->at(i)[(nbSubdivisionLines*(nbSubdivisionLines-1))/2 + 1];
            (*points)[index1]->neighborsIndices.push_back(index2);
            (*points)[index2]->neighborsIndices.push_back(index1);

            index1 = meshIndicesInSubdivisions->at(i)[nbSubdivisionPoints - nbSubdivisionLines - 1];
            index2 = meshIndicesInSubdivisions->at(i)[nbSubdivisionPoints - 2];
            (*points)[index1]->neighborsIndices.push_back(index2);
            (*points)[index2]->neighborsIndices.push_back(index1);
        }
    }
}

void H2MeshConstructor::createExteriorNeighbors()
{
    bool jumpNext = false;
    uint side=0, k;
    std::vector<uint> indices1, indices2, neighbors1, neighbors2;
    H2MeshBoundaryPoint *m1, *m2;
    H2MeshSteinerPoint *p1, *p2;
    H2MeshPoint *q1, *q2;
    Word id;
    while(side < nbVertices)
    {
        indices1 = meshPointsIndicesAlongFullSide(side);
        indices2 = meshPointsIndicesAlongFullSide(side + 2);

        for (k=1; k+1!=indices1.size(); ++k)
        {
            q1 = (*points)[indices1[k]];
            q2 = (*points)[indices2[indices1.size()-1-k]];
            if (q1->isBoundaryPoint() && q2->isBoundaryPoint())
            {
                m1 = static_cast<H2MeshBoundaryPoint*> (q1);
                m2 = static_cast<H2MeshBoundaryPoint*> (q2);
                m1->partnerPointIndex = indices2[indices1.size()-1-k];
                m2->partnerPointIndex = indices1[k];

                neighbors1 = m1->neighborsIndices;
                neighbors2 = m2->neighborsIndices;

                m1->neighborsPairings.resize(neighbors1.size());
                std::fill(m1->neighborsPairings.begin(),m1->neighborsPairings.end(),id);
                m2->neighborsPairings.resize(neighbors2.size());
                std::fill(m2->neighborsPairings.begin(),m2->neighborsPairings.end(),id);

                for (auto neighbor : m1->neighborsIndices)
                {
                    if ((!((*points)[neighbor]->isVertexPoint())) &&
                            ( (!(*points)[neighbor]->isBoundaryPoint()) || ((*points)[neighbor]->isBoundaryPoint() &&((static_cast<H2MeshBoundaryPoint*>((*points)[neighbor]))->side != side)) ) &&
                            ( (!(*points)[neighbor]->isSteinerPoint()) || ((*points)[neighbor]->isSteinerPoint() && ((static_cast<H2MeshSteinerPoint*>((*points)[neighbor]))->side != side)) ))
                    {
                        neighbors2.push_back(neighbor);
                        m2->neighborsPairings.push_back(sidePairings[side]);
                    }
                }

                for (auto neighbor : m2->neighborsIndices)
                {
                    if ((!((*points)[neighbor]->isVertexPoint())) &&
                            ( (!(*points)[neighbor]->isBoundaryPoint()) || ((*points)[neighbor]->isBoundaryPoint() &&((static_cast<H2MeshBoundaryPoint*>((*points)[neighbor]))->side != side+2)) ) &&
                            ( (!(*points)[neighbor]->isSteinerPoint()) || ((*points)[neighbor]->isSteinerPoint() && ((static_cast<H2MeshSteinerPoint*>((*points)[neighbor]))->side != side+2)) ))
                    {
                        neighbors1.push_back(neighbor);
                        m1->neighborsPairings.push_back(sidePairings[side+2]);
                    }
                }

                m1->neighborsIndices = neighbors1;
                m2->neighborsIndices = neighbors2;
            }
            else if (q1->isSteinerPoint() && q2->isSteinerPoint())
            {
                p1 = static_cast<H2MeshSteinerPoint*>(q1);
                p2 = static_cast<H2MeshSteinerPoint*>(q2);
                p1->partnerPointIndex = indices2[indices1.size()-1-k];
                p2->partnerPointIndex = indices1[k];

                neighbors1 = p1->neighborsIndices;
                neighbors2 = p2->neighborsIndices;

                p1->neighborsPairings.resize(neighbors1.size());
                std::fill(p1->neighborsPairings.begin(),p1->neighborsPairings.end(),id);
                p2->neighborsPairings.resize(neighbors2.size());
                std::fill(p2->neighborsPairings.begin(),p2->neighborsPairings.end(),id);

                for (auto neighbor : p1->neighborsIndices)
                {
                    if ((!((*points)[neighbor]->isVertexPoint())) &&
                            ( (!(*points)[neighbor]->isBoundaryPoint()) || ((*points)[neighbor]->isBoundaryPoint() && ((static_cast<H2MeshBoundaryPoint*> ((*points)[neighbor]))->side != side)) ) &&
                            ( (!(*points)[neighbor]->isSteinerPoint()) || ((*points)[neighbor]->isSteinerPoint() && ((static_cast<H2MeshSteinerPoint*> ((*points)[neighbor]))->side != side)) ))
                    {
                        neighbors2.push_back(neighbor);
                        p2->neighborsPairings.push_back(sidePairings[side]);
                    }
                }

                for (auto neighbor : p2->neighborsIndices)
                {
                    if ((!((*points)[neighbor]->isVertexPoint())) &&
                            ( (!(*points)[neighbor]->isBoundaryPoint()) || ((*points)[neighbor]->isBoundaryPoint() && ((static_cast<H2MeshBoundaryPoint*> ((*points)[neighbor]))->side != side+2)) ) &&
                            ( (!(*points)[neighbor]->isSteinerPoint()) || ((*points)[neighbor]->isSteinerPoint() && ((static_cast<H2MeshSteinerPoint*> ((*points)[neighbor]))->side != side+2)) ))
                    {
                        neighbors1.push_back(neighbor);
                        p1->neighborsPairings.push_back(sidePairings[side+2]);
                    }
                }

                p1->neighborsIndices = neighbors1;
                p2->neighborsIndices = neighbors2;
            }
            else
            {
                throw(QString("A BoundaryPoint is paired with a SteinerPoint or something"));
            }
        }
        side += 1 + jumpNext*2;
        jumpNext = !jumpNext;
    }
}

void H2MeshConstructor::reorganizeNeighbors()
{
    std::vector<std::tuple< H2Point, H2Point, uint> > triples;
    uint index,i;
    std::vector<uint> indicesOld, indicesNew;

    for (auto & cutpoint : *cutPoints)
    {
        i=0;
        index = meshIndicesInSubdivisions->at(cutpoint.subdivisionIndex)[cutpoint.indexInSubdivision];
        indicesOld = cutpoint.neighborsIndices;
        for (const auto & neighbor : mesh->getH2Neighbors(index))
        {
            triples.push_back(std::tuple<H2Point, H2Point, int>(mesh->getH2Point(index), neighbor, i));
            ++i;
        }
        std::sort(triples.begin(), triples.end(), compareTriples);
        for (const auto & triple : triples)
        {
            indicesNew.push_back(indicesOld[std::get<2>(triple)]);
        }
        cutpoint.neighborsIndices = indicesNew;
        triples.clear();
        indicesNew.clear();
    }


    std::vector<Word> neighborsPairingsOld, neighborsPairingsNew;
    for (auto & boundarypoint : *boundaryPoints)
    {
        i=0;
        index = (meshIndicesInSubdivisions->at(boundarypoint.subdivisionIndex))[boundarypoint.indexInSubdivision];
        indicesOld = boundarypoint.neighborsIndices;
        neighborsPairingsOld = boundarypoint.neighborsPairings;
        for (const auto & neighbor : mesh->getKickedH2Neighbors(index))
        {
            triples.push_back(std::tuple<H2Point, H2Point, int>(mesh->getH2Point(index), neighbor, i));
            ++i;
        }
        std::sort(triples.begin(), triples.end(), compareTriples);
        for (const auto & triple : triples)
        {
            indicesNew.push_back(indicesOld[std::get<2>(triple)]);
            neighborsPairingsNew.push_back(neighborsPairingsOld[std::get<2>(triple)]);
        }
        boundarypoint.neighborsIndices = indicesNew;
        boundarypoint.neighborsPairings = neighborsPairingsNew;
        triples.clear();
        indicesNew.clear();
        neighborsPairingsNew.clear();
    }


    for (auto & vertexpoint : *vertexPoints)
    {
        i=0;
        index = (meshIndicesInSubdivisions->at(vertexpoint.subdivisionIndex))[vertexpoint.indexInSubdivision];
        indicesOld = vertexpoint.neighborsIndices;
        neighborsPairingsOld = vertexpoint.neighborsPairings;
        for (const auto & neighbor : mesh->getKickedH2Neighbors(index))
        {
            triples.push_back(std::tuple<H2Point, H2Point, int>(mesh->getH2Point(index), neighbor, i));
            ++i;
        }
        std::sort(triples.begin(), triples.end(), compareTriples);
        for (const auto & triple : triples)
        {
            indicesNew.push_back(indicesOld[std::get<2>(triple)]);
            neighborsPairingsNew.push_back(neighborsPairingsOld[std::get<2>(triple)]);
        }
        vertexpoint.neighborsIndices = indicesNew;
        vertexpoint.neighborsPairings = neighborsPairingsNew;
        triples.clear();
        indicesNew.clear();
        neighborsPairingsNew.clear();
    }


    for (auto & steinerpoint : *steinerPoints)
    {
        i=0;
        index = (meshIndicesInSubdivisions->at(steinerpoint.subdivisionIndex))[steinerpoint.indexInSubdivision];
        indicesOld = steinerpoint.neighborsIndices;
        neighborsPairingsOld = steinerpoint.neighborsPairings;
        for (const auto & neighbor : mesh->getKickedH2Neighbors(index))
        {
            triples.push_back(std::tuple<H2Point, H2Point, uint>(mesh->getH2Point(index), neighbor, i));
            ++i;
        }
        std::sort(triples.begin() ,triples.end(), compareTriples);
        for (const auto & triple : triples)
        {
            indicesNew.push_back(indicesOld[std::get<2>(triple)]);
            neighborsPairingsNew.push_back(neighborsPairingsOld[std::get<2>(triple)]);
        }


        steinerpoint.neighborsIndices = indicesNew;
        steinerpoint.neighborsPairings = neighborsPairingsNew;
        triples.clear();
        indicesNew.clear();
        neighborsPairingsNew.clear();
    }
}

void H2MeshConstructor::createPiecewiseAffineWeights()
{
    uint i=0;
    H2Point basept;
    std::vector<H2Point> neighbors;
    for(const auto & meshPoint : mesh->meshPoints)
    {
        neighbors = mesh->getKickedH2Neighbors(i);
        basept = mesh->getH2Point(i);
        basept.computeAffineWeights(neighbors,meshPoint->neighborsWeights);
        meshPoint->weight = 0.0;
        ++i;
    }
}

void H2MeshConstructor::createNaiveWeights()
{
    uint i=0;
    H2Point basept;
    std::vector<H2Point> neighbors;
    for(const auto & meshPoint : mesh->meshPoints)
    {
        neighbors = mesh->getKickedH2Neighbors(i);
        basept = mesh->getH2Point(i);
        basept.computeNaiveWeights(neighbors,meshPoint->neighborsWeights);
        meshPoint->weight = 0.0;
        ++i;
    }
}

void H2MeshConstructor::createQuadraticWeights()
{
    uint i=0;
    H2Point basept;
    std::vector<H2Point> neighbors;
    for(const auto & meshPoint : mesh->meshPoints)
    {
        neighbors = mesh->getKickedH2Neighbors(i);
        basept = mesh->getH2Point(i);
        if (meshPoint->isVertexPoint())
        {
            basept.computeAffineWeights(neighbors,meshPoint->neighborsWeights);
        }
        else
        {
            basept.computeQuadraticWeights(neighbors,meshPoint->neighborsWeights);
        }
        meshPoint->weight = 0.0;
        ++i;
    }
}

std::vector<uint> H2MeshConstructor::meshPointsIndicesAlongSide(uint side) const
{
    uint triangleIndex = triangulater.sideTrianglesIndices[side];
    uint triangleSideIndex = triangulater.sideTrianglesBoundarySideIndices[side];
    std::vector<uint> indicesInTriangulation, res;

    if (triangleSideIndex == 2)
    {
        indicesInTriangulation = (*subdivisions)[triangleIndex].sidePointsIndices(2, 0);
    }
    else
    {
        indicesInTriangulation = (*subdivisions)[triangleIndex].sidePointsIndices(triangleSideIndex, triangleSideIndex + 1);
    }

    res.resize(indicesInTriangulation.size());

    std::vector<uint>::size_type i=0;
    for (i=0; i!=indicesInTriangulation.size(); ++i)
    {
        res[i] = (meshIndicesInSubdivisions->at(triangleIndex))[indicesInTriangulation[i]];
    }

    return res;
}

std::vector<uint> H2MeshConstructor::meshPointsIndicesAlongFullSide(uint side) const
{
    std::vector<uint> output;
    std::vector<uint> temp;
    uint index = mesh->fundamentalSteinerDomain.getIndexOfFullVertex(side);
    for (uint j=index; j != index + mesh->fundamentalSteinerDomain.getNbSteinerPointsOnSide(side) + 1; ++j)
    {
        temp = meshPointsIndicesAlongSide(j);
        temp.pop_back();
        output.insert(output.end(),temp.begin(),temp.end());
    }
    if (side+1 < nbVertices)
    {
        output.push_back(vertexMeshIndex[side+1]);
    }
    else
    {
        output.push_back(vertexMeshIndex[0]);
    }

    return output;
}

bool H2MeshConstructor::checkNumberOfMeshPoints() const
{
    uint expected = 0, nbPoints = points->size();
    expected += nbSubdivisions * (nbSubdivisionPoints - 3*(nbSubdivisionLines -1));
    expected += triangulater.cuts.size() * (nbSubdivisionLines - 2);
    expected += (nbVertices+nbSteinerPoints) * (nbSubdivisionLines - 1);

    bool out = (expected == nbPoints);

    if (!out)
    {
        QString errorMessage = QString("ERROR in H2MeshConstructor::checkNumberOfMeshPoints: test failed (expected: %1, found: %2)")
                .arg(QString::number(expected))
                .arg(QString::number(points->size()));
        throw(errorMessage);
    }
    return out;
}

bool H2MeshConstructor::checkForDuplicateNeighbors() const
{
    if(depth > 1)
    {
        for (const auto &m : *points)
        {
            if (Tools::containsDuplicates(m->neighborsIndices))
            {
                throw(QString("ERROR in H2MeshConstructor::checkForDuplicateNeighbors: test failed"));
                return false;
            }
        }
    }
    return true;
}


bool H2MeshConstructor::checkNumberOfNeighbors() const
{
    uint failed = 0;
    for (const auto &m : *points)
    {
        if (!m->isVertexPoint() && !m->isSteinerPoint())
        {
            if (m->neighborsIndices.size() != 6)
            {
                std::stringstream errorMessage;
                errorMessage << "ERROR in H2MeshConstructor::checkNumberOfNeighbors: failed ("
                             << "point has " << m->neighborsIndices.size() << " neighbors)" << std::endl
                             << m->neighborsIndices << std::endl;
                throw(QString::fromStdString(errorMessage.str()));
                ++failed;
            }
        }
    }
    bool out = (failed == 0);

    if (!out)
    {
        throw(QString("checkNumberOfNeighbors(): test failed"));
    }

    return out;
}

void H2MeshConstructor::createExteriorVertexNeighbors()
{
    std::vector<Word> wordSidePairings = mesh->rho.getDiscreteGroup().getSidePairingsClosedSurface();

    uint i, genus = nbVertices/4;
    std::vector<Word> neighborsWordIsometriesTemp;
    Word f;
    std::vector<uint> neighborsTemp;
    for (i=0; i<genus; ++i)
    {
        // choppe les voisins de 4i
        for(auto k: (*points)[vertexMeshIndex[4*i]]->neighborsIndices)
        {
            if(((*points)[k]->isBoundaryPoint() && (static_cast<H2MeshBoundaryPoint*>((*points)[k])->side == 4*i))
                    || ((*points)[k]->isSteinerPoint() && (static_cast<H2MeshSteinerPoint*>((*points)[k])->side == 4*i))
                    || ((!(*points)[k]->isSteinerPoint()) && (!(*points)[k]->isBoundaryPoint())))
            {
                neighborsTemp.push_back(k);
                neighborsWordIsometriesTemp.push_back(f);
            }
        }

        f = f*wordSidePairings[4*i].inverse();
        // choppe les voisins de sommet[4i+3]
        
        for(auto k: (*points)[vertexMeshIndex[4*i+3]]->neighborsIndices)
        {
            if(((*points)[k]->isBoundaryPoint() && (static_cast<H2MeshBoundaryPoint*>((*points)[k])->side == 4*i+3))
                    || ((*points)[k]->isSteinerPoint() && (static_cast<H2MeshSteinerPoint*>((*points)[k])->side == 4*i+3))
                    || ((!(*points)[k]->isSteinerPoint()) && (!(*points)[k]->isBoundaryPoint())))
            {
                neighborsTemp.push_back(k);
                neighborsWordIsometriesTemp.push_back(f);
            }
        }
        f = f*wordSidePairings[4*i+3].inverse();
        // choppe les voisins de sommet[4i+2]
        
        for(auto k: (*points)[vertexMeshIndex[4*i+2]]->neighborsIndices)
        {
            if(((*points)[k]->isBoundaryPoint() && (static_cast<H2MeshBoundaryPoint*>((*points)[k])->side == 4*i+2))
                    || ((*points)[k]->isSteinerPoint() && (static_cast<H2MeshSteinerPoint*>((*points)[k])->side == 4*i+2))
                    || ((!(*points)[k]->isSteinerPoint()) && (!(*points)[k]->isBoundaryPoint())))
            {
                neighborsTemp.push_back(k);
                neighborsWordIsometriesTemp.push_back(f);
            }
        }
        f= f*wordSidePairings[4*i+2].inverse();
        
        // choppe les voisins de 4i+1
        for(auto k: (*points)[vertexMeshIndex[4*i+1]]->neighborsIndices)
        {
            if(((*points)[k]->isBoundaryPoint() && (static_cast<H2MeshBoundaryPoint*>((*points)[k])->side == 4*i+1))
                    || ((*points)[k]->isSteinerPoint() && (static_cast<H2MeshSteinerPoint*>((*points)[k])->side == 4*i+1))
                    || ((!(*points)[k]->isSteinerPoint()) && (!(*points)[k]->isBoundaryPoint())))
            {
                neighborsTemp.push_back(k);
                neighborsWordIsometriesTemp.push_back(f);
            }
        }
        f = f*wordSidePairings[4*i+1].inverse();
    }


    for (i=0; i<genus; ++i)
    {

        (*points)[vertexMeshIndex[4*i]]->neighborsIndices = neighborsTemp;
        neighborsWordIsometriesTemp = Word::contract(neighborsWordIsometriesTemp);
        (static_cast<H2MeshVertexPoint *> ((*points)[vertexMeshIndex[4*i]]))->neighborsPairings = neighborsWordIsometriesTemp;

        (*points)[vertexMeshIndex[4*i+3]]->neighborsIndices = neighborsTemp;
        neighborsWordIsometriesTemp = wordSidePairings[4*i]*neighborsWordIsometriesTemp;
        neighborsWordIsometriesTemp = Word::contract(neighborsWordIsometriesTemp);
        (static_cast<H2MeshVertexPoint *> ((*points)[vertexMeshIndex[4*i+3]]))->neighborsPairings = neighborsWordIsometriesTemp;

        (*points)[vertexMeshIndex[4*i+2]]->neighborsIndices = neighborsTemp;
        neighborsWordIsometriesTemp = wordSidePairings[4*i+3]*neighborsWordIsometriesTemp;
        neighborsWordIsometriesTemp = Word::contract(neighborsWordIsometriesTemp);
        (static_cast<H2MeshVertexPoint *> ((*points)[vertexMeshIndex[4*i+2]]))->neighborsPairings = neighborsWordIsometriesTemp;

        (*points)[vertexMeshIndex[4*i+1]]->neighborsIndices = neighborsTemp;
        neighborsWordIsometriesTemp = wordSidePairings[4*i+2]*neighborsWordIsometriesTemp;
        neighborsWordIsometriesTemp = Word::contract(neighborsWordIsometriesTemp);
        (static_cast<H2MeshVertexPoint *> ((*points)[vertexMeshIndex[4*i+1]]))->neighborsPairings = neighborsWordIsometriesTemp;

        neighborsWordIsometriesTemp = wordSidePairings[4*i+1]*neighborsWordIsometriesTemp;
    }
}

void H2MeshConstructor::createMeshExteriorIndices()
{
    mesh->exteriorSidesIndices.clear();
    std::vector<uint> indicesAlongSide;
    for (uint i=0; i != nbVertices; ++i)
    {
        indicesAlongSide = meshPointsIndicesAlongFullSide(i);
        mesh->exteriorSidesIndices.insert(mesh->exteriorSidesIndices.end(), indicesAlongSide.begin(), indicesAlongSide.end());
        mesh->exteriorSidesIndices.pop_back();
    }
}

bool H2MeshConstructor::runTests() const
{
    bool b1 = checkNumberOfMeshPoints();
    bool b2 = checkForDuplicateNeighbors();
    bool b4 = checkNumberOfNeighbors();
    bool b5 = checkPartnerPoints();
    bool b6 = checkCurrentIndex();
    return b1 && b2 && b4 && b5 && b6;
}

bool H2MeshConstructor::checkCurrentIndex() const
{
    bool out = (nextIndex==(mesh->meshPoints).size());
    if (!out)
    {
        throw(QString("H2MeshConstructor::checkCurrentIndex(): test failed"));
    }
    return out;
}

bool H2MeshConstructor::checkPartnerPoints() const
{
    H2Point p1, p2;
    H2MeshPoint *m;
    double d, max = 0;
    uint passed = 0, failed = 0;
    uint nbMeshPoints = points->size();
    for (uint i=0; i!=nbMeshPoints; ++i)
    {
        m = (*points)[i];
        if (m->isBoundaryPoint())
        {
            p1 = mesh->rho.evaluateRepresentation(sidePairings[(static_cast<H2MeshBoundaryPoint*>(m))->side])*(mesh->getH2Point(i));
            p2 = mesh->getH2Point((static_cast<H2MeshBoundaryPoint*>(m))->partnerPointIndex);
            d = H2Point::distance(p1, p2);
            max = d > max ? d : max;
            if (d > 0.01)
            {
                std::stringstream errorMessage;
                errorMessage << "ERROR in H2MeshConstructor::checkForPartnerPoints(): test failed";
                errorMessage << "For point #" << i << ", partner point is supposed to be #" << (static_cast<H2MeshBoundaryPoint*>(m))->partnerPointIndex;
                errorMessage << "Following points are supposed to be the same: p1 = " << p1 << " and p2 = " << p2;
                throw(QString::fromStdString(errorMessage.str()));
                ++failed;
            }
            else
            {
                ++passed;
            }
        }
    }

    bool out = (failed == 0);
    if (!out)
    {
        throw(QString("H2MeshConstructor::checkForPartnerPoints(): failed (max error = %1)").arg(QString::number(max)));
    }
    return out;
}

bool H2MeshConstructor::compareTriples(const std::tuple<H2Point, H2Point, uint> &t1, const std::tuple<H2Point, H2Point, uint> &t2)
{
    // Assert that std::get<0>(t1) and std::get<0>(t2) are the same.
    H2Point p(std::get<0>(t1)), p1(std::get<1>(t1)), p2(std::get<1>(t2));
    return p.compareAngles(p1, p2);
}
