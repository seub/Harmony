#include "liftedgraph.h"

#include "h2mesh.h"


void LiftedGraph::cloneCopyAssignImpl(const LiftedGraph *other)
{
    Gamma = other->Gamma;
    nbBoundaryPoints = other->nbBoundaryPoints;
    nbPoints = other->nbPoints;
    neighborsIndices = other->neighborsIndices;
    neighborsWeights = other->neighborsWeights;
    boundaryPointsNeighborsPairings = other->boundaryPointsNeighborsPairings;
    boundaryPointsPartnersIndices = other->boundaryPointsPartnersIndices;
}

void LiftedGraph::cloneCopyAssign(const LiftedGraph *other)
{
    cloneCopyAssignImpl(other);
}


LiftedGraph::LiftedGraph(const LiftedGraph &other)
{
    Gamma = other.Gamma;
    nbBoundaryPoints = other.nbBoundaryPoints;
    nbPoints = other.nbPoints;
    neighborsIndices = other.neighborsIndices;
    neighborsWeights = other.neighborsWeights;
    boundaryPointsNeighborsPairings = other.boundaryPointsNeighborsPairings;
    boundaryPointsPartnersIndices = other.boundaryPointsPartnersIndices;
}

LiftedGraph* LiftedGraph::cloneCopyConstructImpl() const
{
    return new LiftedGraph(*this);
}

std::unique_ptr<LiftedGraph> LiftedGraph::cloneCopyConstruct() const
{
    return std::unique_ptr<LiftedGraph>(cloneCopyConstructImpl());
}



uint LiftedGraph::getNbPoints() const
{
    return nbPoints;
}

uint LiftedGraph::getNbBoundaryPoints() const
{
    return nbBoundaryPoints;
}

bool LiftedGraph::isBoundaryPoint(uint index) const
{
    assert(index < nbPoints);
    return (index < nbBoundaryPoints);
}

void LiftedGraph::resetNeighborsWeights(std::vector<std::vector<double> > &newNeighborsWeights)
{
    assert(newNeighborsWeights.size() == nbPoints);
    assert(neighborsWeights.size() == newNeighborsWeights.size());

    for (uint i=0; i!=nbPoints; ++i)
    {
        neighborsWeights[i] = newNeighborsWeights[i];
    }
}



template <typename Point, typename Map>
void LiftedGraphFunction<Point, Map>::cloneCopyAssignImpl(const LiftedGraph *other)
{
    LiftedGraph::cloneCopyAssignImpl(other);
    const LiftedGraphFunction<Point, Map> *otherCast = static_cast<const LiftedGraphFunction<Point, Map>*>(other);
    rho = otherCast->rho;
    boundaryPointsNeighborsPairingsValues = otherCast->boundaryPointsNeighborsPairingsValues;
    values = otherCast->values;
}

template <typename Point, typename Map>
void LiftedGraphFunction<Point, Map>::cloneCopyAssign(const LiftedGraphFunction<Point, Map> *other)
{
    cloneCopyAssignImpl(other);
}



template <typename Point, typename Map>
LiftedGraphFunction<Point, Map>::LiftedGraphFunction(const LiftedGraphFunction<Point, Map> &other) : LiftedGraph(other)
{
    rho = other.rho;
    boundaryPointsNeighborsPairingsValues = other.boundaryPointsNeighborsPairingsValues;
    values = other.values;
}

template <typename Point, typename Map>
std::unique_ptr< LiftedGraphFunction<Point, Map> > LiftedGraphFunction<Point, Map>::cloneCopyConstruct() const
{
    return std::unique_ptr<LiftedGraphFunction<Point, Map> >(static_cast<LiftedGraphFunction<Point, Map>*>(cloneCopyConstructImpl()));
}

template <typename Point, typename Map>
LiftedGraph* LiftedGraphFunction<Point, Map>::cloneCopyConstructImpl() const
{
    return new LiftedGraphFunction(*this);
}



template <typename Point, typename Map>
GroupRepresentation<Map> LiftedGraphFunction<Point, Map>::getRepresentation() const
{
    return rho;
}


