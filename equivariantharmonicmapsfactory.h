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
    void iterate(int n=1);

    std::vector<H2GeodesicArc> getPolygonTranslatesDomain() const;

public slots:
    void run();
    void stopRunning();

private:
    void resetBooleans();
    bool isReady() const;
    void refreshFunction();

    int genus, meshDepth;
    DiscreteGroup Gamma;
    bool isGenusSet, isMeshDepthSet, isRhoDomainSet, isRhoTargetSet, isInitialized;
    bool stop;
    std::vector<double> FNLengthsDomain, FNTwistsDomain, FNLengthsTarget, FNTwistsTarget;
    IsomH2Representation rhoDomain, rhoTarget;
    H2Mesh mesh;
    H2MeshFunction functionInit, function;
    H2MeshFunctionIterator iterator;
};

#endif // EQUIVARIANTHARMONICMAPSFACTORY_H
