#include "fenchelnielsenuser.h"

#include <QDoubleSpinBox>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QApplication>
#include <QDesktopWidget>
#include <QStyle>
#include <QResizeEvent>

#include "canvas.h"
#include "actionhandler.h"
#include "canvasdelegate.h"
#include "h2canvasdelegateliftedgraph.h"
#include "fenchelnielsenconstructor.h"

FenchelNielsenUser::FenchelNielsenUser(ActionHandler *handler, uint genus) : handler(handler)
{
    nbLengths = 3*genus - 3;
    saveFNcoordinates = false;

    createWindow();
    createDelegatePointers();
    setAttribute(Qt::WA_DeleteOnClose);
}

void FenchelNielsenUser::createWindow()
{
    setWindowTitle("Fenchel-Nielsen coordinates selector");

    selector = new FNselector(this, nbLengths);
    selector->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    selector->setEnabled(true);

    QString message = QString("NB: The canvas shows the fundamental domain and initial triangulation ")
            .append(QString(tr("that would be chosen for a <i> domain </i> representation")));
    infoLabel = new QLabel(message);
    infoLabel->setWordWrap(true);
    infoLabel->setMinimumWidth(selector->sizeHint().width());
    infoLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    infoLabel->setVisible(true);
    infoLabel->setEnabled(true);

    menu = new FNmenu(this);
    menu->setMinimumWidth(selector->sizeHint().width());
    menu->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    menu->setEnabled(true);

    canvas = new Canvas(this);
    canvas->setEnabled(true);
    canvas->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    canvas->setMinimumSize(QSize(canvasMinimumSize(), canvasMinimumSize()));

    layout = new QGridLayout;
    layout->setMargin(9);
    layout->setSpacing(6);

    vertSpace = 20;
    layout->setRowMinimumHeight(1, vertSpace);
    layout->addWidget(canvas, 0, 1, 4, 1);
    layout->addWidget(selector, 0, 0, Qt::AlignCenter);
    layout->addWidget(infoLabel, 2, 0, Qt::AlignCenter);
    layout->addWidget(menu, 3, 0, Qt::AlignCenter);
    setLayout(layout);

    resize(optimalSize());
    move(QApplication::desktop()->screen()->rect().center() - rect().center());
}

int FenchelNielsenUser::canvasMinimumSize() const
{
    return selector->sizeHint().width();
}

QSize FenchelNielsenUser::optimalSize() const
{
    int screenWidth = QApplication::desktop()->width();
    int screenHeight = QApplication::desktop()->height();

    int complementaryWidth = 2*layout->margin() + 1*layout->horizontalSpacing() + selector->sizeHint().width();
    int complementaryHeight = 2*layout->margin() + 0*layout->verticalSpacing();

    int canvasOptimalSizeH = Tools::intRound(0.7*screenWidth - complementaryWidth);
    int canvasOptimalSizeV = std::max(Tools::intRound(0.7*screenHeight -complementaryHeight),
                                      selector->sizeHint().height() + infoLabel->sizeHint().height() + menu->sizeHint().height() + 2*layout->verticalSpacing());
    int canvasOptimalSize =  std::min(canvasOptimalSizeH, canvasOptimalSizeV);

    return QSize(complementaryWidth + canvasOptimalSize, complementaryHeight + canvasOptimalSize);
}


void FenchelNielsenUser::createDelegatePointers()
{
    delegate = static_cast<H2CanvasDelegateLiftedGraph*>(canvas->delegate);
    delegate->setRhoPointer(&(this->rho));
    delegate->setGraphPointer(&(this->graph));
    delegate->graphSidesTranslatesColor = "gray";
    delegate->setShowTranslates(false, false, false);
}



void FenchelNielsenUser::saveCoordinates()
{
    saveFNcoordinates = true;
    close();
}

void FenchelNielsenUser::discard()
{
    saveFNcoordinates = false;
    close();
}

void FenchelNielsenUser::closeEvent(QCloseEvent *)
{
    if (saveFNcoordinates)
    {
        std::vector<double> lengths, twists;
        selector->getFNcoordinates(lengths, twists);
        handler->receiveFNcoordinates(lengths, twists);
    }
    else
    {
        handler->discardReceiveFNcoordinates();
    }
}

void FenchelNielsenUser::refresh()
{
    std::vector<double> lengths, twists;
    selector->getFNcoordinates(lengths, twists);

    FenchelNielsenConstructor FNC(lengths, twists);
    rho = FNC.getRepresentation();
    LiftedGraphFunctionTriangulated<H2Point, H2Isometry> tempGraph(rho, 0);
    graph.cloneCopyAssign(&tempGraph);


    delegate->setIsRhoEmpty(false);
    delegate->setIsGraphEmpty(false);

    delegate->refreshRho();
    delegate->updateGraph(true);
    canvas->updateRefresh(true, true);
}

void FenchelNielsenUser::showEvent(QShowEvent *)
{
    refresh();
}











FNselector::FNselector(FenchelNielsenUser *user, uint nbLengths) : user(user), nbLengths(nbLengths)
{
    setParent(user);
    vertSpace = 5;

    createButtons();
    createLayout();
}

