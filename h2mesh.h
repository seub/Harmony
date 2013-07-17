#ifndef H2MESH_H
#define H2MESH_H
#include "types.h"


class H2mesh
{
public:
    H2mesh();
    H2mesh(int width);

    complex getMeshPoint(int index) const;
    std::vector<complex> getMeshPoints() const;
    double getWidth() const;

private:
    double width;
    std::vector<complex> meshPoints;
    int numberOfMeshPoints;
};

#endif // H2MESH_H