template <typename Point, typename Map>
Point LiftedGraphFunction<Point, Map>::getValue(uint index) const
{
    return values.at(index);
}

template <typename Point, typename Map>
std::vector<Point> LiftedGraphFunction<Point, Map>::getValues() const
{
    return values;
}

template <typename Point, typename Map>
std::vector<Point> LiftedGraphFunction<Point, Map>::getNeighborsValues(uint index) const
{
    std::vector<Point> out;
    for (auto neighborIndex : this->neighborsIndices.at(index))
    {
        out.push_back(values.at(neighborIndex));
    }
    return out;
}

template <typename Point, typename Map>
std::vector<Point> LiftedGraphFunction<Point, Map>::getNeighborsValuesKicked(uint index) const
{    
    std::vector<Point> out;
    out.reserve(this->neighborsIndices.at(index).size());
    if (this->isBoundaryPoint(index))
    {
        uint i=0;
        for (auto neighborIndex : this->neighborsIndices.at(index))
        {
            out.push_back(boundaryPointsNeighborsPairingsValues.at(index).at(i)*values.at(neighborIndex));
            ++i;
        }
    }
    else
    {
        for (auto neighborIndex : this->neighborsIndices.at(index))
        {
            out.push_back(values.at(neighborIndex));
        }
    }
    return out;
}

template <typename Point, typename Map>
std::vector<Point> LiftedGraphFunction<Point, Map>::getPartnersValues(uint index) const
{
    if (isBoundaryPoint(index))
    {
        std::vector<Point> out;
        for (auto partnerIndex : boundaryPointsPartnersIndices.at(index))
        {
            out.push_back(values.at(partnerIndex));
        }
        return out;
    }
    else
    {
        qDebug() << "Warning in LiftedGraphFunction<Point, Map>::getPartnersValues(uint index): interior point";
        return {};
    }
}


template <typename Point, typename Map>
void LiftedGraphFunction<Point, Map>::refreshBoundaryPointsNeighborsPairingsValues()
{
    assert(this->boundaryPointsNeighborsPairings.size() == nbBoundaryPoints);
    assert(this->boundaryPointsNeighborsPairingsValues.size() == nbBoundaryPoints);

    uint i=0;
    for(const auto &pairings : this->boundaryPointsNeighborsPairings)
    {
        this->boundaryPointsNeighborsPairingsValues[i++] = rho.evaluateRepresentation(pairings);
    }
}



template <typename Point, typename Map>
void LiftedGraphFunction<Point, Map>::resetValues(const std::vector<Point> &newValues)
{
    assert(newValues.size() == this->nbPoints);
    values = newValues;
}







template <typename Point, typename Map>
void LiftedGraphFunctionTriangulated<Point, Map>::cloneCopyAssignImpl(const LiftedGraph *other)
{
    LiftedGraphFunction<Point, Map>::cloneCopyAssignImpl(other);

    const LiftedGraphFunctionTriangulated<Point, Map> *otherCast = static_cast<const LiftedGraphFunctionTriangulated<Point, Map>*>(other);
    depth = otherCast->depth;
    subdivisions = otherCast->subdivisions;
    subdivisionsPointsIndicesInValues = otherCast->subdivisionsPointsIndicesInValues;
    triangles = otherCast->triangles;
}

template <typename Point, typename Map>
void LiftedGraphFunctionTriangulated<Point, Map>::cloneCopyAssign(const LiftedGraphFunctionTriangulated<Point, Map> *other)
{
    cloneCopyAssignImpl(other);
}



template <typename Point, typename Map>
LiftedGraphFunctionTriangulated<Point, Map>::LiftedGraphFunctionTriangulated(const LiftedGraphFunctionTriangulated<Point, Map> &other) : LiftedGraphFunction<Point, Map>(other)
{
    depth = other.depth;
    subdivisions = other.subdivisions;
    subdivisionsPointsIndicesInValues = other.subdivisionsPointsIndicesInValues;
    triangles = other.triangles;
}

