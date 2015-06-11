#include "h2meshfunctioniterator.h"
#include "h2meshfunction.h"
#include "h2trianglesubdivision.h"

H2MeshFunctionIterator::H2MeshFunctionIterator(const H2MeshFunction * const f) : f(f), mesh(f->mesh)
{
    oldValues = f->values;
    newValues = oldValues;
}

void H2MeshFunctionIterator::iterate(int n)
{
    int i,j, N=n;
    std::vector<H2Point> neighborsImages;
    std::vector<double> weights;

    std::vector<H2Point> testImages;
    std::vector<double> testWeights;
    std::vector<H2Isometry> testIsometriesImage;
    std::vector<int> testNeighborIndices;
    int testIndex=14;

    IsomH2Representation rhoImage = f->rhoImage;
    std::vector<H2Isometry> isometriesImage;
    while (n>0)
    {
        i=0;
        for(const auto & meshPoint : mesh->meshPoints)
        {
            neighborsImages.clear();
            weights = meshPoint->neighborsWeights;
            if (meshPoint->isBoundaryPoint())
            {
                isometriesImage = rhoImage.evaluateRepresentation( ((H2MeshBoundaryPoint*)meshPoint) ->neighborsPairings);
                j=0;
                for(auto k : meshPoint->neighborsIndices)
                {
                    neighborsImages.push_back(isometriesImage[j]*oldValues[k]);
                    if(i==testIndex)
                    {
                        testIsometriesImage.push_back(isometriesImage[j]);
                        testNeighborIndices.push_back(k);
                    }
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
                    if(i==testIndex)
                    {
                        testIsometriesImage.push_back(isometriesImage[j]);
                        testNeighborIndices.push_back(k);
                    }
                    ++j;
                }
            }
            else
            {
                for(auto k : meshPoint->neighborsIndices)
                {
                    neighborsImages.push_back(oldValues[k]);
                    if(i==testIndex)
                    {
                        testNeighborIndices.push_back(k);
                    }
                }
            }
            // This is unique to a weighting regime where the basepoint has a non-zero weight
            neighborsImages.push_back(oldValues[i]);
            weights.push_back(meshPoint->weight);
            if(i==testIndex)
            {
                testImages = neighborsImages;
                testWeights = weights;
            }
            newValues[i] = H2Point::centroid(neighborsImages, weights);
            ++i;
        }
        std::vector<Complex> test;
        for (auto pt : testImages)
        {
            test.push_back(pt.getDiskCoordinate());
        }
        std::cout << "A mesh point has index " << testIndex << std::endl;
        std::cout << "A mesh point is a vertex? " << mesh->meshPoints[testIndex]->isVertexPoint() <<
                     " boundary point? " << mesh->meshPoints[testIndex]->isBoundaryPoint() <<
                     " cut point? " << mesh->meshPoints[testIndex]->isCutPoint() << std::endl;
        std::cout << "A mesh point has neighbors with indices " << testNeighborIndices << std::endl;
        std::cout << "A mesh point has image " << oldValues[testIndex].getDiskCoordinate() << std::endl;
        //std::cout << "This mesh point's neighbor has side pairing " << testIsometriesImage[2] << std::endl;
        std::cout << "A mesh point has neighbor images " << test << std::endl;
        std::vector<double> norms;
        for (auto w : test)
        {
            norms.push_back(norm(w));
        }
        std::cout << "maxnorm = " << *std::max_element(norms.begin(), norms.end()) << std::endl;
        std::cout << "A mesh point has weights " << testWeights << std::endl;
        std::cout << "testWeights.size = test.size? " << (testWeights.size()==test.size()) << std::endl;
        double sum=0;
        for (auto x : testWeights)
        {
            sum += x;
        }
        std::cout << "Sum of weights = " << sum << std::endl;
        std::cout << "A mesh point's averaged value is " << newValues[testIndex].getDiskCoordinate() << std::endl;
        std::cout << "Centroid = " << H2Point::centroid(testImages, testWeights).getDiskCoordinate() << std::endl;
        std::cout << "The error in the " << N-n+1 <<"th iterate is = " << supError() << std::endl;
        oldValues = newValues;
        --n;
    }
}

H2MeshFunction H2MeshFunctionIterator::getOutput()
{
    return H2MeshFunction(mesh, f->rhoImage, oldValues);
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
