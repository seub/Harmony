#include "equivariantharmonicmapsfactory.h"

#include "fenchelnielsenconstructor.h"
#include "h2meshconstructor.h"

EquivariantHarmonicMapsFactory::EquivariantHarmonicMapsFactory() : functionInit(&mesh, rhoTarget), iterator(&function)
{
    reset();
}

void EquivariantHarmonicMapsFactory::reset()
{
    resetBooleans();

    rhoDomain = GroupRepresentation<H2Isometry>(Gamma);
    rhoTarget = GroupRepresentation<H2Isometry>(Gamma);

    function = functionInit;
    stop = false;
}

void EquivariantHarmonicMapsFactory::resetBooleans()
{
    isGenusSet = false;
    isRhoDomainSet = false;
    isRhoTargetSet = false;
    isMeshDepthSet = false;
    isInitialized = false;
    isMeshInitialized = false;
}

void EquivariantHarmonicMapsFactory::setGenus(uint genus)
{
    this->genus = genus;
    isGenusSet = true;
    Gamma = DiscreteGroup(TopologicalSurface(genus, 0));
    isRhoDomainSet = false;
    isRhoTargetSet = false;
    isInitialized = false;
    isMeshInitialized = false;
}

void EquivariantHarmonicMapsFactory::setNiceRhoDomain()
{
    if (!isGenusSet)
    {
        throw(QString("Error in EquivariantHarmonicMapsFactory::setNiceRhoDomain(): genus not set"));
    }
    if (genus != 2)
    {
        throw(QString("Error in EquivariantHarmonicMapsFactory::setNiceRhoDomain(): genus needs to be 2"));
    }
    rhoDomain.setNiceRepresentation();
    isRhoDomainSet = true;
    initializeRhoDomain();
}

void EquivariantHarmonicMapsFactory::initializeRhoDomain()
{

    if (!(isGenusSet && isRhoDomainSet))
    {
        throw(QString("Error in EquivariantHarmonicMapsFactory::initializeRhoDomain(): not ready to initialize rho domain"));
    }
    isInitialized = false;
    isMeshInitialized = false;
    if (isMeshDepthSet)
    {
        initializeMesh();
    }

}

void EquivariantHarmonicMapsFactory::initializeRhoTarget()
{
    if (!(isGenusSet && isRhoTargetSet))
    {
        throw(QString("Error in EquivariantHarmonicMapsFactory::initializeRhoTarget(): not ready to initialize rho target"));
    }

    isInitialized = false;
    functionInit.rhoImage = rhoTarget;


    if (isMeshInitialized)
    {
        initializeFunction();
    }
}

void EquivariantHarmonicMapsFactory::setNiceRhoTarget()
{
    if (!(isGenusSet))
    {
        throw(QString("Error in EquivariantHarmonicMapsFactory::setNiceRhoTarget(): genus not set"));
    }
    if (genus != 2)
    {
        throw(QString("Error in EquivariantHarmonicMapsFactory::setNiceRhoTarget(): genus needs to be 2"));
    }
    rhoTarget.setNiceRepresentation();

    isRhoTargetSet = true;
    initializeRhoTarget();
}

void EquivariantHarmonicMapsFactory::setMeshDepth(uint meshDepth)
{
    this->meshDepth = meshDepth;
    isMeshDepthSet = true;
    isInitialized = false;
    isMeshInitialized = false;
    if (isGenusSet && isRhoDomainSet)
    {
        initializeMesh();
    }
}

void EquivariantHarmonicMapsFactory::setRhoDomain(const std::vector<double> &FNLengths, const std::vector<double> FNTwists)
{


    if (!isGenusSet)
    {
        throw(QString("Error in EquivariantHarmonicMapsFactory::setRhoDomain(): genus has not been set"));
    }
    else if ((3*genus != (3+FNLengths.size())) || (3*genus != (3+FNTwists.size())))
    {
        throw(QString("Error in EquivariantHarmonicMapsFactory::setRhoDomain(): genus does not match number of FN coordinates"));
    }
    FNLengthsDomain = FNLengths;
    FNTwistsDomain = FNTwists;

    FenchelNielsenConstructor FN(FNLengths, FNTwists);
    rhoDomain = FN.getRepresentation();


    isRhoDomainSet = true;

    initializeRhoDomain();
}

