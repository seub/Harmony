#include "h2mesh.h"

#include "h2isometry.h"
#include "h2trianglesubdivision.h"
#include "h2polygontriangulater.h"

H2Mesh::H2Mesh(const IsomH2Representation &rho, int depth) : rho(rho), depth(depth)
{
    fundamentalDomain = rho.generatePolygon(200);
    sidePairings = rho.getSidePairingsForNormalizedFundamentalDomain();

    H2PolygonTriangulater PT(&fundamentalDomain);
    std::vector<H2Triangle> triangles = PT.getTriangles();

    H2TriangleSubdivision T;
    std::vector< std::vector<int> > neighbors;
    std::vector<bool> boundary;
    int i, j, N = triangles.size(), M = H2TriangleSubdivision::nbOfPoints(depth), k = 0;
    for (i=0; i<N; i++)
    {
        T = H2TriangleSubdivision(triangles[i], depth);
        subdivisions.push_back(T);
        neighbors = T.neighborsIndices();
        boundary = T.areBoundaryPoints();

        for (i=0; i<M; i++)
        {
            if (!boundary[i])
            {
                H2MeshPoint p;
            }
        }

        k += M;
    }

}
