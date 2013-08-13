#include "h2mesh.h"

H2Mesh::H2Mesh(const IsomH2Representation &rho, int depth) : rho(rho), depth(depth)
{
    fundamentalDomain = rho.generatePolygon(200);
    sidePairings = rho.getSidePairingsForNormalizedFundamentalDomain();

    std::vector<H2Triangle> triangles = fundamentalDomain.triangulate();
    for (unsigned int i=0; i<triangles.size(); i++)
    {
        subdivisions.push_back(H2TriangleSubdivision(triangles[i], depth));
    }


}
