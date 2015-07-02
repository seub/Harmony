#ifndef ACTIONHANDLER_H
#define ACTIONHANDLER_H

#include <QObject>

#include "tools.h"

class MathsContainer; class H2CanvasDelegateLiftedGraph; class EquivariantHarmonicMapsFactory; class Window; class Canvas;
class InputMenu; class DisplayMenu; class OutputMenu; class Canvas; class TopFactory; class QStatusBar;

enum class ActionHandlerMessage {HIGHLIGHTED_LEFT, HIGHLIGHTED_RIGHT, END_CANVAS_REPAINT, FINISHED_COMPUTING};

class ActionHandler : public QObject
{
    Q_OBJECT

    friend class MainApplication;

public:
    ActionHandler(const ActionHandler &) = delete;
    ActionHandler & operator=(const ActionHandler &) = delete;

    void processMessage(ActionHandlerMessage message);
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

    void resetViewButtonClicked();
    void showTranslatesClicked(int choice);
    void coloringClicked(int choice);
    void colorClicked(int choice);

    void finishedComputing();

public slots:
    void meshCreated(uint nbMeshPoints);
    void showStatusBarMessage(const QString &message, int timeout = 0);

private:
    ActionHandler();
    void resetBooleans();

    void setWindow(Window *window);
    void setContainer(MathsContainer *mathsContainer);
    void setFactory(TopFactory *topFactory);
    void inputReset();
    void resetDelegatePointers();
    void connectMenusSignals();

    void setRhoNiceDomain();
    void setRhoNiceImage();
    void errorMessageForSetRhoNice();
    void setRhoRandomDomain();
    void setRhoRandomImage();
    void setRhoFNDomain();
    void setRhoFNImage();

    void goComputeMode();
    void goBackNormalMode();

    void resetStatusBarMessage();
    void updateCanvasGraph(bool left, bool right);
    static void randomFNcoordinates(uint genus, std::vector<double> &lengthsOut, std::vector<double> &twistsOut);
    void setReadyToCompute();
    void setDisplayMenuReady(bool left);
    void dealRhoDomainReady();
    void dealRhoImageReady();
    bool isReadyToCompute() const;


    Window *window;
    Canvas *leftCanvas, *rightCanvas;
    H2CanvasDelegateLiftedGraph *leftDelegate, *rightDelegate;
    InputMenu *inputMenu;
    OutputMenu *outputMenu;
    DisplayMenu *displayMenu;
    QStatusBar *statusBar;

    MathsContainer *mathsContainer;
    TopFactory *topFactory;

    bool isShowingLive;
    bool showTranslatesAroundVertex, showTranslatesAroundVertices;
    bool isRhoDomainSet, isRhoImageSet;
    bool expectingFNdomain, expectingFNimage;
};

#endif // ACTIONHANDLER_H
