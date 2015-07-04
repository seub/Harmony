#include "statusbar.h"

#include <QLabel>

#include "mainwindow.h"

StatusBar::StatusBar(MainWindow *window) : QStatusBar(window)
{    
    setSizeGripEnabled(false);
    setFixedHeight(25);
    label = new QLabel;
    label->setText("Ready");
    addWidget(label);
}