template <typename Point, typename Map>
std::unique_ptr<LiftedGraphFunctionTriangulated<Point, Map> > LiftedGraphFunctionTriangulated<Point, Map>::cloneCopyConstruct() const
{
    return std::unique_ptr< LiftedGraphFunctionTriangulated<Point, Map> >(static_cast<LiftedGraphFunctionTriangulated<Point, Map>*>(cloneCopyConstructImpl()));
}

template <typename Point, typename Map>
LiftedGraph* LiftedGraphFunctionTriangulated<Point, Map>::cloneCopyConstructImpl() const
{
    return new LiftedGraphFunctionTriangulated<Point, Map>(*this);
}


template <typename Point, typename Map>
void LiftedGraphFunctionTriangulated<Point, Map>::initializePiecewiseLinear(const std::vector<Point> &polygonVerticesValues)
{
    assert(polygonVerticesValues.size()*(TriangularSubdivision<Point>::nbLines(depth)-1) == this->nbBoundaryPoints);

    uint index1, index2, index3;
    for (uint i=0; i!=triangles.size(); ++i)
    {
        triangles[i].getVertices(index1, index2, index3);
        subdivisions[i].initializeSubdivisionByMidpoints(polygonVerticesValues.at(index1), polygonVerticesValues.at(index2), polygonVerticesValues.at(index3));
    }

    refreshValuesFromSubdivisions();
}


template <typename Point, typename Map>
std::vector<Point> LiftedGraphFunctionTriangulated<Point, Map>::getBoundary() const
{
    std::vector<Point> out;
    out.reserve(this->nbBoundaryPoints);

    for (uint i=0; i!=this->nbBoundaryPoints; ++i)
    {
        out.push_back(this->values[i]);
    }
    return out;
}

template <typename Point, typename Map>
std::vector<uint> LiftedGraphFunctionTriangulated<Point, Map>::getSteinerWeights() const
{
    std::vector<uint> weightsOut;
    std::vector<uint> fullVerticesIndices = this->boundaryPointsPartnersIndices.front();
    std::sort(fullVerticesIndices.begin(), fullVerticesIndices.end());

    uint currentVertexIndex = 0, nextVertexIndex;
    for (auto fullVertexIndex : fullVerticesIndices)
    {
        nextVertexIndex = fullVertexIndex;
        weightsOut.push_back((nextVertexIndex-currentVertexIndex)/Tools::exponentiation(2, this->depth) - 1);
        currentVertexIndex = nextVertexIndex;
    }
    weightsOut.push_back((this->nbBoundaryPoints-currentVertexIndex)/Tools::exponentiation(2, this->depth) - 1);

    return weightsOut;
}

template <typename Point, typename Map>
std::vector<Point> LiftedGraphFunctionTriangulated<Point, Map>::getFirstVertexOrbit() const
{
    std::vector<uint> orbitIndices = this->boundaryPointsPartnersIndices.front();
    orbitIndices.push_back(0);
    std::sort(orbitIndices.begin(), orbitIndices.end());

    std::vector<Point> out;
    out.reserve(orbitIndices.size());
    for (auto index : orbitIndices)
    {
        out.push_back(this->values[index]);
    }
    return out;
}

template <typename Point, typename Map>
double LiftedGraphFunctionTriangulated<Point, Map>::getMinEdgeLengthForRegularTriangulation() const
{
    std::vector<double> bigTrianglesSidelengths;
    bigTrianglesSidelengths.reserve(3*subdivisions.size());

    Point A, B, C;
    for (const auto &subdivision : subdivisions)
    {
        subdivision.getBigTriangle(A, B, C);
        bigTrianglesSidelengths.push_back(Point::distance(A, B));
        bigTrianglesSidelengths.push_back(Point::distance(B, C));
        bigTrianglesSidelengths.push_back(Point::distance(C, A));
    }

    return (*std::min_element(bigTrianglesSidelengths.begin(), bigTrianglesSidelengths.end()))/(Tools::exponentiation(2, depth));
}


