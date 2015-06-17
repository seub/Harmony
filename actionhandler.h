#ifndef ACTIONHANDLER_H
#define ACTIONHANDLER_H

#include <QObject>

#include "tools.h"

class MathsContainer; class H2CanvasDelegate; class EquivariantHarmonicMapsFactory; class Window; class Canvas;
class InputMenu; class OutputMenu; class Canvas; class TopFactory;

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
    void computeButtonClicked();
    void stopButtonClicked();
    void finishedComputing();
    void iterateButtonClicked();
    void outputResetButtonClicked();
    void updateFunction(bool updateTranslates = true);
    void outputShowTranslatesChoice(int choice);

private:
    void runDiscreteFlow();
    void iterateDiscreteFlow(int N);

    Window *window;
    Canvas *leftCanvas, *rightCanvas;
    InputMenu* inputMenu;
    OutputMenu* outputMenu;

    MathsContainer *container;
    TopFactory *topFactory;

    H2CanvasDelegate *leftDelegate, *rightDelegate;
    bool isShowingLive;
    bool showTranslatesAroundVertexOld, showTranslatesAroundVerticesOld;
};

#endif // ACTIONHANDLER_H
