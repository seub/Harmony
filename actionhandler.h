#ifndef ACTIONHANDLER_H
#define ACTIONHANDLER_H

#include <QObject>

#include "tools.h"

class MathsContainer; class H2CanvasDelegate; class EquivariantHarmonicMapsFactory; class Window; class Canvas;
class InputMenu; class DisplayMenu; class OutputMenu; class Canvas; class TopFactory;

class ActionHandler : public QObject
{
    Q_OBJECT

    friend class Canvas;

public:
    ActionHandler();

    void setWindow(Window *window);
    void setContainer(MathsContainer *container);
    void setFactory();
    void processMessage(actionHandlerMessage message, int parameter = 0);

private slots:
    void genusClicked(int choice);
    void meshDepthClicked(int choice);
    void setRhoDomainClicked(int choice);
    void setRhoImageClicked(int choice);
    void computeButtonClicked();
    void stopButtonClicked();
    void iterateButtonClicked();
    void outputResetButtonClicked();
    void imageShowTranslatesClicked(int choice);
    void domainShowTranslatesClicked(int choice);

    void finishedComputing();
    void updateFunction(bool updateTranslates);
    void updateMesh(bool updateTranslates);

private:
    void runDiscreteFlow();
    void iterateDiscreteFlow(int N);

    Window *window;
    Canvas *leftCanvas, *rightCanvas;
    InputMenu* inputMenu;
    OutputMenu* outputMenu;
    DisplayMenu* displayMenu;

    MathsContainer *container;
    TopFactory *topFactory;

    H2CanvasDelegate *leftDelegate, *rightDelegate;
    bool isShowingLive;
    bool showTranslatesAroundVertexOld, showTranslatesAroundVerticesOld;
};

#endif // ACTIONHANDLER_H