template <typename Point, typename Map>
void LiftedGraphFunctionTriangulated<Point, Map>::refreshValuesFromSubdivisions()
{
    uint subdivisionIndex = 0, indexInSubdivision;
    for (auto subdivisionPointsIndicesInValues : subdivisionsPointsIndicesInValues)
    {
        indexInSubdivision = 0;
        for (auto subdivisionPointIndexInValues : subdivisionPointsIndicesInValues)
        {
            this->values.at(subdivisionPointIndexInValues) = subdivisions.at(subdivisionIndex).points->at(indexInSubdivision);
            ++indexInSubdivision;
        }
        ++subdivisionIndex;
    }
}


template <typename Point, typename Map>
void LiftedGraphFunctionTriangulated<Point, Map>::refreshSubdivisionsFromValues()
{
    uint subdivisionIndex = 0, indexInSubdivision;
    for (auto subdivisionPointsIndicesInValues : subdivisionsPointsIndicesInValues)
    {
        indexInSubdivision = 0;
        for (auto subdivisionPointIndexInValues : subdivisionPointsIndicesInValues)
        {
            subdivisions.at(subdivisionIndex).points->at(indexInSubdivision) = this->values.at(subdivisionPointIndexInValues);
            ++indexInSubdivision;
        }
        ++subdivisionIndex;
    }
}

template <typename Point, typename Map>
void LiftedGraphFunctionTriangulated<Point, Map>::resetValues(const std::vector<Point> &newValues)
{
    assert(newValues.size() == this->nbPoints);
    this->values = newValues;
    refreshSubdivisionsFromValues();
}


template <typename Point, typename Map>
std::vector< std::vector<Point> > LiftedGraphFunctionTriangulated<Point, Map>::getTrianglesUp() const
{
    std::vector< std::vector<Point> > out;
    uint aIndex, bIndex, cIndex;
    uint L = TriangularSubdivision<Point>::nbLines(depth);
    uint i, j, m = 0;
    out.reserve((subdivisions.size()*(L-1)*L)/2);


    for (const auto & indices : subdivisionsPointsIndicesInValues)
    {
        m = 0;
        for (i=0; i<L-1; i++)
        {
            m += i;
            for (j=0; j<=i; j++)
            {
                aIndex = indices.at(m + j);
                bIndex = indices.at(m + j + i + 1);
                cIndex = indices.at(m + j + i + 2);
                out.push_back({this->values[aIndex], this->values[bIndex], this->values[cIndex]});
            }
        }
    }

    return out;
}

template <typename Point, typename Map>
std::vector< std::vector<Point> > LiftedGraphFunctionTriangulated<Point, Map>::getAllTriangles() const
{
    std::vector< std::vector<Point> > out;
    uint aIndex, bIndex, cIndex;
    uint L = TriangularSubdivision<Point>::nbLines(depth);
    uint i, j, m = 0;
    out.reserve((subdivisions.size()*(L-1)*L)/2);


    for (const auto & indices : subdivisionsPointsIndicesInValues)
    {
        m = 0;

        aIndex = indices.at(0);
        bIndex = indices.at(1);
        cIndex = indices.at(2);
        out.push_back({this->values[aIndex], this->values[bIndex], this->values[cIndex]});

        for (i=1; i!=L-1; ++i)
        {
            m += i;
            for (j=0; j!=i; ++j)
            {
                aIndex = indices.at(m + j);
                bIndex = indices.at(m + j + i + 1);
                cIndex = indices.at(m + j + i + 2);
                out.push_back({this->values[aIndex], this->values[bIndex], this->values[cIndex]});

                aIndex = indices.at(m + j);
                cIndex = indices.at(m + j + i + 2);
                bIndex = indices.at(m + j + 1);
                out.push_back({this->values[aIndex], this->values[bIndex], this->values[cIndex]});
            }

            aIndex = indices.at(m + j);
            bIndex = indices.at(m + j + i + 1);
            cIndex = indices.at(m + j + i + 2);
            out.push_back({this->values[aIndex], this->values[bIndex], this->values[cIndex]});
        }
    }

    return out;
}

