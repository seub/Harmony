#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QStatusBar>

class Window;

class StatusBar : public QStatusBar
{
    Q_OBJECT

public:
    StatusBar(Window *window);

    void resizeEvent(QResizeEvent *);
};

#endif // STATUSBAR_H
