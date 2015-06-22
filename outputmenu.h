#ifndef OUTPUTMENU_H
#define OUTPUTMENU_H

#include <QGroupBox>

#include "tools.h"

class QGridLayout; class QLabel; class QCheckBox; class QPushButton; class QSpinBox; class QComboBox;

class Window; class ActionHandler;

class OutputMenu : public QGroupBox
{
    Q_OBJECT

    friend class Window;
    friend class ActionHandler;

public:
    OutputMenu() = delete;
    OutputMenu(const OutputMenu &) = delete;
    OutputMenu & operator=(OutputMenu) = delete;

    int maxLeftColWidth() const;
    int maxRightColWidth() const;
    int maxWidth() const;
    int maxHeight() const;
    int getNbIterations() const;

private:
    OutputMenu(Window *window, ActionHandler *handler);
    void resizeEvent(QResizeEvent *event);
    void resetMenu();

    void createLayout();
    void createButtons();

    void enableAll();
    void disableAllButStop();
    void enableReset();

    void switchComputeToStopButton();
    void switchStopToComputeButton();

    ActionHandler *handler;

    QGridLayout *layout;
    QPushButton *resetButton, *computeButton, *iterateButton;
    QLabel *showLiveLabel, *nbIterationsLabel;
    QCheckBox *showLiveCheckbox;
    QSpinBox *nbIterationsSpinBox;

    int vertSpace;
    int buttonHeight;
};

#endif // OUTPUTMENU_H
