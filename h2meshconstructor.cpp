#include "h2meshconstructor.h"
#include "h2trianglesubdivision.h"

H2MeshConstructor::H2MeshConstructor(H2Mesh *mesh) :
    mesh(mesh), depth(mesh->depth),
    subdivisions(&(mesh->subdivisions)), points(&(mesh->meshPoints)),
    regularPoints(&(mesh->regularPoints)), cutPoints(&(mesh->cutPoints)),
    boundaryPoints(&(mesh->boundaryPoints)), vertexPoints(&(mesh->vertexPoints)),
    triangulater(H2PolygonTriangulater(&(mesh->fundamentalDomain)))
{

    nbVertices = mesh->fundamentalDomain.nbVertices();
    nbSubdivisions = triangulater.triangles.size();
    nbSubdivisionLines = H2TriangleSubdivision::nbOfLines(depth);
    nbSubdivisionPoints = H2TriangleSubdivision::nbOfPoints(depth);
    nextIndex = 0;
    sidePairings = mesh->rho.getSidePairingsForNormalizedFundamentalDomain();

    createSubdivisions();
    createPoints();
    createNeighbors();

    runTests();
}

void H2MeshConstructor::createPoints()
{
    createRegularPoints();
    createCutPoints();
    createBoundaryPoints();
    createVertexPoints();
    return;
}

void H2MeshConstructor::createNeighbors()
{
    createInteriorNeighbors();

    createCutNeighbors();

    createSideNeighbors();

    createRemainingNeighbors();

    createExteriorNeighbors();

    createExteriorVertexNeighbors();

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

void H2MeshConstructor::createRegularPoints()
{
    boundaryPointInSubdivisions.reserve(nbSubdivisions);
    for (const auto & S : *subdivisions)
    {
        boundaryPointInSubdivisions.push_back(S.areBoundaryPoints());
    }


    points->reserve(nbSubdivisions*nbSubdivisionPoints);
    regularPoints->reserve(nbSubdivisions*(nbSubdivisionPoints - boundaryPointInSubdivisions[0].size()));

    int i, j;
    for (i=0; i!=nbSubdivisions; ++i)
    {
        for (j=0; j!=nbSubdivisionPoints; ++j)
        {
            if (!boundaryPointInSubdivisions[i][j])
            {
                regularPoints->push_back(H2MeshPoint(i, j));

                (*((*subdivisions)[i].meshIndices))[j] = nextIndex;
                ++nextIndex;
            }
        }
    }

    for (auto &point : *regularPoints)
    {
        points->push_back(&point);
    }

    return;
}

void H2MeshConstructor::createCutPoints()
{
    std::vector<int> indicesLeft, indicesRight;
    int i, j, nbCuts = triangulater.cuts.size();
    int vertexIndexLeft1, vertexIndexLeft2, vertexIndexRight1, vertexIndexRight2;
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
            cutPoints->push_back(H2MeshCutPoint(cut.leftTriangleIndex, indicesLeft[j], cut.rightTriangleIndex, indicesRight[j]));
            (*(((*subdivisions)[cut.leftTriangleIndex]).meshIndices))[indicesLeft[j]] = nextIndex;
            (*(((*subdivisions)[cut.rightTriangleIndex]).meshIndices))[indicesRight[j]] = nextIndex;
            ++nextIndex;
        }
    }

    for (auto &point : *cutPoints)
    {
        points->push_back(&point);
    }

    return;
}

void H2MeshConstructor::createBoundaryPoints()
{
    boundaryPoints->reserve(nbVertices*(nbSubdivisionLines-1));

    int triangleIndex, triangleSideIndex;
    std::vector<int> indices;

    int i, j;
    for (i=0; i!=nbVertices; ++i)
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

        for (j=1; j+1!=nbSubdivisionLines; ++j)
        {
            boundaryPoints->push_back(H2MeshBoundaryPoint(triangleIndex, indices[j], i));
            (*(((*subdivisions)[triangleIndex]).meshIndices))[indices[j]] = nextIndex;
            ++nextIndex;
        }
    }

    for (auto &point : *boundaryPoints)
    {
        points->push_back(&point);
    }

    return;
}

