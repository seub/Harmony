#include "h2meshconstructor.h"
#include "h2trianglesubdivision.h"

H2MeshConstructor::H2MeshConstructor(H2Mesh *mesh) :
    mesh(mesh), depth(mesh->depth), sidePairings(&(mesh->sidePairings)),
    subdivisions(&(mesh->subdivisions)), meshPoints(&(mesh->meshPoints)), triangulater(H2PolygonTriangulater(&(mesh->fundamentalDomain)))
{
    *sidePairings = mesh->rho.getSidePairingsForNormalizedFundamentalDomain();

    nbVertices = mesh->fundamentalDomain.nbVertices();
    nbSubdivisions = triangulater.triangles.size();
    nbSubdivisionLines = H2TriangleSubdivision::nbOfLines(depth);
    nbSubdivisionPoints = H2TriangleSubdivision::nbOfPoints(depth);
    nextIndex = 0;

    createSubdivisions();
    createPoints();
    createNeighbors();
}

void H2MeshConstructor::createPoints()
{
    createInteriorMeshPoints();
    createCutMeshPoints();
    createSideMeshPoints();
    return;
}

void H2MeshConstructor::createNeighbors()
{
    createInteriorNeighbors();
    createCutNeighbors();
    createSideNeighbors();
    createExteriorNeighbors();
    return;
}

void H2MeshConstructor::createSubdivisions()
{
    std::vector<H2Triangle> triangles = triangulater.getTriangles();
    for (const auto &T : triangles)
    {
        subdivisions->push_back(H2TriangleSubdivision(T, depth));
    }

    neighborsInSubdivisions.reserve(nbSubdivisions);
    for (const auto &S : *subdivisions)
    {
        neighborsInSubdivisions.push_back(S.neighborsIndices());
    }

    return;
}

void H2MeshConstructor::createInteriorMeshPoints()
{
    boundaryPointInSubdivisions.reserve(nbSubdivisions);
    for (const auto & S : *subdivisions)
    {
        boundaryPointInSubdivisions.push_back(S.areBoundaryPoints());
    }


    meshPoints->reserve(nbSubdivisions*nbSubdivisionPoints);

    int i, j;
    for (i=0; i<nbSubdivisions; ++i)
    {
        for (j=0; j<nbSubdivisionPoints; ++j)
        {
            if (!boundaryPointInSubdivisions[i][j])
            {
                meshPoints->push_back(H2MeshPoint(i, j));

                (*((*subdivisions)[i].meshIndices))[j] = nextIndex;
                ++nextIndex;
            }
        }
    }

    return;
}

void H2MeshConstructor::createCutMeshPoints()
{

    vertexAdded.resize(nbVertices);
    std::fill(vertexAdded.begin(), vertexAdded.end(), false);
    vertexMeshIndex.resize(nbVertices);
    std::fill(vertexMeshIndex.begin(), vertexMeshIndex.end(), -1);

    std::vector<int> indicesLeft, indicesRight;
    int i, j, nbCuts = triangulater.cuts.size();
    int vertexIndexLeft1, vertexIndexLeft2, vertexIndexRight1, vertexIndexRight2;
    TriangulationCut cut ;

    for (i=0; i<nbCuts; ++i)
    {
        cut = triangulater.cuts[i];
        triangulater.adjacentSidesIndices(i, vertexIndexLeft1, vertexIndexLeft2, vertexIndexRight1, vertexIndexRight2);
        indicesLeft = (*subdivisions)[cut.leftTriangleIndex].sidePointsIndices(vertexIndexLeft1, vertexIndexLeft2);
        indicesRight = (*subdivisions)[cut.rightTriangleIndex].sidePointsIndices(vertexIndexRight1, vertexIndexRight2);

        if (!vertexAdded[cut.vertexIndex1])
        {
            meshPoints->push_back(H2MeshPoint(cut.leftTriangleIndex, indicesLeft[0], true, cut.rightTriangleIndex, indicesRight[0], true,
                    cut.vertexIndex1, true, cut.vertexIndex1));
            vertexMeshIndex[cut.vertexIndex1] = nextIndex;
            ++nextIndex;
            vertexAdded[cut.vertexIndex1] = true;
        }
        (*(((*subdivisions)[cut.leftTriangleIndex]).meshIndices))[indicesLeft[0]] = vertexMeshIndex[cut.vertexIndex1];
        (*(((*subdivisions)[cut.rightTriangleIndex]).meshIndices))[indicesRight[0]] = vertexMeshIndex[cut.vertexIndex1];

        for (j=1; j<nbSubdivisionLines-1; j++)
        {
            meshPoints->push_back(H2MeshPoint(cut.leftTriangleIndex, indicesLeft[j], true, cut.rightTriangleIndex, indicesRight[j]));
            (*(((*subdivisions)[cut.leftTriangleIndex]).meshIndices))[indicesLeft[j]] = nextIndex;
            (*(((*subdivisions)[cut.rightTriangleIndex]).meshIndices))[indicesRight[j]] = nextIndex;
            ++nextIndex;
        }

        if (!vertexAdded[cut.vertexIndex2])
        {
            meshPoints->push_back(H2MeshPoint(cut.leftTriangleIndex, indicesLeft.back(), true, cut.rightTriangleIndex, indicesRight.back(), true,
                                              cut.vertexIndex2, true, cut.vertexIndex2));
            vertexMeshIndex[cut.vertexIndex2] = nextIndex;
            ++nextIndex;
            vertexAdded[cut.vertexIndex2] = true;
        }
        (*(((*subdivisions)[cut.leftTriangleIndex]).meshIndices))[indicesLeft.back()] = vertexMeshIndex[cut.vertexIndex2];
        (*(((*subdivisions)[cut.rightTriangleIndex]).meshIndices))[indicesRight.back()] = vertexMeshIndex[cut.vertexIndex2];
    }

    return;
}