void EquivariantHarmonicMapsFactory::setRhoTarget(const std::vector<double> &FNLengths, const std::vector<double> FNTwists)
{
    if (!isGenusSet)
    {
        throw(QString("Error in EquivariantHarmonicMapsFactory::setRhoTarget(): genus has not been set"));
    }
    else if ((3*genus != (3+FNLengths.size())) || (3*genus != (3+FNTwists.size())))
    {
        throw(QString("Error in EquivariantHarmonicMapsFactory::setRhoTarget(): genus does not match number of FN coordinates"));
    }
    FNLengthsTarget = FNLengths;
    FNTwistsTarget = FNTwists;
    FenchelNielsenConstructor FN(FNLengths, FNTwists);
    rhoTarget = FN.getRepresentation();

    isRhoTargetSet = true;
    initializeRhoTarget();
}

void EquivariantHarmonicMapsFactory::resetRhoDomain()
{
    isRhoDomainSet = false;
    isMeshInitialized = false;
    isInitialized = false;
}

void EquivariantHarmonicMapsFactory::resetRhoTarget()
{
    isRhoTargetSet = false;
    isInitialized = false;
}

GroupRepresentation<H2Isometry> EquivariantHarmonicMapsFactory::getRhoDomain() const
{
    return rhoDomain;
}

GroupRepresentation<H2Isometry> EquivariantHarmonicMapsFactory::getRhoTarget() const
{
    return rhoTarget;
}

void EquivariantHarmonicMapsFactory::initializeFunction()
{
    if (!(isGenusSet && isMeshDepthSet && isRhoDomainSet && isRhoTargetSet && isMeshInitialized))
    {
        throw(QString("Error in EquivariantHarmonicMapsFactory::initializeFunction: Factory not ready to initializeFunction"));
    }

    functionInit.initializePLsmart();
    function = functionInit;

    iterator.initialize();
    isInitialized = true;
}

void EquivariantHarmonicMapsFactory::initializeMesh()
{
    if (!(isGenusSet && isRhoDomainSet && isMeshDepthSet))
    {
        throw(QString("Error in EquivariantHarmonicMapsFactory::initializeMesh(): not ready to initialize mesh"));
    }

    mesh = H2Mesh(rhoDomain, meshDepth);
    emit meshCreated(mesh.nbPoints());

    isMeshInitialized = true;
    if (isRhoTargetSet)
    {
        initializeFunction();
    }
}

bool EquivariantHarmonicMapsFactory::isReady() const
{
    return isGenusSet && isRhoDomainSet && isRhoTargetSet && isMeshDepthSet && isInitialized && isMeshInitialized;
}

void EquivariantHarmonicMapsFactory::resetInit()
{
    if (!(isGenusSet && isMeshDepthSet && isRhoDomainSet && isRhoTargetSet && isMeshInitialized))
    {
        throw(QString("Error in EquivariantHarmonicMapsFactory::resetInit: Factory not ready to reset Init"));
    }
    function = functionInit;

    iterator.initialize();
    isInitialized = true;
}

void EquivariantHarmonicMapsFactory::refreshFunction()
{
    iterator.getOutput(function);
}

void EquivariantHarmonicMapsFactory::iterate(uint n)
{
    if (!isReady())
    {
        throw(QString("Error in void EquivariantHarmonicMapsFactory::iterate(): not ready"));
    }
    iterator.iterate(n);
    refreshFunction();
}

void EquivariantHarmonicMapsFactory::run()
{
    nbIterations = 0;
    while(!stop)
    {
        iterator.iterate();
        ++nbIterations;
    }
    refreshFunction();
}

void EquivariantHarmonicMapsFactory::stopRunning()
{
    stop = true;
}
