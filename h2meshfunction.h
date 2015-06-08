#ifndef H2MESHFUNCTION_H
#define H2MESHFUNCTION_H

#include "tools.h"
#include "h2mesh.h"

class H2MeshFunction
{
public:
    H2MeshFunction(const H2Mesh * const mesh, const IsomH2Representation &rhoImage);
    void initializePL(const H2Point &basePoint);

private:
    const H2Mesh * const mesh;
    IsomH2Representation rhoImage;
    std::vector<H2Point> values;
};

#endif // H2MESHFUNCTION_H
