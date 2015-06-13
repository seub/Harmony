#include "equivariantharmonicmapsfactory.h"

#include "fenchelnielsenconstructor.h"
#include "h2trianglesubdivision.h"

EquivariantHarmonicMapsFactory::EquivariantHarmonicMapsFactory() : function(&mesh, &rhoTarget), iterator(&function)
{
    reset();
}

void EquivariantHarmonicMapsFactory::reset()
{
    isGenusSet = false;
    isRhoDomainSet = false;
    isRhoTargetSet = false;
    isMeshDepthSet = false;

    rhoDomain = IsomH2Representation(&Gamma);
    rhoTarget = IsomH2Representation(&Gamma);
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
}

void EquivariantHarmonicMapsFactory::setNiceRhoTarget()
{
    if (genus != 2)
    {
        std::cout << "Error in EquivariantHarmonicMapsFactory::setNiceRhoTarget(): genus needs to be 2" << std::endl;
    }
    rhoTarget.setNiceRepresentation();
    isRhoTargetSet = true;
}

void EquivariantHarmonicMapsFactory::setMeshDepth(int meshDepth)
{
    this->meshDepth = meshDepth;
    isMeshDepthSet = true;
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
        std::cout << "Error in EquivariantHarmonicMapsFactory::initialize: Factory not ready to run" << std::endl;
    }
    mesh = H2Mesh(rhoDomain, meshDepth);
    function.initializePLsmart();
}
