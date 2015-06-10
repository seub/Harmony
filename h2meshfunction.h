#ifndef H2MESHFUNCTION_H
#define H2MESHFUNCTION_H

#include "tools.h"
#include "h2mesh.h"

class H2MeshFunction
{
    friend class H2MeshFunctionIterator;

public:
    H2MeshFunction(const H2Mesh * const mesh, const IsomH2Representation &rhoImage);
    void initializePL(const H2Point &basePoint);
    void iterate(int n=1);

private:
    H2MeshFunction(const H2Mesh * const mesh, const IsomH2Representation &rhoImage, std::vector<H2Point> values);
    const H2Mesh * const mesh;
    IsomH2Representation rhoImage;
    std::vector<H2Point> values;
};

#endif // H2MESHFUNCTION_H
