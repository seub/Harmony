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
    void setGenus(uint genus);
    void setMeshDepth(uint meshDepth);
    void setNiceRhoDomain();
    void setNiceRhoTarget();
    void setRhoDomain(const std::vector<double> & FNlengths, const std::vector<double> FNtwists);
    void setRhoTarget(const std::vector<double> & FNlengths, const std::vector<double> FNtwists);

    void resetRhoDomain();
    void resetRhoTarget();

    void resetInit();

    GroupRepresentation<H2Isometry> getRhoDomain() const;
    GroupRepresentation<H2Isometry> getRhoTarget() const;

    void reset();
    void iterate(uint n=1);

public slots:
    void run();
    void stopRunning();

signals:
    void meshCreated(uint nbMeshPoints);

private:
    void resetBooleans();
    bool isReady() const;

    void initializeMesh();
    void initializeFunction();
    void initializeRhoDomain();
    void initializeRhoTarget();
    void refreshFunction();

    uint genus, meshDepth;
    DiscreteGroup Gamma;
    bool isGenusSet, isMeshDepthSet, isRhoDomainSet, isRhoTargetSet, isMeshInitialized, isInitialized;
    bool stop;
    std::vector<double> FNLengthsDomain, FNTwistsDomain, FNLengthsTarget, FNTwistsTarget;
    GroupRepresentation<H2Isometry> rhoDomain, rhoTarget;
    H2Mesh mesh;
    H2MeshFunction functionInit, function;
    H2MeshFunctionIterator iterator;
    uint nbIterations;
};

#endif // EQUIVARIANTHARMONICMAPSFACTORY_H
