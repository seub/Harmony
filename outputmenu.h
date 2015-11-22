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
    OutputMenu() = delete;
    OutputMenu(const OutputMenu &) = delete;
    OutputMenu & operator=(OutputMenu) = delete;

private:
    OutputMenu(LeftMenu *leftMenu);
    void resetMenu();

    void createLayout();
    void createButtons();

    void enableAll();
    void disableAllButStop();
    void enableReset();

    void switchComputeToStopButton();
    void switchStopToComputeButton();

    QGridLayout *layout;
    QPushButton *resetButton, *iterateButton, *computeButton;
    QLabel *iterateLabel, *showLiveLabel;
    QSpinBox *iterateSpinBox;
    QCheckBox *showLiveCheckbox;

    int vertSpace;
    int buttonHeight;
};

#endif // OUTPUTMENU_H
