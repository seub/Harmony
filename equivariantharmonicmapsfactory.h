#ifndef EQUIVARIANTHARMONICMAPSFACTORY_H
#define EQUIVARIANTHARMONICMAPSFACTORY_H

#include <QThread>

#include "tools.h"
#include "discretegroup.h"
#include "grouprepresentation.h"
#include "h2meshfunctioniterator.h"
#include "h2meshfunction.h"


class EquivariantHarmonicMapsFactory : public QThread
{
    Q_OBJECT

    friend class ActionHandler;
    friend class TopFactory;
    friend class FenchelNielsenUser;

public:
    EquivariantHarmonicMapsFactory();
    void setGenus(int genus);
    void setMeshDepth(int meshDepth);
    void setRhoDomain(const std::vector<double> & FNlengths, const std::vector<double> FNtwists);
    void setRhoTarget(const std::vector<double> & FNlengths, const std::vector<double> FNtwists);

    void resetRhoDomain();
    void resetRhoTarget();

    void setNiceRhoDomain();
    void setNiceRhoTarget();
    void resetInit();
    void initializeMesh();
    void initializeFunction();
    void initializeRhoDomain();
    void initializeRhoTarget();

    IsomH2Representation getRhoDomain() const;
    IsomH2Representation getRhoTarget() const;

    void reset();
    void iterate(int n=1);

public slots:
    void run();
    void stopRunning();

private:
    void resetBooleans();
    bool isReady() const;
    void refreshFunction();

    int genus, meshDepth;
    DiscreteGroup Gamma;
    bool isGenusSet, isMeshDepthSet, isRhoDomainSet, isRhoTargetSet, isMeshInitialized, isInitialized;
    bool stop;
    std::vector<double> FNLengthsDomain, FNTwistsDomain, FNLengthsTarget, FNTwistsTarget;
    IsomH2Representation rhoDomain, rhoTarget;
    H2Mesh mesh;
    H2MeshFunction functionInit, function;
    H2MeshFunctionIterator iterator;
};

#endif // EQUIVARIANTHARMONICMAPSFACTORY_H
