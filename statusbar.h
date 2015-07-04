#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QStatusBar>

class MainWindow; class QLabel;

class StatusBar : public QStatusBar
{
    Q_OBJECT

public:
    StatusBar(MainWindow *window);

private:
    QLabel *label;
};

#endif // STATUSBAR_H