template <>
std::vector<H2Triangle> LiftedGraphFunctionTriangulated<H2Point, H2Isometry>::getH2TrianglesUp() const
{
    std::vector< std::vector<H2Point> > triplesOfPoints = getTrianglesUp();

    std::vector<H2Triangle> out;
    out.reserve(triplesOfPoints.size());

    for (const auto &tripleOfPoints : triplesOfPoints)
    {
        out.push_back(H2Triangle(tripleOfPoints[0], tripleOfPoints[1], tripleOfPoints[2]));
    }
    return out;
}

template <>
std::vector<H2Triangle> LiftedGraphFunctionTriangulated<H2Point, H2Isometry>::getAllH2Triangles() const
{
    std::vector< std::vector<H2Point> > triplesOfPoints = getAllTriangles();

    std::vector<H2Triangle> out;
    out.reserve(triplesOfPoints.size());

    for (const auto &tripleOfPoints : triplesOfPoints)
    {
        out.push_back(H2Triangle(tripleOfPoints[0], tripleOfPoints[1], tripleOfPoints[2]));
    }
    return out;
}



template <>
H2Triangle LiftedGraphFunctionTriangulated<H2Point, H2Isometry>::getH2Triangle(uint index1, uint index2, uint index3) const
{
    return H2Triangle(values.at(index1), values.at(index2), values.at(index3));
}

template <>
bool LiftedGraphFunctionTriangulated<H2Point, H2Isometry>::triangleContaining(const H2Point &point, H2Triangle &triangleOut,
                                                                              uint &index1Out, uint &index2Out, uint &index3Out) const
{
    H2Point A, B, C;
    uint indexInSubdivision1, indexInSubdivision2, indexInSubdivision3, i = 0;
    for(const auto &subdivision : subdivisions)
    {
        if (subdivision.triangleContaining(point, A, B, C, indexInSubdivision1, indexInSubdivision2, indexInSubdivision3))
        {
            index1Out = subdivisionsPointsIndicesInValues[i][indexInSubdivision1];
            index2Out = subdivisionsPointsIndicesInValues[i][indexInSubdivision2];
            index3Out = subdivisionsPointsIndicesInValues[i][indexInSubdivision3];
            triangleOut = H2Triangle(A, B, C);
            return true;
        }
        ++i;
    }
    return false;
}

template <>
void LiftedGraphFunctionTriangulated<H2Point, H2Isometry>::rearrangeOrderForConstructFromH2Mesh(std::vector<uint> newIndices)
{
    std::vector< std::vector<uint> > newNeighborsIndices(nbPoints), newBoundaryPointsPartnersIndices(nbBoundaryPoints), newSubdivisionsPointsIndicesInValues(subdivisions.size());
    std::vector< std::vector<Word> > newBoundaryPointsNeighborsPairings(nbBoundaryPoints);
    std::vector< std::vector<double> > newNeighborsWeights(nbPoints);

    for (uint i=0; i!=nbPoints; ++i)
    {
        for (auto neighborIndex : this->neighborsIndices[i])
        {
            newNeighborsIndices[newIndices[i]].push_back(newIndices[neighborIndex]);
        }
        newNeighborsWeights[newIndices[i]] = this->neighborsWeights[i];
    }

    for (uint i=0; i!=nbBoundaryPoints; ++i)
    {
        newBoundaryPointsNeighborsPairings[newIndices[i + nbPoints - nbBoundaryPoints]] = this->boundaryPointsNeighborsPairings[i];
        for (auto partnerIndex : this->boundaryPointsPartnersIndices[i])
        {
            newBoundaryPointsPartnersIndices[newIndices[i + nbPoints - nbBoundaryPoints]].push_back(newIndices[partnerIndex]);
        }
    }

    for (uint i=0; i!=subdivisions.size(); ++i)
    {
        newSubdivisionsPointsIndicesInValues[i].reserve(subdivisionsPointsIndicesInValues[i].size());
        for (auto index : subdivisionsPointsIndicesInValues[i])
        {
            newSubdivisionsPointsIndicesInValues[i].push_back(newIndices[index]);
        }
    }

    this->neighborsIndices = newNeighborsIndices;
    this->neighborsWeights = newNeighborsWeights;
    this->boundaryPointsPartnersIndices = newBoundaryPointsPartnersIndices;
    this->boundaryPointsNeighborsPairings = newBoundaryPointsNeighborsPairings;
    this->subdivisionsPointsIndicesInValues = newSubdivisionsPointsIndicesInValues;
}