void H2MeshConstructor::createSideMeshPoints()
{
    int triangleIndex, triangleSideIndex;
    std::vector<int> indices;

    int i, j, k;
    for (i=0; i<nbVertices; i++)
    {
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

        if (!vertexAdded[i])
        {
            vertexAdded[i] = true;
            (*meshPoints).push_back(H2MeshPoint(triangleIndex, indices[0], false, -1, -1, true, i, true, i));
            vertexMeshIndex[i] = nextIndex;
            ++nextIndex;
        }
        (*(((*subdivisions)[triangleIndex]).meshIndices))[indices[0]] = vertexMeshIndex[i];

        for (j=1; j+1<nbSubdivisionLines; ++j)
        {
            (*meshPoints).push_back(H2MeshPoint(triangleIndex, indices[j], false, -1, -1, true, i));
            (*(((*subdivisions)[triangleIndex]).meshIndices))[indices[j]] = nextIndex;
            ++nextIndex;
        }

        k = (i+1 == nbVertices) ? 0 : i+1;
        if (!vertexAdded[k])
        {
            vertexAdded[k] = true;
            (*meshPoints).push_back(H2MeshPoint(triangleIndex, indices.back(), false, -1, -1, true, k, true, k));
            vertexMeshIndex[k] = nextIndex;
            ++nextIndex;
        }
        (*(((*subdivisions)[triangleIndex]).meshIndices))[indices.back()] = vertexMeshIndex[k];
    }

    return;
}


void H2MeshConstructor::createInteriorNeighbors()
{
    int i, j, index, neighborIndex;
    for (i=0; i<nbSubdivisions; i++)
    {
        for (j=0; j<nbSubdivisionPoints; j++)
        {
            if (!boundaryPointInSubdivisions[i][j])
            {
                index = (*((*subdivisions)[i].meshIndices))[j];
                (*meshPoints)[index].neighborsIndices.reserve(6);
                for (auto k : neighborsInSubdivisions[i][j])
                {
                    neighborIndex = (*((*subdivisions)[i].meshIndices))[k];
                    (*meshPoints)[index].neighborsIndices.push_back(neighborIndex);
                    if (boundaryPointInSubdivisions[i][k])
                    {
                        (*meshPoints)[neighborIndex].neighborsIndices.push_back(index);
                    }
                }
            }
        }
    }

    return;
}

void H2MeshConstructor::createCutNeighbors()
{
    int i, j, index, previousIndex, nextIndex, nbCuts = triangulater.cuts.size();
    int vertexIndexLeft1, vertexIndexLeft2, vertexIndexRight1, vertexIndexRight2;
    std::vector<int> indicesLeft;
    TriangulationCut cut ;

    for (i=0; i<nbCuts; i++)
    {
        cut = triangulater.cuts[i];
        triangulater.adjacentSidesIndices(i, vertexIndexLeft1, vertexIndexLeft2, vertexIndexRight1, vertexIndexRight2);
        indicesLeft = (*subdivisions)[cut.leftTriangleIndex].sidePointsIndices(vertexIndexLeft1, vertexIndexLeft2);

        std::cout << "alice" << std::endl;
        index = (*((*subdivisions)[cut.leftTriangleIndex].meshIndices))[indicesLeft[0]];
        nextIndex = (*((*subdivisions)[cut.leftTriangleIndex].meshIndices))[indicesLeft[1]];

        (*meshPoints)[index].neighborsIndices.push_back(nextIndex);

        for (j=1; j<nbSubdivisionLines-1; j++)
        {
            previousIndex = index;
            index = nextIndex;
            nextIndex = (*((*subdivisions)[cut.leftTriangleIndex].meshIndices))[indicesLeft[j+1]];
            (*meshPoints)[index].neighborsIndices.push_back(previousIndex);
            (*meshPoints)[index].neighborsIndices.push_back(nextIndex);
        }

        (*meshPoints)[nextIndex].neighborsIndices.push_back(index);
    }

    return;
}

void H2MeshConstructor::createSideNeighbors()
{
    int triangleIndex, triangleSideIndex, previousIndex, index, nextIndex;
    std::vector<int> indices;

    int i, j;
    for (i=0; i<nbVertices; i++)
    {
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


        index = (*((*subdivisions)[triangleIndex].meshIndices))[indices[0]];
        nextIndex = (*((*subdivisions)[triangleIndex].meshIndices))[indices[1]];

        (*meshPoints)[index].neighborsIndices.push_back(nextIndex);

        for (j=1; j<nbSubdivisionLines-1; j++)
        {
            previousIndex = index;
            index = nextIndex;
            nextIndex = (*((*subdivisions)[triangleIndex].meshIndices))[indices[j+1]];
            (*meshPoints)[index].neighborsIndices.push_back(previousIndex);
            (*meshPoints)[index].neighborsIndices.push_back(nextIndex);
        }

        (*meshPoints)[nextIndex].neighborsIndices.push_back(index);
    }

    return;
}

void H2MeshConstructor::createExteriorNeighbors()
{
    return;
}





