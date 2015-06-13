#include "inputmenu.h"

#include <QGridLayout>
#include <QStyle>

#include "window.h"

InputMenu::InputMenu(Window *window) : window(window)
{
    setParent(window);

    setTitle(tr("Input"));

    layout = new QGridLayout(this);
    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignTop);

    setRowHeights();
    setLayout(layout);
}

void InputMenu::setRowHeights()
{

}

int InputMenu::maxLeftColWidth() const
{
    return 10;
}

int InputMenu::maxRightColWidth() const
{
    return 10;
}

int InputMenu::maxWidth() const
{
        int maxi = maxLeftColWidth() + maxRightColWidth() + layout->horizontalSpacing();
        return QStyle::CE_MenuHMargin + maxi + layout->margin();
}


int InputMenu::maxHeight() const
{
    int absurdMargin = 1;
    return QStyle::CE_HeaderLabel + absurdMargin;
}

void InputMenu::resizeEvent(QResizeEvent *)
{
    int max = maxHeight();
    if(height() < max)
    {
        setMinimumHeight(max);
    }
}

void InputMenu::enterEvent(QEvent *)
{

}
