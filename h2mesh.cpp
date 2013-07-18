#include "h2mesh.h"
#include "h2geodesic.h"

H2mesh::H2mesh()
{

}

H2mesh::H2mesh(double step, const H2Polygon & polygon): step(step), polygon(polygon)
{
    double xMin,xMax,yMin,yMax;
    polygon.getExtremalCoordinatesInDiskModel(xMin,xMax,yMin,yMax);

    nbPointsX = 1 + int((xMax - xMin + (step/2.0))/step);
    nbPointsY = 1 + int((yMax - yMin + (step/2.0))/step);
    nbMeshPoints = nbPointsX*nbPointsY;
    firstMeshPoint = complex( xMin - (step/2.0) , yMin -step/2.0 );

    meshPoints = new complex[nbMeshPoints];
    isInside = new bool[nbMeshPoints];

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

/*void H2mesh::fillIsInside()
{
    std::vector<H2GeodesicArc> sides = polygon.getSides();
    std::vector<bool> areLines;

    std::vector<complex> centers;
    std::vector<double> radii;
    std::vector<complex> directions;

    complex v1, v2, v3, direction;

    bool isLine;
    unsigned int k;
    for (k=0; k<sides.size(); k++)
    {
        isLine = sides[k].isLineSegmentInDiskModel();
        areLines.push_back(isLine);
        if (isLine)
        {
            centers.push_back(0.0);
            radii.push_back(0.0);
            v1 = polygon.getVertex(k).getDiskCoordinate();
            v2 = polygon.getVertex((k+1)%sides.size()).getDiskCoordinate();
            v3 = polygon.getVertex((k+2)%sides.size()).getDiskCoordinate();
            direction = v2 - v1;
            if (imag( (v3-v2) * conj(direction) ) < 0)
            {
                direction = -direction;
            }
            std::cout << "direction = " << direction << std::endl;
            directions.push_back(direction);

        }
        else
        {
            directions.push_back(0.0);
            centers.push_back(sides[k].getCircleCenterInDiskModel());
            radii.push_back(sides[k].getCircleRadiusInDiskModel());
        }
    }


    complex z;
    int i;
    for (i=0; i<nbMeshPoints; i++)
    {
        z = meshPoints[i];
        k=0;
        isInside[i] = true;
        while(k<sides.size() && isInside[i])
        {
            if (areLines[k])
            {
                if (imag(z - polygon.getVertex(k).getDiskCoordinate() * conj(directions[k])) < 0.0)
                {
                    isInside[i] = false;
                }
            }
            else
            {
                if (norm(centers[k]-z) < radii[k]*radii[k])
                {
                    isInside[i] = false;
                }
            }
            k++;
        }
    }

    return;
}*/

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
    double x=real(z), y=imag(z);
    int NcloseX = (x - real(firstMeshPoint))/step;
    int NcloseY = (y - imag(firstMeshPoint))/step;
    int index = NcloseX + nbPointsX*NcloseY;
    double bestStep = step*step;
    int output;

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
















