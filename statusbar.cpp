#include "statusbar.h"

#include <QLabel>

#include "window.h"

StatusBar::StatusBar(Window *window) : QStatusBar(window)
{    
    setSizeGripEnabled(false);
    setFixedHeight(25);
    label = new QLabel;
    label->setText("Ready");
    addWidget(label);
}
