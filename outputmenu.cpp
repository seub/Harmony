#include "outputmenu.h"

#include <QGridLayout>
#include <QStyle>

#include "window.h"

OutputMenu::OutputMenu(Window * const window) : window(window)
{
    setParent(window);
    setTitle(tr("Output"));

    layout = new QGridLayout;
    layout->setSpacing(0);
    setLayout(layout);
}

void OutputMenu::resizeEvent(QResizeEvent *)
{
    if(height() < maxHeight())
    {
        setMinimumHeight(maxHeight());
    }
}

int OutputMenu::maxLeftColWidth() const
{
    int leftMax = 0;

    return leftMax;
}

int OutputMenu::maxRightColWidth() const
{
    int rightMax = 0;

    return rightMax;
}

int OutputMenu::maxWidth() const
{
    int maxi = maxLeftColWidth() + maxRightColWidth() + layout->horizontalSpacing();
    maxi = std::max(maxi, 0);
    return maxi + layout->margin() + QStyle::CE_MenuHMargin;
}

int OutputMenu::maxHeight() const
{
    int absurdMargin = 1;
    return QStyle::CE_HeaderLabel + absurdMargin;
}

void OutputMenu::enterEvent(QEvent *)
{
}
