#include "h2pentagon.h"
#include "h2triangle.h"

H2OneHoledTorus::H2OneHoledTorus()
{
    vertices.reserve(5);
}

H2OneHoledTorus::H2OneHoledTorus(std::vector<H2Point> &vertices, H2Isometry &pair13, H2Isometry &pair24):
    vertices(vertices), pair13(pair13), pair24(pair24)
{
    if(vertices.size()!=5 || pair13*vertices[0]!=vertices[3] ||
            pair13*vertices[1]!=vertices[2] || pair24*vertices[1]!=vertices[4] || pair24*vertices[2]!=vertices[3])
    {
        std::cout << "That's not a uniformized one-holed torus" << std::endl;
    }
}

void H2OneHoledTorus::getVertices(std::vector<H2Point> & vertices) const
{
    vertices = this->vertices;
    return;
}

void H2OneHoledTorus::getAngles(std::vector<double> & angles)
{
    angles.clear();
    H2Triangle T;
    double angle, dummy;
    for(unsigned int j=0; j<vertices.size(); j++)
    {
        T.setPoints(vertices[j==0 ? 4 : j-1], vertices[j>3 ? 0 : j+1], vertices[j>2 ? j-3 : j+2]);
        T.getAngles(dummy,angle,dummy);
        angles.push_back(angle);
    }
    return;
}
