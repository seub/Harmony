#ifndef H2MESHVECTORFIELD_H
#define H2MESHVECTORFIELD_H


#include "tools.h"
#include "h2mesh.h"

class H2MeshVectorField
{
public:
    H2MeshVectorField(const H2Mesh * const mesh, const IsomH2Representation &rho);

private:
    const H2Mesh * const mesh;
    IsomH2Representation rho;

    std::vector<double> xValues;
    std::vector<double> yValues;
};

#endif // H2MESHVECTORFIELD_H
