#ifndef OUTPUTMENU_H
#define OUTPUTMENU_H

#include <QGroupBox>

#include "tools.h"

class QGridLayout; class QLabel; class QCheckBox; class QPushButton; class QSpinBox; class QComboBox;

class LeftMenu;

class OutputMenu : public QGroupBox
{
    Q_OBJECT

    friend class LeftMenu;
    friend class ActionHandler;

public:
    enum FlowChoice {FLOW_CHOICE, FLOW_CENTROID, FLOW_ENERGY_CONSTANT_STEP};

    OutputMenu() = delete;
    OutputMenu(const OutputMenu &) = delete;
    OutputMenu & operator=(OutputMenu) = delete;

private:
    OutputMenu(LeftMenu *leftMenu);
    void resetMenu(bool fullReset = false);

    void createLayout();
    void createButtons();

    void enableAll();
    void disableAllButStop();
    void enableReset();

    void enableRunButtons(bool b);

    void switchComputeToStopButton();
    void switchStopToComputeButton();

    QGridLayout *layout;
    QPushButton *resetButton;
    QPushButton *iterateButton, *computeButton;
    QLabel *iterateLabel, *showLiveLabel;
    QSpinBox *iterateSpinBox;
    QComboBox *flowComboBox;

    QCheckBox *showLiveCheckbox;

    int vertSpace;
    int buttonHeight;
};

#endif // OUTPUTMENU_H
