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
}

void EquivariantHarmonicMapsFactory::setGenus(int genus)
{
    this->genus = genus;
    isGenusSet = true;
    Gamma = DiscreteGroup(TopologicalSurface(genus, 0));
}

void EquivariantHarmonicMapsFactory::setNiceRhoDomain()
{
    if (genus != 2)
    {
        std::cout << "Error in EquivariantHarmonicMapsFactory::setNiceRhoDomain(): genus needs to be 2" << std::endl;
    }
    rhoDomain.setNiceRepresentation();
    isRhoDomainSet = true;
    isInitialized = false;
}

void EquivariantHarmonicMapsFactory::setNiceRhoTarget()
{
    if (genus != 2)
    {
        std::cout << "Error in EquivariantHarmonicMapsFactory::setNiceRhoTarget(): genus needs to be 2" << std::endl;
    }
    rhoTarget.setNiceRepresentation();
    functionInit.rhoImage = rhoTarget;
    isRhoTargetSet = true;
    isInitialized = false;
}

void EquivariantHarmonicMapsFactory::setMeshDepth(int meshDepth)
{
    this->meshDepth = meshDepth;
    isMeshDepthSet = true;
    isInitialized = false;
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
    FenchelNielsenConstructor FN(FNLengths, FNTwists);
    rhoDomain = FN.getRepresentation(&Gamma);
    isRhoDomainSet = true;
    isInitialized = false;
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
    functionInit.rhoImage = rhoTarget;
    isRhoTargetSet = true;
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

void EquivariantHarmonicMapsFactory::initialize()
{
    if (!(isGenusSet && isMeshDepthSet && isRhoDomainSet && isRhoTargetSet))
    {
        std::cout << "Error in EquivariantHarmonicMapsFactory::initialize: Factory not ready to initialize" << std::endl;
    }
    //mesh = H2Mesh(rhoDomain, meshDepth);
    mesh.fundamentalDomain = rhoDomain.generatePolygon(100);
    mesh.rho = rhoDomain;
    mesh.depth = meshDepth;
    H2MeshConstructor(&(this->mesh));

    functionInit.initializePLsmart();
    function = functionInit;

    iterator.initialize();
    isInitialized = true;
}

bool EquivariantHarmonicMapsFactory::isReady() const
{
    return isGenusSet && isRhoDomainSet && isRhoTargetSet && isMeshDepthSet && isInitialized;
}

void EquivariantHarmonicMapsFactory::resetInit()
{
    if (!(isGenusSet && isMeshDepthSet && isRhoDomainSet && isRhoTargetSet))
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
}

void EquivariantHarmonicMapsFactory::stopRunning()
{
    stop = true;
}
