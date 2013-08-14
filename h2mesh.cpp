#include "h2mesh.h"

#include "h2isometry.h"
#include "h2trianglesubdivision.h"
#include "h2polygontriangulater.h"

H2Mesh::H2Mesh(const IsomH2Representation &rho, int depth) : rho(rho), depth(depth)
{
    fundamentalDomain = rho.generatePolygon(200);
    sidePairings = rho.getSidePairingsForNormalizedFundamentalDomain();

    H2PolygonTriangulater PT(&fundamentalDomain);

}
