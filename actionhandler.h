#ifndef ACTIONHANDLER_H
#define ACTIONHANDLER_H

#include <QObject>

#include "tools.h"

class MathsContainer; class H2CanvasDelegate; class EquivariantHarmonicMapsFactory; class Window; class Canvas;
class InputMenu; class DisplayMenu; class OutputMenu; class Canvas; class TopFactory; class QStatusBar;

class ActionHandler : public QObject
{
    Q_OBJECT

    friend class Canvas;
    friend class MainApplication;
    friend class TopFactory;

public:
    void processMessage(actionHandlerMessage message, int parameter = 0);
    static void randomFNcoordinates(int genus, std::vector<double> &lengthsOut, std::vector<double> &twistsOut);
    void receiveFNcoordinates(const std::vector<double> &lengths, const std::vector<double> &twists);
    void discardReceiveFNcoordinates();

private slots:
    void genusClicked(int choice);
    void setRhoDomainClicked(int choice);
    void setRhoImageClicked(int choice);
    void meshDepthClicked(int choice);

    void outputResetButtonClicked();
    void computeButtonClicked();
    void stopButtonClicked();
    void iterateButtonClicked();
    void showTranslatesClicked(int choice);

    void finishedComputing();
    void meshCreated(int nbMeshPoints);

private:
    ActionHandler();

    void setWindow(Window *window);
    void setContainer(MathsContainer *container);
    void setFactory();
    void inputReset();

    void setRhoNiceDomain();
    void setRhoNiceTarget();
    void errorMessageForSetRhoNice();
    void setRhoRandomDomain();
    void setRhoRandomTarget();
    void setRhoFNDomain();
    void setRhoFNTarget();

    void resetStatusBarMessage();

    void updateFunction(bool updateTranslates);
    void updateMesh(bool updateTranslates);


    void setReadyToCompute();
    void setDisplayMenuReady(bool left);
    void dealRhosReady();
    bool isReadyToCompute() const;

    void resetDelegatePointers();

    Window *window;
    Canvas *leftCanvas, *rightCanvas;
    InputMenu* inputMenu;
    OutputMenu* outputMenu;
    DisplayMenu* displayMenu;
    QStatusBar *statusBar;

    MathsContainer *container;
    TopFactory *topFactory;

    H2CanvasDelegate *leftDelegate, *rightDelegate;
    bool isShowingLive;
    bool showTranslatesAroundVertexLeft, showTranslatesAroundVerticesLeft;
    bool showTranslatesAroundVertexRight, showTranslatesAroundVerticesRight;
    bool isRhoDomainSet, isRhoImageSet;
    bool expectingFNdomain, expectingFNtarget;
};

#endif // ACTIONHANDLER_H
