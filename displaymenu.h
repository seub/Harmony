#ifndef DISPLAYMENU_H
#define DISPLAYMENU_H

#include <QGroupBox>

#include "tools.h"

class QGridLayout; class QComboBox; class QLabel; class QPushButton;

class Window; class ActionHandler;

class DisplayMenu : public QGroupBox
{
    Q_OBJECT

    friend class Window;
    friend class ActionHandler;

public:
    enum ShowTranslatesChoice {SHOW_TRANSLATES_DOMAIN, SHOW_TRANSLATES_VERTEX, SHOW_TRANSLATES_VERTICES};
    enum ColoringChoice {COLORING_NONE, COLORING_PLAIN};

    DisplayMenu() = delete;
    DisplayMenu(const DisplayMenu &) = delete;
    DisplayMenu& operator=(DisplayMenu) = delete;

    int maxLeftColWidth() const;
    int maxRightColWidth() const;
    int maxWidth() const;
    int maxHeight() const;

private:
    DisplayMenu(Window *window, ActionHandler *handler);
    void resizeEvent(QResizeEvent *);

    void createLayout();
    void createButtons();
    void setReady(bool left);

    ActionHandler *handler;

    QGridLayout *layout;
    QPushButton *resetViewButton;
    QLabel *showTranslatesLabel;
    QComboBox *showTranslatesComboBox;
    QLabel *coloringLabel;
    QComboBox *coloringComboBox;
    int vertSpace;
    int buttonHeight;

};

#endif // DISPLAYMENU_H
