#ifndef OUTPUTMENU_H
#define OUTPUTMENU_H

#include <QGroupBox>

#include "tools.h"

class QGridLayout;

class Window;

class OutputMenu : public QGroupBox
{
    Q_OBJECT
public:
    friend class Window;

    int maxLeftColWidth() const;
    int maxRightColWidth() const;
    int maxWidth() const;
    int maxHeight() const;

signals:

public slots:

private:
    OutputMenu(Window * const window);
    void resizeEvent(QResizeEvent *event);
    void enterEvent(QEvent *);

    Window* const window;
    QGridLayout *layout;
};

#endif // OUTPUTMENU_H
