#include "canvasdelegatetests.h"

#include <QImage>


void CanvasDelegateTests::plotSequences()
{
    rescaleForPlottingSequences();

    drawSegment(Complex(0.0, yMin()), Complex(0.0, yMax));
    drawSegment(Complex(xMin, 0.0), Complex(xMax(), 0.0));

    uint i, n;
    for (i=0; i!=sequences.size(); ++i)
    {
        for (n=0; n< sequences[i].size(); ++n)
        {
            drawPoint(Complex(n+1, sequences[i][n]), sequencesColors[i], 2);
        }
    }
}

void CanvasDelegateTests::rescaleForPlottingSequences()
{
    std::vector<double> maxs;
    for (const auto &sequence : sequences)
    {
        maxs.push_back(*std::max_element(sequence.begin(), sequence.end()));
    }

    double max = *std::max_element(maxs.begin(), maxs.end());
    double mathsSizeY = 1.1*max;
    yMax = 1.05*max;
    
    std::vector<uint> sequencesNbElements;
    for (const auto &sequence : sequences)
    {
        sequencesNbElements.push_back(sequence.size());
    }

    max = *std::max_element(sequencesNbElements.begin(), sequencesNbElements.end());
    double mathsSizeX = 1.1*max;
    xMin = -0.05*max;

    scaleX = sizeX/mathsSizeX;
    scaleY = sizeY/mathsSizeY;
}


void CanvasDelegateTests::addSequence(const std::vector<double> sequence, const QColor &color)
{
    sequences.push_back(sequence);
    sequencesColors.push_back(color);
}


void CanvasDelegateTests::redraw(bool, bool)
{
    imageBack->fill("white");
    plotSequences();
}




void CanvasDelegateTests2::plotSequences()
{
    uint i, n;
    for (i=0; i!=sequences.size(); ++i)
    {
        for (n=0; n+1< sequences[i].size(); ++n)
        {
            drawH2Point(sequences[i][n], sequencesColors[i], 2);
        }
        drawH2Point(sequences[i].back(), "red", 4);
        drawH2GeodesicArc(H2GeodesicArc(sequences[i].front(), sequences[i].back()));
    }
}


void CanvasDelegateTests2::addSequence(const std::vector<H2Point> sequence, const QColor &color)
{
    sequences.push_back(sequence);
    sequencesColors.push_back(color);
}


void CanvasDelegateTests2::redraw(bool, bool)
{
    imageBack->fill("white");
    drawCircle(0,1);

    plotSequences();
}
