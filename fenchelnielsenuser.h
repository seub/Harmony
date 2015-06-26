#ifndef FENCHELNIELSENUSER_H
#define FENCHELNIELSENUSER_H

#include <QWidget>
#include <QGroupBox>

#include "canvascontainer.h"
#include "liftedgraph.h"

class ActionHandler; class QLabel; class QDoubleSpinBox; class QPushButton; class QGridLayout; class Canvas;

class FNselector; class FNmenu; class H2CanvasDelegateLiftedGraph;

class FenchelNielsenUser : public QWidget, public CanvasContainer
{
    Q_OBJECT

    friend class ActionHandler;

public:
    FenchelNielsenUser(ActionHandler *handler, uint genus);
    FenchelNielsenUser() = delete;
    FenchelNielsenUser(const FenchelNielsenUser &) = delete;
    FenchelNielsenUser & operator=(FenchelNielsenUser) = delete;


    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *);

public slots:
    void saveCoordinates();
    void discard();
    void refresh();
    void closeEvent(QCloseEvent *);

private:
    void createWindow();
    void createDelegatePointers();
    void canvasResized();
    int menuWidth() const;
    void optimalSize(uint &outputWidth, uint &outputHeight) const;


    ActionHandler *handler;

    QGridLayout* layout;
    FNselector* selector;
    QLabel* infoLabel;
    FNmenu* menu;
    Canvas *canvas;
    H2CanvasDelegateLiftedGraph *delegate;
    int vertSpace;

    bool saveFNcoordinates;
    uint nbLengths;

    GroupRepresentation<H2Isometry> rho;
    LiftedGraphFunctionTriangulated<H2Point, H2Isometry> graph;
};

class FNselector : public QGroupBox
{
    Q_OBJECT

    friend class FenchelNielsenUser;

public:
    int maxFirstColWidth() const;
    int maxSecondColWidth() const;
    int maxThirdColWidth() const;
    int maxFourthColWidth() const;
    int maxWidth() const;
    int maxHeight() const;

    void getFNcoordinates(std::vector<double> &lengthsOut, std::vector<double> &twistsOut) const;

private:
    FNselector(FenchelNielsenUser *user, uint nbLengths);

    void resizeEvent(QResizeEvent *);

    void createLayout();
    void createButtons();
    void resetRhos();

    FenchelNielsenUser* user;
    QGridLayout* layout;
    std::vector<QLabel*> lengthsLabels, twistsLabels;
    std::vector<QDoubleSpinBox*> lengthsBoxes, twistsBoxes;

    uint nbLengths;
    int vertSpace;
    int buttonHeight;
};

class FNmenu : public QGroupBox
{
    Q_OBJECT

    friend class FenchelNielsenUser;

public:
    int maxWidth() const;
    int maxHeight() const;

private:
    FNmenu(FenchelNielsenUser *user, int minWidth = 0);

    void resizeEvent(QResizeEvent *);

    void createLayout();
    void createButtons();

    QGridLayout* layout;
    QPushButton *setButton, *discardButton;
    QLabel *questionLabel;
    FenchelNielsenUser *user;

    int vertSpace;
    int buttonHeight, buttonMaxWidth;
};


#endif // FENCHELNIELSENUSER_H
