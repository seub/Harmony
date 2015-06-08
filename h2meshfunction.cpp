#include "h2meshfunction.h"

H2MeshFunction::H2MeshFunction(const H2Mesh *const mesh, const IsomH2Representation &rhoImage) : mesh(mesh), rhoImage(rhoImage)
{
}

void H2MeshFunction::initializePL(const H2Point &basePoint)
{
    /*int nbVertices = mesh->fundamentalDomain.nbVertices();
    std::vector<H2Point> verticesImages(nbVertices);

    std::vector<Word> wordSidePairings = mesh->rho.getWordSidePairings();

    int i, genus = nbVertices/4;
    std::vector<Word> neighborsWordIsometriesTemp;
    Word f;
    std::vector<int> neighborsTemp;
    for (i=0; i<genus; ++i)
    {
        // choppe les voisins de 4i

        f = f*wordSidePairings[4*i].inverse();
        // choppe les voisins de sommet[4i+3]
        f = f*wordSidePairings[4*i+3].inverse();
        // choppe les voisins de sommet[4i+2]
        f= f*wordSidePairings[4*i+2].inverse();
        // choppe les voisins de 4i+1
        f = f*wordSidePairings[4*i+1].inverse();
    }



    int nbSubdivisions = mesh->subdivisions.size();
    std::vector<H2TriangleSubdivision> subdivisions(nbSubdivisions);*/
}
