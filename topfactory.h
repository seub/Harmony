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

public:
    TopFactory();

    bool isMeshInitialized() const;
    bool isFunctionInitialized() const;

    void resetInitSubfactory();
    void iterateSubfactory(int N);
    void setGenus(int genus);
    void setMeshDepth(int meshDepth);

    void runHeatFlow();
    void stopHeatFlow();
    double getTimeElapsed() const;
    int getNbIterations() const;

signals:

private slots:
    void finishedComputing();

private:
    void setHandler(ActionHandler *handler);

    ActionHandler *handler;
    EquivariantHarmonicMapsFactory subfactory;

    clock_t time;
};

#endif // TOPFACTORY_H
