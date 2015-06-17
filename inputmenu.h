#ifndef INPUTMENU_H
#define INPUTMENU_H

#include <QGroupBox>

#include "tools.h"

class QGridLayout; class QComboBox;

class Window; class ActionHandler;

class InputMenu : public QGroupBox
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
    InputMenu(Window *window, ActionHandler *handler);
    void resizeEvent(QResizeEvent *);

    void createLayout();
    void createButtons();

    ActionHandler *handler;

    QGridLayout *layout;
    QComboBox *showTranslatesComboBox;

    int vertSpace;
    int buttonHeight;
};

#endif // INPUTMENU_H
