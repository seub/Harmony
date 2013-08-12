#include "h2mesh.h"

H2Mesh::H2Mesh(const IsomH2Representation &rho, int depth) : rho(rho), depth(depth)
{
    fundamentalDomain = rho.generatePolygon(200);
    sidePairings = rho.getSidePairingsForNormalizedFundamentalDomain();


}