template <>
void LiftedGraphFunctionTriangulated<H2Point, H2Isometry>::constructFromH2Mesh(const H2Mesh &mesh)
{
    //clock_t t0 = clock();

    uint nbInteriorPoints = mesh.regularPoints.size() + mesh.cutPoints.size();
    uint nbBoundaryPoints = mesh.boundaryPoints.size() + mesh.vertexPoints.size() + mesh.steinerPoints.size();
    assert(nbBoundaryPoints == mesh.exteriorSidesIndices.size());
    uint nbPoints = mesh.nbPoints();
    assert(nbInteriorPoints + nbBoundaryPoints == nbPoints);
    this->nbBoundaryPoints = nbBoundaryPoints;
    this->nbPoints = nbPoints;

    this->Gamma = mesh.rho.getDiscreteGroup();


    this->neighborsIndices.clear();
    this->neighborsWeights.clear();
    this->boundaryPointsNeighborsPairings.clear();
    this->boundaryPointsPartnersIndices.clear();

    this->neighborsIndices.reserve(nbPoints);
    this->neighborsWeights.reserve(nbPoints);
    this->boundaryPointsNeighborsPairings.reserve(nbBoundaryPoints);
    this->boundaryPointsPartnersIndices.reserve(nbBoundaryPoints);

    for (const auto & meshPoint : mesh.regularPoints)
    {
        neighborsIndices.push_back(meshPoint.neighborsIndices);
        neighborsWeights.push_back(meshPoint.neighborsWeights);
    }

    for (const auto &meshPoint : mesh.cutPoints)
    {
        neighborsIndices.push_back(meshPoint.neighborsIndices);
        neighborsWeights.push_back(meshPoint.neighborsWeights);
    }

    for (const auto &meshPoint : mesh.boundaryPoints)
    {
        neighborsIndices.push_back(meshPoint.neighborsIndices);
        neighborsWeights.push_back(meshPoint.neighborsWeights);
        this->boundaryPointsNeighborsPairings.push_back(meshPoint.neighborsPairings);
        this->boundaryPointsPartnersIndices.push_back({meshPoint.partnerPointIndex});
    }


    std::vector<uint> verticesIndices;
    verticesIndices.reserve(mesh.vertexPoints.size());
    for (const auto &meshPoint : mesh.vertexPoints)
    {
        verticesIndices.push_back(meshPoint.index);
    }
    assert(verticesIndices.size() != 0);

    std::vector<uint> partnersIndices;
    for (const auto &meshPoint : mesh.vertexPoints)
    {
        neighborsIndices.push_back(meshPoint.neighborsIndices);
        neighborsWeights.push_back(meshPoint.neighborsWeights);
        this->boundaryPointsNeighborsPairings.push_back(meshPoint.neighborsPairings);

        partnersIndices.clear();
        partnersIndices.reserve(verticesIndices.size()-1);
        for (auto index : verticesIndices)
        {
            if (index != meshPoint.index)
            {
                partnersIndices.push_back(index);
            }
        }
        this->boundaryPointsPartnersIndices.push_back(partnersIndices);
    }

    for (const auto &meshPoint : mesh.steinerPoints)
    {
        neighborsIndices.push_back(meshPoint.neighborsIndices);
        neighborsWeights.push_back(meshPoint.neighborsWeights);
        this->boundaryPointsNeighborsPairings.push_back(meshPoint.neighborsPairings);
        this->boundaryPointsPartnersIndices.push_back({meshPoint.partnerPointIndex});
    }

    this->rho = mesh.rho;
    this->depth = mesh.depth;
    this->subdivisions = mesh.subdivisions;
    this->triangles = mesh.triangles;
    this->subdivisionsPointsIndicesInValues = mesh.meshIndicesInSubdivisions;


    //clock_t a = clock();
    std::vector<uint> newIndices(nbPoints);
    std::vector<uint> boundaryPointsIndices = mesh.exteriorSidesIndices;
    assert(mesh.exteriorSidesIndices.size() == nbBoundaryPoints);
    for (uint i=0; i!=nbBoundaryPoints; ++i)
    {
        assert(boundaryPointsIndices[i] >= nbInteriorPoints);
        newIndices[boundaryPointsIndices[i]] = i;
    }
    for (uint i=0; i!=nbInteriorPoints; ++i)
    {
        newIndices[i]= i + nbBoundaryPoints;
    }
    rearrangeOrderForConstructFromH2Mesh(newIndices);
    //clock_t b = clock();


    this->boundaryPointsNeighborsPairingsValues.resize(nbBoundaryPoints);
    this->values.resize(nbPoints);
    this->refreshBoundaryPointsNeighborsPairingsValues();
    refreshValuesFromSubdivisions();

    //clock_t t1 = clock();
    //std::cout << "Time to generate graph from mesh: " << 0.001*int(1000*(t1 - t0)*1.0/CLOCKS_PER_SEC)
    //          << "s (including "<< 0.001*int(1000*(b - a)*1.0/CLOCKS_PER_SEC) << "s to permute points), out of ";
}

