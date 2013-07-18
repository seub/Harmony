#include "h2mesh.h"
#include "h2geodesic.h"

H2mesh::H2mesh()
{

}

H2mesh::H2mesh(double step, const H2Polygon & polygon): step(step), polygon(polygon)
{
    polygon.getExtremalCoordinatesInDiskModel(xMin,xMax,yMin,yMax);

    nbPointsX = 1 + int((xMax - xMin + (step/2.0))/step);
    nbPointsY = 1 + int((yMax - yMin + (step/2.0))/step);
    nbMeshPoints = nbPointsX*nbPointsY;
    firstMeshPoint = complex( xMin - (step/2.0) , yMin -step/2.0 );

    meshPoints = new complex[nbMeshPoints];
    isInside = new bool[nbMeshPoints];
    leftNeighbors = new int[nbMeshPoints];


    int i, j;
    double x = real(firstMeshPoint), y = imag(firstMeshPoint);
    for (j=0; j<nbPointsY; j++)
    {
        x = real(firstMeshPoint);
        for (i=0; i<nbPointsX; i++)
        {
            meshPoints[i + j*nbPointsX] = complex(x,y);
            x += step;
        }
        y += step;
    }
    xMin -= step/2.0;
    yMin -= step/2.0;
    xMax = xMin + (nbPointsX-1)*step;
    yMax = yMin + (nbPointsY-1)*step;
    fillIsInside();
}

void H2mesh::fillIsInside()
{
    for (int i=0; i<nbMeshPoints; i++)
    {
        isInside[i] = polygon.isInsideInDiskModel(meshPoints[i]);
    }
    return;
}

void H2mesh::fillNeighbors()
{

}

H2mesh::~H2mesh()
{
    delete [] meshPoints;
    delete [] isInside;
}

const complex * H2mesh::getMeshPoints() const
{
    return meshPoints;
}

const bool * H2mesh::getIsInside() const
{
    return isInside;
}

int H2mesh::getNbMeshPoints() const
{
    return nbMeshPoints;
}

int H2mesh::getClosestMeshIndex(const complex &z) const
{
    if (real(z) < xMin || real(z) > xMax  || imag(z) < yMin || imag(z) > yMax)
    {
        std::cout << "Error in H2mesh::getClosestMeshIndex: Point is not within mesh boundaries" << std::endl;
        return -1;
    }

    double x=real(z), y=imag(z);
    int NcloseX = (x - real(firstMeshPoint))/step;
    int NcloseY = (y - imag(firstMeshPoint))/step;
    int index = NcloseX + nbPointsX*NcloseY;
    double bestStep = step*step;
    int output;


    if (!polygon.isInsideInDiskModel(z))
    {
        std::cout << "Error in H2mesh::getClosestMeshIndex: Point is not inside polygon" << std::endl;
        return index;
    }
    if (norm(z - meshPoints[index]) < bestStep && isInside[index])
    {
        output = index;
        bestStep = norm(z - meshPoints[index]);
    }
    if (norm(z - meshPoints[index + 1]) < bestStep && isInside[index + 1])
    {
        output = index + 1;
        bestStep = norm(z - meshPoints[index + 1]);
    }
    if (norm(z - meshPoints[index + nbPointsX]) < bestStep && isInside[index + nbPointsX])
    {
        output = index + nbPointsX;
        bestStep = norm(z - meshPoints[nbPointsX]);
    }
    if (norm(z - meshPoints[index + nbPointsX + 1]) < bestStep && isInside[index + nbPointsX + 1])
    {
        output = index + nbPointsX + 1;
    }
    return output;
}
















