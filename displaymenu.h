#ifndef DISPLAYMENU_H
#define DISPLAYMENU_H

#include <QGroupBox>

#include "tools.h"

class QGridLayout; class QComboBox; class QLabel; class QPushButton;

class LeftMenu;

class DisplayMenu : public QGroupBox
{
    Q_OBJECT

    friend class LeftMenu;
    friend class ActionHandler;

public:
    enum ShowTranslatesChoice {SHOW_TRANSLATES_DOMAIN, SHOW_TRANSLATES_VERTEX, SHOW_TRANSLATES_VERTICES, SHOW_TRANSLATES_VERTICES_STAR};
    //enum ColoringChoice {COLORING_NONE, COLORING_PLAIN};
    enum ColoringChoiceLeft {COLORING_NONE_LEFT, COLORING_PLAIN_LEFT};
    enum ColoringChoiceRight {COLORING_NONE_RIGHT, COLORING_PLAIN_RIGHT};
    //enum ColorChoice {RED, GREEN, BLUE, LIGHT_BLUE, ORANGE, GRAY, BLACK};
    enum ColorChoiceLeft {RED_L, GREEN_L, BLUE_L, LIGHT_BLUE_L, ORANGE_L, GRAY_L, BLACK_L};
    enum ColorChoiceRight {RED_R, GREEN_R, BLUE_R, LIGHT_BLUE_R, ORANGE_R, GRAY_R, BLACK_R};
    enum FlowChoice {FLOW_ENERGY,FLOW_CENTROID};

    DisplayMenu() = delete;
    DisplayMenu(const DisplayMenu &) = delete;
    DisplayMenu& operator=(DisplayMenu) = delete;

private:
    DisplayMenu(LeftMenu *leftMenu);

    void createLayout();
    void createButtons();
    void setReady(bool left);

    QGridLayout *layout;
    QPushButton *resetViewButton;
    QLabel *showTranslatesLabel;
    QComboBox *showTranslatesComboBox;
    QLabel *coloringLabel;
    //QComboBox *coloringComboBox, *colorComboBox;
    QComboBox *coloringComboBoxLeft,*coloringComboBoxRight, *colorComboBoxLeft, *colorComboBoxRight;
    QLabel *flowLabel;
    QComboBox *flowBox;
    int vertSpace;
    int buttonHeight;

};

#endif // DISPLAYMENU_H
