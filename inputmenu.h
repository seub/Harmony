#ifndef INPUTMENU_H
#define INPUTMENU_H

#include <QGroupBox>

#include "tools.h"

class QGridLayout; class QComboBox; class QLabel; class QSpinBox;

class Window; class ActionHandler;

class InputMenu : public QGroupBox
{
    Q_OBJECT

    friend class Window;
    friend class ActionHandler;

public:
    enum SetRhoChoice {SET_RHO_CHOOSE, SET_RHO_RANDOM, SET_RHO_NICE, SET_RHO_FN};

    InputMenu() = delete;
    InputMenu(const InputMenu &) = delete;
    InputMenu & operator=(InputMenu) = delete;

    int maxLeftColWidth() const;
    int maxRightColWidth() const;
    int maxWidth() const;
    int maxHeight() const;
    int getGenus() const;
    int getMeshDepth() const;

private:
    InputMenu(Window *window, ActionHandler *handler);
    void resizeEvent(QResizeEvent *);

    void createLayout();
    void createButtons();
    void resetRhos();

    ActionHandler *handler;

    QGridLayout *layout;
    QComboBox *setRhoDomainComboBox, *setRhoImageComboBox;
    QLabel *genusLabel, *meshDepthLabel;
    QSpinBox *genusSpinBox, *meshDepthSpinBox;


    int vertSpace;
    int buttonHeight;
};

#endif // INPUTMENU_H
