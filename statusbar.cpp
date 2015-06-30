#include "statusbar.h"

#include <QResizeEvent>

#include "window.h"

StatusBar::StatusBar(Window *window) : QStatusBar(window)
{
}

void StatusBar::resizeEvent(QResizeEvent *)
{
}
