#ifndef CANVASDELEGATETESTS_H
#define CANVASDELEGATETESTS_H

#include "tools.h"
#include "canvasdelegate.h"
#include "h2canvasdelegate.h"


class CanvasDelegateTests : public CanvasDelegate
{
public:
    CanvasDelegateTests(uint size) : CanvasDelegate(size, size) {}
    void plotSequences();
    void addSequence(const std::vector<double> sequence, const QColor & color);

private:
    void rescaleForPlottingSequences();
    void redraw(bool back, bool top);

    
    std::vector< std::vector<double> > sequences;
    std::vector<QColor> sequencesColors;
};

class CanvasDelegateTests2 : public H2CanvasDelegate
{

public:
    CanvasDelegateTests2(uint size) : H2CanvasDelegate(size, size) {}
    void plotSequences();
    void addSequence(const std::vector<H2Point> sequence, const QColor &color);

private:
    void redraw(bool back, bool top);

    std::vector< std::vector<H2Point> > sequences;
    std::vector<QColor> sequencesColors;
};




#endif // CANVASDELEGATETESTS_H
