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
    int maxLeftColWidth() const;
    int maxRightColWidth() const;
    int maxWidth() const;
    int maxHeight() const;


signals:

public slots:

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
    int vertSpace;
    int buttonHeight;

};

#endif // DISPLAYMENU_H