void H2MeshConstructor::createVertexPoints()
{
    vertexPoints->reserve(nbVertices);
    vertexMeshIndex.resize(nbVertices);

    std::vector< std::vector<int> > subdivisionIndices, indicesInTriangles, indicesInSubdivisions;
    triangulater.verticesIndices(subdivisionIndices, indicesInTriangles);
    int index2 = nbSubdivisionPoints-1, index1 = index2 - nbSubdivisionLines + 1;

    indicesInSubdivisions.resize(nbVertices);
    int i, indexInTriangle;
    unsigned int j;
    for (i=0; i!=nbVertices; ++i)
    {
        for (j=0; j!=indicesInTriangles[i].size(); ++j)
        {
            indexInTriangle = indicesInTriangles[i][j];
            indicesInSubdivisions[i].push_back((indexInTriangle==1)*index1 + (indexInTriangle==2)*index2);
        }
    }

    for (i=0; i!=nbVertices; ++i)
    {
        vertexPoints->push_back(H2MeshVertexPoint(subdivisionIndices[i][0], indicesInSubdivisions[i][0], i,
                subdivisionIndices[i], indicesInSubdivisions[i]));
        vertexMeshIndex[i] = nextIndex;

        for (j=0; j!=subdivisionIndices[i].size(); ++j)
        {
            (*(((*subdivisions)[subdivisionIndices[i][j]]).meshIndices))[indicesInSubdivisions[i][j]] = nextIndex;
        }

        ++nextIndex;
    }

    for (auto &point : *vertexPoints)
    {
        points->push_back(&point);
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
                (*points)[index]->neighborsIndices.reserve(6);
                for (auto k : neighborsInSubdivisions[i][j])
                {
                    neighborIndex = (*((*subdivisions)[i].meshIndices))[k];
                    (*points)[index]->neighborsIndices.push_back(neighborIndex);
                    if (boundaryPointInSubdivisions[i][k])
                    {
                        (*points)[neighborIndex]->neighborsIndices.push_back(index);
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

        (*points)[index]->neighborsIndices.push_back(nextIndex);

        for (j=1; j<nbSubdivisionLines-1; j++)
        {
            previousIndex = index;
            index = nextIndex;
            nextIndex = (*((*subdivisions)[cut.leftTriangleIndex].meshIndices))[indicesLeft[j+1]];
            (*points)[index]->neighborsIndices.push_back(previousIndex);
            (*points)[index]->neighborsIndices.push_back(nextIndex);
        }

        (*points)[nextIndex]->neighborsIndices.push_back(index);
    }

    return;
}

void H2MeshConstructor::createSideNeighbors()
{
    int previousIndex, index, nextIndex;
    std::vector<int> indices;

    int i, j;
    for (i=0; i<nbVertices; i++)
    {
        indices = meshPointsIndicesAlongSide(i);

        index = indices[0];
        nextIndex = indices[1];
        (*points)[index]->neighborsIndices.push_back(nextIndex);

        for (j=1; j<nbSubdivisionLines-1; j++)
        {
            previousIndex = index;
            index = nextIndex;
            nextIndex = indices[j+1];
            (*points)[index]->neighborsIndices.push_back(previousIndex);
            (*points)[index]->neighborsIndices.push_back(nextIndex);
        }

        (*points)[nextIndex]->neighborsIndices.push_back(index);
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
            (*points)[index1]->neighborsIndices.push_back(index2);
            (*points)[index2]->neighborsIndices.push_back(index1);

            index1 = (*(*subdivisions)[i].meshIndices)[((nbSubdivisionLines-1)*(nbSubdivisionLines-2))/2];
            index2 = (*(*subdivisions)[i].meshIndices)[(nbSubdivisionLines*(nbSubdivisionLines-1))/2 + 1];
            (*points)[index1]->neighborsIndices.push_back(index2);
            (*points)[index2]->neighborsIndices.push_back(index1);

            index1 = (*(*subdivisions)[i].meshIndices)[nbSubdivisionPoints - nbSubdivisionLines - 1];
            index2 = (*(*subdivisions)[i].meshIndices)[nbSubdivisionPoints - 2];
            (*points)[index1]->neighborsIndices.push_back(index2);
            (*points)[index2]->neighborsIndices.push_back(index1);
        }
    }
    return;
}

void H2MeshConstructor::createExteriorNeighbors()
{
    bool jumpNext = false;
    int side=0, k;
    std::vector<int> indices1, indices2, neighbors1, neighbors2;
    H2MeshBoundaryPoint *m1, *m2;
    H2Isometry id = H2Isometry::identity();
    while(side < nbVertices)
    {
        indices1 = meshPointsIndicesAlongSide(side);
        indices2 = meshPointsIndicesAlongSide(side + 2);

        for (k=1; k<nbSubdivisionLines - 1; ++k)
        {
            m1 = (H2MeshBoundaryPoint *)(*points)[indices1[k]];
            m2 = (H2MeshBoundaryPoint *)(*points)[indices2[nbSubdivisionLines-1-k]];
            m1->partnerPointIndex = indices2[nbSubdivisionLines-1-k];
            m2->partnerPointIndex = indices1[k];
            if(indices1[k]==33)
            {
                k = k;
            }
            neighbors1 = m1->neighborsIndices;
            neighbors2 = m2->neighborsIndices;
            
            m1->neighborsIsometries.resize(neighbors1.size());
            std::fill(m1->neighborsIsometries.begin(),m1->neighborsIsometries.end(),id);
            m2->neighborsIsometries.resize(neighbors2.size());
            std::fill(m2->neighborsIsometries.begin(),m2->neighborsIsometries.end(),id);
            
            for (auto neighbor : m1->neighborsIndices)
            {
                if (!((*points)[neighbor]->isVertexPoint()) && (!(*points)[neighbor]->isBoundaryPoint() ||
                        ((H2MeshBoundaryPoint *) (*points)[neighbor])->side != side ))
                {
                    neighbors2.push_back(neighbor);
                    m2->neighborsIsometries.push_back(sidePairings[side]);
                }
            }

            for (auto neighbor : m2->neighborsIndices)
            {
                if (!((*points)[neighbor]->isVertexPoint()) && (!(*points)[neighbor]->isBoundaryPoint() ||
                         ((H2MeshBoundaryPoint *)(*points)[neighbor])->side != side + 2 ))
                {
                    neighbors1.push_back(neighbor);
                    m1->neighborsIsometries.push_back(sidePairings[side+2]);
                }
            }
            
            m1->neighborsIndices = neighbors1;
            m2->neighborsIndices = neighbors2;
        }


        side += 1 + jumpNext*2;
        jumpNext = !jumpNext;
    }

    return;
}

std::vector<int> H2MeshConstructor::meshPointsIndicesAlongSide(int side) const
{
    int triangleIndex = triangulater.sideTrianglesIndices[side];
    int triangleSideIndex = triangulater.sideTrianglesBoundarySideIndices[side];
    std::vector<int> indicesInTriangulation, res;

    if (triangleSideIndex == 2)
    {
        indicesInTriangulation = (*subdivisions)[triangleIndex].sidePointsIndices(2, 0);
    }
    else
    {
        indicesInTriangulation = (*subdivisions)[triangleIndex].sidePointsIndices(triangleSideIndex, triangleSideIndex + 1);
    }

    res.resize(indicesInTriangulation.size());

    std::vector<int>::size_type i=0;
    for (i=0; i!=indicesInTriangulation.size(); ++i)
    {
        res[i] = (*((*subdivisions)[triangleIndex].meshIndices))[indicesInTriangulation[i]];
    }

    return res;
}

bool H2MeshConstructor::checkNumberOfMeshPoints() const
{
    int expected = 0, nbPoints = points->size();
    expected += nbSubdivisions * (nbSubdivisionPoints - 3*(nbSubdivisionLines -1));
    expected += triangulater.cuts.size() * (nbSubdivisionLines - 2);
    expected += nbVertices * (nbSubdivisionLines - 1);
    if (expected != nbPoints)
    {
        std::cout << "ERROR in H2MeshConstructor::checkNumberOfMeshPoints: test failed (expected: " << expected
                  << " , found: " << points->size() << ")" << std::endl;
        return false;
    }
    else
    {
        std::cout << "H2MeshConstructor::checkNumberOfMeshPoints: passed" << std::endl;
        return true;
    }
}

bool H2MeshConstructor::checkForDuplicateNeighbors() const
{
    if(depth > 1)
    {
    for (const auto &m : *points)
    {
        if (Tools::containsDuplicates(m->neighborsIndices))
        {
            std::cout << "ERROR in H2MeshConstructor::checkForDuplicateNeighbors: test failed" << std::endl;
            return false;
        }
    }
    }
    std::cout << "H2MeshConstructor::checkForDuplicateNeighbors: passed" << std::endl;
    return true;

}


bool H2MeshConstructor::checkNumberOfNeighbors() const
{
    int failed = 0;
    for (const auto &m : *points)
    {
        if (!m->isVertexPoint())
        {
            if (m->neighborsIndices.size() != 6)
            {
                std::cout << "ERROR in H2MeshConstructor::checkNumberOfNeighbors: failed ("
                          << "point has " << m->neighborsIndices.size() << " neighbors)" << std::endl;
                std::cout << m->neighborsIndices << std::endl;
                ++failed;
            }
        }
    }

    if (failed==0)
    {
        std::cout << "H2MeshConstructor::checkNumberOfNeighbors: passed" << std::endl;
    }
    return failed == 0;
}

void H2MeshConstructor::createExteriorVertexNeighbors()
{
    std::vector<Word> wordSidePairings = mesh->rho.getWordSidePairings();

    int i, genus = nbVertices/4;
    std::vector<Word> neighborsWordIsometriesTemp;
    Word f;
    std::vector<int> neighborsTemp;
    for (i=0; i<genus; ++i)
    {
        // choppe les voisins de 4i
        for(auto k: (*points)[vertexMeshIndex[4*i]]->neighborsIndices)
        {
            if(!(*points)[k]->isBoundaryPoint() || ((H2MeshBoundaryPoint *) ((*points)[k]))->side == 4*i)
            {
                neighborsTemp.push_back(k);
                neighborsWordIsometriesTemp.push_back(f);
            }
        }

        f = f*wordSidePairings[4*i].inverse();
        // choppe les voisins de sommet[4i+3]
        
        for(auto k: (*points)[vertexMeshIndex[4*i+3]]->neighborsIndices)
        {
            if(!(*points)[k]->isBoundaryPoint() || ((H2MeshBoundaryPoint *) ((*points)[k]))->side == 4*i+3)
            {
                neighborsTemp.push_back(k);
                neighborsWordIsometriesTemp.push_back(f);
            }
        }
        f = f*wordSidePairings[4*i+3].inverse();
        // choppe les voisins de sommet[4i+2]
        
        for(auto k: (*points)[vertexMeshIndex[4*i+2]]->neighborsIndices)
        {
            if(!(*points)[k]->isBoundaryPoint() || ((H2MeshBoundaryPoint *) ((*points)[k]))->side == 4*i+2)
            {
                neighborsTemp.push_back(k);
                neighborsWordIsometriesTemp.push_back(f);
            }
        }
        f= f*wordSidePairings[4*i+2].inverse();
        
        // choppe les voisins de 4i+1
        for(auto k: (*points)[vertexMeshIndex[4*i+1]]->neighborsIndices)
        {
            if(!(*points)[k]->isBoundaryPoint() || ((H2MeshBoundaryPoint *) ((*points)[k]))->side == 4*i+1)
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
        ((H2MeshVertexPoint *) ((*points)[vertexMeshIndex[4*i]]))->neighborsIsometries = mesh->rho.evaluateRepresentation(neighborsWordIsometriesTemp);

        (*points)[vertexMeshIndex[4*i+3]]->neighborsIndices = neighborsTemp;
        neighborsWordIsometriesTemp = wordSidePairings[4*i]*neighborsWordIsometriesTemp;
        neighborsWordIsometriesTemp = Word::contract(neighborsWordIsometriesTemp);
        ((H2MeshVertexPoint *) ((*points)[vertexMeshIndex[4*i+3]]))->neighborsIsometries = mesh->rho.evaluateRepresentation(neighborsWordIsometriesTemp);

        (*points)[vertexMeshIndex[4*i+2]]->neighborsIndices = neighborsTemp;
        neighborsWordIsometriesTemp = wordSidePairings[4*i+3]*neighborsWordIsometriesTemp;
        neighborsWordIsometriesTemp = Word::contract(neighborsWordIsometriesTemp);
        ((H2MeshVertexPoint *) ((*points)[vertexMeshIndex[4*i+2]]))->neighborsIsometries = mesh->rho.evaluateRepresentation(neighborsWordIsometriesTemp);

        (*points)[vertexMeshIndex[4*i+1]]->neighborsIndices = neighborsTemp;
        neighborsWordIsometriesTemp = wordSidePairings[4*i+2]*neighborsWordIsometriesTemp;
        neighborsWordIsometriesTemp = Word::contract(neighborsWordIsometriesTemp);
        ((H2MeshVertexPoint *) ((*points)[vertexMeshIndex[4*i+1]]))->neighborsIsometries = mesh->rho.evaluateRepresentation(neighborsWordIsometriesTemp);

        neighborsWordIsometriesTemp = wordSidePairings[4*i+1]*neighborsWordIsometriesTemp;
    }
    return;
}

bool H2MeshConstructor::runTests() const
{
    bool b1 = checkNumberOfMeshPoints();
    bool b2 = checkForDuplicateNeighbors();
    bool b4 = checkNumberOfNeighbors();
    bool b5 = checkPartnerPoints();
    return b1 && b2 && b4 && b5;
}

bool H2MeshConstructor::checkPartnerPoints() const
{
    H2Point p1, p2;
    H2MeshPoint *m;
    double d, max = 0;
    int passed = 0, failed = 0;
    int nbMeshPoints = points->size();
    for (int i=0; i<nbMeshPoints; ++i)
    {
        m = (*points)[i];
        if (m->isBoundaryPoint())
        {
            p1 = sidePairings[((H2MeshBoundaryPoint*) m)->side]*(mesh->getH2Point(i));
            p2 = mesh->getH2Point(((H2MeshBoundaryPoint*) m)->partnerPointIndex);
            d = H2Point::distance(p1, p2);
            max = d > max ? d : max;
            if (d > 0.01)
            {
                /*std::cout << "ERROR in H2MeshConstructor::checkForPartnerPoints(): test failed" << std::endl;
                std::cout << "For point #" << i << ", partner point is supposed to be #" << ((H2MeshBoundaryPoint*) m)->partnerPointIndex
                          << std::endl;
                std::cout << "Following points are supposed to be the same: p1 = " << p1 << " and p2 = " << p2 << std::endl;*/
                ++failed;
            }
            else
            {
                ++passed;
            }
        }
    }

    if (failed ==0)
    {
        std::cout << "H2MeshConstructor::checkForPartnerPoints(): passed (max error = " << max << ")" << std::endl;
    }
    else
    {
        std::cout << "H2MeshConstructor::checkForPartnerPoints(): failed (max error = " << max << ")" << std::endl;
    }
    return failed == 0;
}
