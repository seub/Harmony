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
    explicit H2MeshFunctionIterator(const H2MeshFunction * const f);
    H2MeshFunctionIterator() = delete;
    H2MeshFunctionIterator(const H2MeshFunction&) = delete;
    H2MeshFunctionIterator & operator=(H2MeshFunctionIterator) = delete;


    void iterate(int n=1);
    void getOutput(H2MeshFunction &output);

    double supError() const;

private:
    void initialize();

    const H2MeshFunction * const fInit;
    std::vector<H2Point> oldValues, newValues;
    const H2Mesh *mesh;


};

#endif // H2MESHFUNCTIONITERATOR_H
