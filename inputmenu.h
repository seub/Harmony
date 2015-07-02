#ifndef INPUTMENU_H
#define INPUTMENU_H

#include <QGroupBox>

#include "tools.h"

class QGridLayout; class QComboBox; class QLabel; class QSpinBox;

class LeftMenu;

class InputMenu : public QGroupBox
{
    Q_OBJECT

    friend class LeftMenu;
    friend class ActionHandler;

public:
    enum SetRhoChoice {SET_RHO_CHOOSE, SET_RHO_RANDOM, SET_RHO_NICE, SET_RHO_FN};

    InputMenu() = delete;
    InputMenu(const InputMenu &) = delete;
    InputMenu & operator=(InputMenu) = delete;

    int getGenus() const;
    int getMeshDepth() const;

private:
    InputMenu(LeftMenu *leftMenu);

    void createLayout();
    void createButtons();
    void resetRhos();

    QGridLayout *layout;
    QComboBox *setRhoDomainComboBox, *setRhoImageComboBox;
    QLabel *genusLabel, *meshDepthLabel;
    QSpinBox *genusSpinBox, *meshDepthSpinBox;


    int vertSpace;
    int buttonHeight;
};

#endif // INPUTMENU_H
