#ifndef TOPFACTORY_H
#define TOPFACTORY_H

#include <QObject>
#include <QTime>
#include <QTimer>

#include "tools.h"
#include "h2discreteflowfactorythread.h"


class ActionHandler; class MathsContainer;

class TopFactory : public QObject
{
    Q_OBJECT

    friend class MathsContainer;
    friend class ActionHandler;

public:
    TopFactory(MathsContainer *const mathsContainer, ActionHandler *const handler);
    TopFactory(const TopFactory &) = delete;
    TopFactory & operator=(TopFactory) = delete;
    ~TopFactory();

    void setGenus(uint genus);
    void setMeshDepth(uint meshDepth);

    void setNiceRhoDomain();
    void setNiceRhoImage();
    void setRhoDomain(const std::vector<double> & FNlengths, const std::vector<double> FNtwists);
    void setRhoImage(const std::vector<double> & FNlengths, const std::vector<double> FNtwists);
    void resetRhoDomain();
    void resetRhoImage();

    void decideEmittingMeshCreated();

    void resetH2Flow();
    void runH2Flow();
    void stopH2Flow();

    uint getNbIterations() const;

private slots:
    void finishedComputing();
    void sendLiveMessageForStatusBar();
    void sendFinishedMessageForStatusBar();

private:
    void startTimers();
    void stopTimers();


    uint nbVertices;
    uint nbIterations;
    ActionHandler *const handler;
    H2DiscreteFlowFactoryThread h2factory;

    QTime *time;
    QTimer *timer;
};

#endif // TOPFACTORY_H
