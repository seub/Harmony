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

    runTests();
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
    createRemainingNeighbors();
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

void H2MeshConstructor::createRemainingNeighbors()
{
    if (depth != 0)
    {
        int index1, index2;
        for (int i=0; i != nbSubdivisions; ++i)
        {
            index1 = (*(*subdivisions)[i].meshIndices)[1];
            index2 = (*(*subdivisions)[i].meshIndices)[2];
            (*meshPoints)[index1].neighborsIndices.push_back(index2);
            (*meshPoints)[index2].neighborsIndices.push_back(index1);

            index1 = (*(*subdivisions)[i].meshIndices)[((nbSubdivisionLines-1)*(nbSubdivisionLines-2))/2];
            index2 = (*(*subdivisions)[i].meshIndices)[(nbSubdivisionLines*(nbSubdivisionLines-1))/2 + 1];
            (*meshPoints)[index1].neighborsIndices.push_back(index2);
            (*meshPoints)[index2].neighborsIndices.push_back(index1);

            index1 = (*(*subdivisions)[i].meshIndices)[nbSubdivisionPoints - nbSubdivisionLines - 1];
            index2 = (*(*subdivisions)[i].meshIndices)[nbSubdivisionPoints - 2];
            (*meshPoints)[index1].neighborsIndices.push_back(index2);
            (*meshPoints)[index2].neighborsIndices.push_back(index1);
        }
    }
    return;
}

void H2MeshConstructor::createExteriorNeighbors()
{
    return;
}

bool H2MeshConstructor::checkForNumberOfMeshPoints() const
{
    int expected = 0, nbPoints = meshPoints->size();
    expected += nbSubdivisions * (nbSubdivisionPoints - 3*(nbSubdivisionLines -1));
    expected += triangulater.cuts.size() * (nbSubdivisionLines - 2);
    expected += nbVertices * (nbSubdivisionLines - 1);
    if (expected != nbPoints)
    {
        std::cout << "ERROR in H2MeshConstructor::checkForNumberOfMeshPoints: test failed (expected: " << expected
                  << " , found: " << meshPoints->size() << ")" << std::endl;
        return false;
    }
    else
    {
        std::cout << "H2MeshConstructor::checkForNumberOfMeshPoints: passed" << std::endl;
        return true;
    }
}

bool H2MeshConstructor::checkForDuplicateNeighbors() const
{
    for (const auto &m : *meshPoints)
    {
        if (Tools::containsDuplicates(m.neighborsIndices))
        {
            std::cout << "ERROR in H2MeshConstructor::checkForDuplicateNeighbors: test failed" << std::endl;
            return false;
        }
    }

    std::cout << "H2MeshConstructor::checkForDuplicateNeighbors: passed" << std::endl;
    return true;
}

bool H2MeshConstructor::checkForNeighborsReciprocity() const
{
    int i, N=meshPoints->size();
    for (i=0; i<N; ++i)
    {
        for (auto k : (*meshPoints)[i].neighborsIndices)
        {
            if (!Tools::isInList(i, (*meshPoints)[k].neighborsIndices))
            {
                std::cout << "ERROR in H2MeshConstructor::checkForNeighborsReciprocity: test failed for indices "
                          << i << " and " << k << std::endl;
                return false;
            }
        }
    }

    std::cout << "H2MeshConstructor::checkForNeighborsReciprocity: passed" << std::endl;
    return true;
}

bool H2MeshConstructor::checkForNumberOfNeighbors() const
{
    std::vector<int> nbCutsFromVertex = triangulater.nbCutsFromVertex();
    unsigned int expected;

    for (const auto &m : *meshPoints)
    {
        if (m.cutPoint)
        {
            if (m.vertexPoint)
            {
                expected = 2 + nbCutsFromVertex[m.vertexIndex];

            }
            else if (m.boundaryPoint)
            {
                std::cout << "ERROR in H2MeshConstructor::checkNumberOfNeighbors: not supposed to happen" << std::endl;
                return false;
            }
            else
            {
                expected = 6;
            }
        }
        else if (m.vertexPoint)
        {
            expected = 2;
        }
        else if (m.boundaryPoint)
        {
            expected = 4;
        }
        else
        {
            expected = 6;
        }

        if (m.neighborsIndices.size() != expected)
        {
            std::cout << "ERROR in H2MeshConstructor::checkNumberOfNeighbors: test failed" << std::endl;
            return false;
        }
    }

    std::cout << "H2MeshConstructor::checkNumberOfNeighbors: passed" << std::endl;
    return true;
}

bool H2MeshConstructor::runTests() const
{
    return checkForNumberOfMeshPoints() && checkForDuplicateNeighbors() && checkForNeighborsReciprocity() && checkForDuplicateNeighbors();
}

