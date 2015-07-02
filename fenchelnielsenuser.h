#ifndef FENCHELNIELSENUSER_H
#define FENCHELNIELSENUSER_H

#include <QWidget>
#include <QGroupBox>

#include "liftedgraph.h"

class ActionHandler; class QLabel; class QDoubleSpinBox; class QPushButton; class QGridLayout; class Canvas;

class FNselector; class FNmenu; class H2CanvasDelegateLiftedGraph;

class FenchelNielsenUser : public QWidget
{
    Q_OBJECT

    friend class ActionHandler;

public:
    FenchelNielsenUser(ActionHandler *handler, uint genus);
    FenchelNielsenUser() = delete;
    FenchelNielsenUser(const FenchelNielsenUser &) = delete;
    FenchelNielsenUser & operator=(FenchelNielsenUser) = delete;

    void showEvent(QShowEvent *);

public slots:
    void saveCoordinates();
    void discard();
    void refresh();
    void closeEvent(QCloseEvent *);

private:
    void createWindow();
    void createDelegatePointers();

    int canvasMinimumSize() const;
    QSize optimalSize() const;


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
    void getFNcoordinates(std::vector<double> &lengthsOut, std::vector<double> &twistsOut) const;

private:
    FNselector(FenchelNielsenUser *user, uint nbLengths);

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

private:
    FNmenu(FenchelNielsenUser *user);

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
