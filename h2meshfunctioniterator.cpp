#include "h2meshfunctioniterator.h"
#include "h2meshfunction.h"
#include "h2trianglesubdivision.h"

H2MeshFunctionIterator::H2MeshFunctionIterator(const H2MeshFunction * const f) : f(f), mesh(f->mesh)
{
    oldValues = f->values;
}

void H2MeshFunctionIterator::iterate(int n)
{
    int i,j;
    std::vector<H2Point> neighborsImages;
    std::vector<double> weights;
    IsomH2Representation rhoImage = f->rhoImage;
    std::vector<H2Isometry> isometriesImage;
    while (n>0)
    {
        i=0;
        for(const auto & meshPoint : mesh->meshPoints)
        {
            weights = meshPoint->neighborsWeights;
            if (meshPoint->isBoundaryPoint())
            {
                isometriesImage = rhoImage.evaluateRepresentation( ((H2MeshBoundaryPoint*)meshPoint) ->neighborsPairings);
                j=0;
                for(auto k : meshPoint->neighborsIndices)
                {
                    neighborsImages.push_back(isometriesImage[j]*oldValues[k]);
                    ++j;
                }
            }
            else if (meshPoint->isVertexPoint())
            {
                isometriesImage = rhoImage.evaluateRepresentation( ((H2MeshVertexPoint*)meshPoint) ->neighborsPairings);
                j=0;
                for(auto k : meshPoint->neighborsIndices)
                {
                    neighborsImages.push_back(isometriesImage[j]*oldValues[k]);
                    ++j;
                }
            }
            else
            {
                for(auto k : meshPoint->neighborsIndices)
                {
                    neighborsImages.push_back(oldValues[k]);
                }
            }
            // This is unique to a weighting regime where the basepoint has a non-zero weight
            neighborsImages.push_back(mesh->getH2Point(meshPoint->index));

            newValues[i] = H2Point::centroid(neighborsImages, weights);
            ++i;
        }
        oldValues = newValues;
        --n;
    }
}

H2MeshFunction H2MeshFunctionIterator::getOutput()
{
    return H2MeshFunction(mesh, f->rhoImage, oldValues);
}
