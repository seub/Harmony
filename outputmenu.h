#ifndef OUTPUTMENU_H
#define OUTPUTMENU_H

#include <QGroupBox>

#include "tools.h"

class QGridLayout; class QLabel; class QCheckBox; class QPushButton;

class Window; class ActionHandler;

class OutputMenu : public QGroupBox
{
    Q_OBJECT
public:
    friend class Window;
    friend class ActionHandler;

    int maxLeftColWidth() const;
    int maxRightColWidth() const;
    int maxWidth() const;
    int maxHeight() const;

private:
    OutputMenu(Window * const window, ActionHandler *handler);
    void resizeEvent(QResizeEvent *event);
    void enterEvent(QEvent *);
    void resetMenu();
    void createStandardMenu();

    void enableAll();
    void disableAllButStop();
    void enableReset();

    void switchComputeToStopButton();
    void switchStopToComputeButton();

    ActionHandler *handler;
    QGridLayout *layout;

    QPushButton *resetButton, *computeButton, *iterateButton;
    QLabel *showLiveLabel;
    QCheckBox *showLiveCheckbox;

    int vertSpace;
    int buttonHeight;
};

#endif // OUTPUTMENU_H