template <>
LiftedGraphFunctionTriangulated<H2Point, H2Isometry>::LiftedGraphFunctionTriangulated(const GroupRepresentation<H2Isometry> &rhoDomain, uint depth)
{
    //clock_t t0 = clock();
    constructFromH2Mesh(H2Mesh(rhoDomain, depth));
    //clock_t t1 = clock();
    //std::cout << 0.001*int(1000*(t1 - t0)*1.0/CLOCKS_PER_SEC) << "s total time including construction of mesh" << std::endl;
}

template <typename Point, typename Map>
LiftedGraphFunctionTriangulated<Point, Map>::LiftedGraphFunctionTriangulated(const LiftedGraphFunctionTriangulated<H2Point, H2Isometry> &domainFunction,
                                                                             const GroupRepresentation<Map> &rhoImage)
{
    this->Gamma = domainFunction.Gamma;
    this->nbBoundaryPoints = domainFunction.nbBoundaryPoints;
    this->nbPoints = domainFunction.nbPoints;
    this->neighborsIndices = domainFunction.neighborsIndices;
    this->neighborsWeights = domainFunction.neighborsWeights;
    this->boundaryPointsNeighborsPairings = domainFunction.boundaryPointsNeighborsPairings;
    this->boundaryPointsPartnersIndices = domainFunction.boundaryPointsPartnersIndices;
    this->rho = rhoImage;
    this->boundaryPointsNeighborsPairingsValues.resize(this->nbBoundaryPoints);
    this->refreshBoundaryPointsNeighborsPairingsValues();
    this->depth = domainFunction.depth;
    this->triangles = domainFunction.triangles;
    this->subdivisionsPointsIndicesInValues = domainFunction.subdivisionsPointsIndicesInValues;

    subdivisions.clear();
    for (uint i=0; i!=triangles.size(); ++i)
    {
        subdivisions.push_back(TriangularSubdivision<Point>(depth));
    }

    std::vector<Point> vertexImages = rhoImage.generateFundamentalDomainOptimization().getVertices();
    //std::vector<Point> vertexImages = rhoImage.generateFundamentalDomainOptimization().getVertices();
    H2SteinerPolygon SteinerPolygonImage(vertexImages, getSteinerWeights());
    std::vector<Point> polygonVerticesValues = SteinerPolygonImage.getFullPolygon().getVertices();
    this->values.resize(this->nbPoints);
    initializePiecewiseLinear(polygonVerticesValues);
}



template <typename Point, typename Map>
LiftedGraphFunctionTriangulated<Point, Map>::LiftedGraphFunctionTriangulated(const GroupRepresentation<H2Isometry> &rhoDomain,
                                                                             const GroupRepresentation<Map> &rhoImage, uint depth) :
    LiftedGraphFunctionTriangulated(LiftedGraphFunctionTriangulated(rhoDomain, depth), rhoImage)
{
}




template class LiftedGraphFunction<H2Point, H2Isometry>;
template class LiftedGraphFunctionTriangulated<H2Point, H2Isometry>;
