#ifndef TOPFACTORY_H
#define TOPFACTORY_H

#include <QObject>
#include <QTimer>

#include "tools.h"
#include "equivariantharmonicmapsfactory.h"

class ActionHandler;

class TopFactory : public QObject
{
    Q_OBJECT

    friend class ActionHandler;
    friend class MathsContainer;

public:
    TopFactory(const TopFactory &) = delete;
    TopFactory & operator=(TopFactory) = delete;

    bool isMeshInitialized() const;
    bool isFunctionInitialized() const;

    void resetInitSubfactory();
    void iterateSubfactory(uint N);
    void setGenus(uint genus);
    void setMeshDepth(uint meshDepth);

    void runHeatFlow();
    void stopHeatFlow();
    double getTimeElapsed() const;
    uint getNbIterations() const;

signals:
    void domainFunctionCreated(uint nbMeshPoints);

private slots:
    void finishedComputing();

private:
    TopFactory();
    void setHandler(ActionHandler *handler);

    ActionHandler *handler;
    EquivariantHarmonicMapsFactory subfactory;

    clock_t time;
};

#endif // TOPFACTORY_H
