#ifndef INPUTMENU_H
#define INPUTMENU_H

#include <QGroupBox>

#include "tools.h"

class QGridLayout;

class Window;

class InputMenu : public QGroupBox
{
    Q_OBJECT

    friend class Window;

public:
    int maxLeftColWidth() const;
    int maxRightColWidth() const;
    int maxWidth() const;
    int maxHeight() const;


signals:

public slots:

private:
    InputMenu(Window *window);
    void resizeEvent(QResizeEvent *);
    void enterEvent(QEvent *);
    void setRowHeights();

    Window * const window;
    QGridLayout *layout;

};

#endif // INPUTMENU_H
