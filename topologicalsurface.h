#ifndef TOPOLOGICALSURFACE_H
#define TOPOLOGICALSURFACE_H

class TopologicalSurface
{
public:
    TopologicalSurface();
    TopologicalSurface(int genus, int numberOfPunctures);

    int getGenus() const;
    int getNumberOfPunctures() const;

private:
    int genus;
    int numberOfPunctures;
};

#endif // TOPOLOGICALSURFACE_H
