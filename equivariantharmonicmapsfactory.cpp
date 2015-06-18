#include "equivariantharmonicmapsfactory.h"

#include "fenchelnielsenconstructor.h"
#include "h2trianglesubdivision.h"
#include "h2meshconstructor.h"
#include "h2trianglesubdivision.h"

EquivariantHarmonicMapsFactory::EquivariantHarmonicMapsFactory() : functionInit(&mesh, rhoTarget), iterator(&function)
{
    reset();
}

void EquivariantHarmonicMapsFactory::reset()
{
    resetBooleans();

    rhoDomain = IsomH2Representation(&Gamma);
    rhoTarget = IsomH2Representation(&Gamma);

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

void EquivariantHarmonicMapsFactory::setGenus(int genus)
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
        std::cout << "Error in EquivariantHarmonicMapsFactory::setNiceRhoTarget(): genus needs to be 2" << std::endl;
    }
    rhoTarget.setNiceRepresentation();

    isRhoTargetSet = true;
    initializeRhoTarget();
}

void EquivariantHarmonicMapsFactory::setMeshDepth(int meshDepth)
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
        std::cout << "Error in EquivariantHarmonicMapsFactory::setRhoDomain(): genus has not been set" << std::endl;
    }
    else if ((3*genus != (int) (3+FNLengths.size())) || (3*genus != (int) (3+FNTwists.size())))
    {
        std::cout << "Error in EquivariantHarmonicMapsFactory::setRhoDomain(): genus does not match number of FN coordinates" << std::endl;
    }
    FNLengthsDomain = FNLengths;
    FNTwistsDomain = FNTwists;

    std::cout << "lengths = " << FNLengths << ", twists = " << FNTwists << std::endl;

    FenchelNielsenConstructor FN(FNLengths, FNTwists);
    rhoDomain = FN.getRepresentation(&Gamma);
    isRhoDomainSet = true;

    std::cout << rhoDomain << std::endl;
    initializeRhoDomain();
}

void EquivariantHarmonicMapsFactory::setRhoTarget(const std::vector<double> &FNLengths, const std::vector<double> FNTwists)
{
    if (!isGenusSet)
    {
        std::cout << "Error in EquivariantHarmonicMapsFactory::setRhoTarget(): genus has not been set" << std::endl;
    }
    else if ((3*genus != (int) (3+FNLengths.size())) || (3*genus != (int) (3+FNTwists.size())))
    {
        std::cout << "Error in EquivariantHarmonicMapsFactory::setRhoTarget(): genus does not match number of FN coordinates" << std::endl;
    }
    FNLengthsTarget = FNLengths;
    FNTwistsTarget = FNTwists;
    FenchelNielsenConstructor FN(FNLengths, FNTwists);
    rhoTarget = FN.getRepresentation(&Gamma);

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

IsomH2Representation EquivariantHarmonicMapsFactory::getRhoDomain() const
{
    return rhoDomain;
}

IsomH2Representation EquivariantHarmonicMapsFactory::getRhoTarget() const
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
        std::cout << "Error in EquivariantHarmonicMapsFactory::resetInit: Factory not ready to reset Init" << std::endl;
    }
    function = functionInit;

    iterator.initialize();
    isInitialized = true;
}

void EquivariantHarmonicMapsFactory::refreshFunction()
{
    iterator.getOutput(function);
}

void EquivariantHarmonicMapsFactory::iterate(int n)
{
    if (!isReady())
    {
        std::cout << "Error in void EquivariantHarmonicMapsFactory::iterate(): not ready" << std::endl;
    }
    iterator.iterate(n);
    refreshFunction();
}

void EquivariantHarmonicMapsFactory::run()
{
    while(!stop)
    {
        iterator.iterate();
    }
    refreshFunction();
}

void EquivariantHarmonicMapsFactory::stopRunning()
{
    stop = true;
}
