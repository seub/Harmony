#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QStatusBar>

class Window; class QLabel;

class StatusBar : public QStatusBar
{
    Q_OBJECT

public:
    StatusBar(Window *window);

private:
    QLabel *label;
};

#endif // STATUSBAR_H
