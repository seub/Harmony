#ifndef H2CANVASDELEGATE_H
#define H2CANVASDELEGATE_H

#include "tools.h"
#include "canvasdelegate.h"

class H2CanvasDelegate : public CanvasDelegate
{
    friend class ActionHandler;
    friend class Canvas;
    friend class FenchelNielsenUser;

public:
    virtual DelegateType getDelegateType() const override {return DelegateType::H2DELEGATE;}
    virtual ~H2CanvasDelegate() {}

protected:
    H2CanvasDelegate(uint sizeX, uint sizeY, bool leftCanvas = false, bool rightCanvas = false, ActionHandler *handler = nullptr);

    H2Point pixelToH2coordinate(int x, int y) const;

    void drawH2Point(const H2Point &p, const QColor &color = "black", int width = 1, bool back = true);
    void highlightH2Point(const H2Point &p, const QColor &color = "black", int width = 5);
    void drawH2Geodesic(const H2Geodesic &L, const QColor &color = "black", int width = 1, bool back = true);
    void drawH2GeodesicArc(const H2GeodesicArc &L, const QColor &color = "black", int width = 1, bool back = true);
    void drawStraightH2GeodesicArc(const H2GeodesicArc &L, const QColor &color = "black", int width = 1, bool back = true);
    void drawH2Triangle(const H2Triangle &triangle, const QColor &color = "black", int width = 1, bool back = true);
    void highlightStraightH2Triangle(const H2Triangle &triangle, const QColor &color);
    void drawStraightFilledH2Triangle(const H2Triangle &triangle, bool back = true);


    virtual void redrawTop() override;
    virtual void resetView() override;
    virtual void enter() override;
    virtual void leave() override;
    virtual void redrawBack() override;

    H2Isometry mobius;

    virtual void mousePress (int x, int y, Qt::MouseButton button, Qt::MouseButtons buttons) override;
    virtual void mouseMove(int x, int y, Qt::MouseButton button, Qt::MouseButtons buttons) override;
    virtual void mouseRelease(int x, int y, Qt::MouseButton button, Qt::MouseButtons buttons) override;

private:
    bool mobiusing;
    H2Isometry savedMobius;
};


#endif // H2CANVASDELEGATE_H
