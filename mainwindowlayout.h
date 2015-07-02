#ifndef MAINWINDOWLAYOUT_H
#define MAINWINDOWLAYOUT_H

#include <QLayout>

class InputMenu; class DisplayMenu; class OutputMenu; class Canvas; class StatusBar;

class MainWindowLayout : public QLayout
{
public:
    MainWindowLayout(QWidget *parent): QLayout(parent) {}
    ~MainWindowLayout();

    void addItem(QLayoutItem *item);
    QSize sizeHint() const;
    QSize minimumSize() const;
    int count() const;
    QLayoutItem *itemAt(int) const;
    QLayoutItem *takeAt(int);
    void setGeometry(const QRect &rect);

private:
    InputMenu *inputMenu;
    OutputMenu *outputMenu;
    DisplayMenu *displayMenu;
    Canvas *leftCanvas, *rightCanvas;
};

#endif // MAINWINDOWLAYOUT_H
