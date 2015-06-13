#ifndef EQUIVARIANTHARMONICMAPSFACTORY_H
#define EQUIVARIANTHARMONICMAPSFACTORY_H

#include "tools.h"
#include "discretegroup.h"
#include "grouprepresentation.h"
#include "h2meshfunctioniterator.h"
#include "h2meshfunction.h"


class EquivariantHarmonicMapsFactory
{
    friend class Window;

public:
    EquivariantHarmonicMapsFactory();
    void setGenus(int genus);
    void setMeshDepth(int meshDepth);
    void setRhoDomain(const std::vector<double> & FNlengths, const std::vector<double> FNtwists);
    void setRhoTarget(const std::vector<double> & FNlengths, const std::vector<double> FNtwists);
    void setNiceRhoDomain();
    void setNiceRhoTarget();

    IsomH2Representation getRhoDomain() const;
    IsomH2Representation getRhoTarget() const;

    void reset();

    void initialize();

private:
    int genus, meshDepth;
    DiscreteGroup Gamma;
    bool isGenusSet, isMeshDepthSet, isRhoDomainSet, isRhoTargetSet;
    std::vector<double> FNLengthsDomain, FNTwistsDomain, FNLengthsTarget, FNTwistsTarget;
    IsomH2Representation rhoDomain, rhoTarget;
    H2Mesh mesh;
    H2MeshFunction function;
    H2MeshFunctionIterator iterator;
};

#endif // EQUIVARIANTHARMONICMAPSFACTORY_H
