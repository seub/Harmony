#ifndef DISPLAYMENU_H
#define DISPLAYMENU_H

#include <QGroupBox>

#include "tools.h"

class QGridLayout; class QComboBox; class QLabel;

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

    ActionHandler *handler;

    QGridLayout *layout;
    QLabel *domainLabel, *imageLabel;
    QComboBox *showTranslatesDomainComboBox, *showTranslatesImageComboBox;

    int vertSpace;
    int buttonHeight;

};

#endif // DISPLAYMENU_H