void FNselector::createButtons()
{
    buttonHeight = 30;

    QString numi;
    QDoubleSpinBox *lengthBox, *twistBox;
    QLabel *lengthLabel, *twistLabel;
    for (uint i=0; i!=nbLengths; ++i)
    {
        numi = QString::number(i+1);

        lengthLabel = new QLabel(QString("length %1: ").arg(numi));
        lengthLabel->setFixedHeight(buttonHeight);
        lengthsLabels.push_back(lengthLabel);

        twistLabel = new QLabel(QString("twist %1: ").arg(numi));
        twistLabel->setFixedHeight(buttonHeight);
        twistsLabels.push_back(twistLabel);

        lengthBox = new QDoubleSpinBox();
        lengthBox->setRange(0.10, 8.00);
        lengthBox->setValue(2.00);
        lengthBox->setSingleStep(0.1);
        lengthBox->setFixedHeight(buttonHeight);
        connect(lengthBox, SIGNAL(valueChanged(double)), user, SLOT(refresh()));
        lengthsBoxes.push_back(lengthBox);

        twistBox = new QDoubleSpinBox();
        twistBox->setRange(-12.00, 12.00);
        twistBox->setSingleStep(0.1);
        twistBox->setFixedHeight(buttonHeight);
        connect(twistBox, SIGNAL(valueChanged(double)), user, SLOT(refresh()));
        twistsBoxes.push_back(twistBox);
    }
}

void FNselector::createLayout()
{
    layout = new QGridLayout;
    layout->setSpacing(2);
    layout->setColumnMinimumWidth(2, 4*vertSpace);

    for (uint i=0; i!=nbLengths; ++i)
    {
        layout->setRowMinimumHeight(2*i, vertSpace);
        layout->setRowMinimumHeight(2*i+1, buttonHeight);
    }

    for (uint i=0; i!=nbLengths; ++i)
    {
        layout->addWidget(lengthsLabels[i], 2*i+1, 0, 1, 1, Qt::AlignRight);
        lengthsLabels[i]->setVisible(true);
        lengthsLabels[i]->setEnabled(true);

        layout->addWidget(lengthsBoxes[i], 2*i+1, 1, 1, 1, Qt::AlignLeft);
        lengthsBoxes[i]->setVisible(true);
        lengthsBoxes[i]->setEnabled(true);

        layout->addWidget(twistsLabels[i], 2*i+1, 3, 1, 1, Qt::AlignRight);
        twistsLabels[i]->setVisible(true);
        twistsLabels[i]->setEnabled(true);

        layout->addWidget(twistsBoxes[i], 2*i+1, 4, 1, 1, Qt::AlignLeft);
        twistsBoxes[i]->setVisible(true);
        twistsBoxes[i]->setEnabled(true);
    }

    setLayout(layout);
}



void FNselector::getFNcoordinates(std::vector<double> &lengthsOut, std::vector<double> &twistsOut) const
{
    lengthsOut.clear();
    twistsOut.clear();
    lengthsOut.reserve(nbLengths);
    twistsOut.reserve(nbLengths);
    for (uint i=0; i!=nbLengths; ++i)
    {
        lengthsOut.push_back(lengthsBoxes[i]->value());
        twistsOut.push_back(twistsBoxes[i]->value());
    }
}



FNmenu::FNmenu(FenchelNielsenUser *user) : user(user)
{
    setParent(user);
    vertSpace = 5;

    createButtons();
    createLayout();
}

void FNmenu::createButtons()
{
    setButton = new QPushButton("Proceed");

    buttonHeight = setButton->sizeHint().height();

    setButton->setFixedHeight(buttonHeight);
    setButton->setMaximumWidth(setButton->sizeHint().width());
    connect(setButton, SIGNAL(clicked()), user, SLOT(saveCoordinates()));

    discardButton = new QPushButton("Discard");
    discardButton->setFixedHeight(buttonHeight);
    discardButton->setMaximumWidth(discardButton->sizeHint().width());
    connect(discardButton, SIGNAL(clicked()), user, SLOT(discard()));

    questionLabel = new QLabel("Save these Fenchel-Nielsen coordinates?");
    questionLabel->setWordWrap(true);
}

void FNmenu::createLayout()
{
    layout = new QGridLayout;

    layout->setRowMinimumHeight(0, 2*vertSpace);
    layout->setRowMinimumHeight(1, 4*vertSpace);
    layout->setRowMinimumHeight(2, 3*vertSpace);
    layout->setRowMinimumHeight(3, buttonHeight);

    //layout->setColumnMinimumWidth(0, maxWidth());
    setLayout(layout);

    layout->addWidget(questionLabel, 1, 0, 1, 2);
    questionLabel->setVisible(true);
    questionLabel->setEnabled(true);

    layout->addWidget(setButton, 3, 0, 1, 1);
    setButton->setVisible(true);
    setButton->setEnabled(true);

    layout->addWidget(discardButton, 3, 1, 1, 1);
    discardButton->setVisible(true);
    discardButton->setEnabled(true);
}
