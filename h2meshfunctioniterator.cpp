#include "h2meshfunctioniterator.h"
#include "h2meshfunction.h"
#include "h2trianglesubdivision.h"

H2MeshFunctionIterator::H2MeshFunctionIterator(const H2MeshFunction * const f) : fInit(f), mesh(f->mesh)
{
    oldValues = f->values;
    newValues = oldValues;
}


void H2MeshFunctionIterator::iterate(int n)
{
    int i,j; //int N =0;
    std::vector<H2Point> neighborsImages;
    std::vector<double> weights;

    IsomH2Representation rhoImage = *(fInit->rhoImage);
    std::vector<H2Isometry> isometriesImage;
    while (n>0)
    {
        i=0;
        for(const auto & meshPoint : mesh->meshPoints)
        {
            weights = meshPoint->neighborsWeights;
            neighborsImages.clear();

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
            else if (meshPoint->isSteinerPoint())
            {
                isometriesImage = rhoImage.evaluateRepresentation( ((H2MeshSteinerPoint*)meshPoint) ->neighborsPairings);
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

            /*// This is unique to a weighting regime where the basepoint has a non-zero weight
            neighborsImages.push_back(oldValues[i]);
            weights.push_back(meshPoint->weight);*/
            newValues[i] = H2Point::centroid(neighborsImages, weights);
            ++i;
        }
        //std::cout << "The error in the " << N-n+1 <<"th iterate is = " << supError() << std::endl;
        oldValues = newValues;
        --n;
    }
}

void H2MeshFunctionIterator::getOutput(H2MeshFunction &output)
{
    output = H2MeshFunction(mesh, fInit->rhoImage, oldValues);
}

double H2MeshFunctionIterator::supError() const
{
    double error;
    std::vector<double> errors;
    for (std::vector<H2Point>::size_type j=0; j<oldValues.size(); ++j)
    {
        error = H2Point::distance(oldValues[j],newValues[j]);
        //std::cout << "error = " << error << std::endl;
        errors.push_back(error);
    }
    return *std::max_element(errors.begin(),errors.end());
}
