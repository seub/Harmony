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
    enum ColoringChoice {COLORING_NONE, COLORING_PLAIN};
    enum ColorChoice {RED, GREEN, BLUE, LIGHT_BLUE, ORANGE, GRAY, BLACK};

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
    QComboBox *coloringComboBox, *colorComboBox;
    int vertSpace;
    int buttonHeight;

};

#endif // DISPLAYMENU_H
