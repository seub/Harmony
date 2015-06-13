#include "topmenu.h"

#include "window.h"

TopMenu::TopMenu(Window * window) : window(window)
{
    setParent(window);
}
