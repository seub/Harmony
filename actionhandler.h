#ifndef ACTIONHANDLER_H
#define ACTIONHANDLER_H

#include <QObject>

#include "tools.h"

class MathsContainer; class H2CanvasDelegate; class EquivariantHarmonicMapsFactory; class Window; class Canvas;
class InputMenu; class DisplayMenu; class OutputMenu; class Canvas; class TopFactory; class QStatusBar;

enum class ActionHandlerMessage {HIGHLIGHTED_LEFT, HIGHLIGHTED_RIGHT, END_TARGET_CANVAS_REPAINT, FINISHED_COMPUTING};

class ActionHandler : public QObject
{
    Q_OBJECT

    friend class MainApplication;

public:
    ActionHandler(const ActionHandler &) = delete;
    ActionHandler & operator=(const ActionHandler &) = delete;

    void processMessage(ActionHandlerMessage message, int parameter = 0);
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

    void resetViewButtonClicked();
    void showTranslatesClicked(int choice);

    void finishedComputing();
    void meshCreated(uint nbMeshPoints);

private:
    ActionHandler();
    void resetBooleans();

    void setWindow(Window *window);
    void setContainer(MathsContainer *container);
    void setFactory();
    void inputReset();
    void resetDelegatePointers();

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
    static void randomFNcoordinates(uint genus, std::vector<double> &lengthsOut, std::vector<double> &twistsOut);
    void setReadyToCompute();
    void setDisplayMenuReady(bool left);
    void dealRhosReady();
    bool isReadyToCompute() const;


    Window *window;
    Canvas *leftCanvas, *rightCanvas;
    H2CanvasDelegate *leftDelegate, *rightDelegate;
    InputMenu* inputMenu;
    OutputMenu* outputMenu;
    DisplayMenu* displayMenu;
    QStatusBar *statusBar;

    MathsContainer *container;
    TopFactory *topFactory;

    bool isShowingLive;
    bool showTranslatesAroundVertexLeft, showTranslatesAroundVerticesLeft;
    bool showTranslatesAroundVertexRight, showTranslatesAroundVerticesRight;
    bool isRhoDomainSet, isRhoImageSet;
    bool expectingFNdomain, expectingFNtarget;
};

#endif // ACTIONHANDLER_H
