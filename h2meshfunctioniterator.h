#ifndef H2MESHFUNCTIONITERATOR_H
#define H2MESHFUNCTIONITERATOR_H

#include "tools.h"

class H2Mesh;
class H2MeshFunction;
class H2Point;

class H2MeshFunctionIterator
{
    friend class EquivariantHarmonicMapsFactory;

public:
    H2MeshFunctionIterator(const H2MeshFunction * const f);
    void iterate(int n=1);
    void getOutput(H2MeshFunction &output);

    double supError() const;

private:
    const H2MeshFunction * const fInit;
    std::vector<H2Point> oldValues, newValues;
    const H2Mesh * const mesh;


};

#endif // H2MESHFUNCTIONITERATOR_H
