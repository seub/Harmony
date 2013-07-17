#include "h2mesh.h"

H2mesh::H2mesh()
{

}

H2mesh::H2mesh(int width): width(width)
{
    std::vector<complex> meshStore;
    meshStore.reserve(((2/width)+1)*((2/width)+1));
    for (int i=1; i<(2/width); i++)
    {
        for (int j=1; j<(2/width); j++)
        {
            meshStore.push_back(2.0*i-1.0 + I*(2.0*j-1.0));
        }
    }
    meshPoints = meshStore;
    numberOfMeshPoints = meshPoints.size();
}


complex H2mesh::getMeshPoint(int index) const
{
    return meshPoints[index];
}

std::vector<complex> H2mesh::getMeshPoints() const
{
    return meshPoints;
}

double H2mesh::getWidth() const
{
    return width;
}
