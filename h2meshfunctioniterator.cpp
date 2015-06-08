#include "h2meshfunctioniterator.h"
#include "h2meshfunction.h"

H2MeshFunctionIterator::H2MeshFunctionIterator(const H2MeshFunction * const f) : f(f), mesh(f->mesh)
{
    oldValues = f->values;
}

void H2MeshFunctionIterator::iterate(int n)
{
    int i=0;
    std::vector<H2Point> neighborsImages;
    std::vector<double> weights;
    for(const auto & meshPoint : mesh->meshPoints)
    {
        for(auto k : meshPoint->neighborsIndices)
        {
            neighborsImages.push_back(oldValues[k]);
        }
        //newValues[i]
    }


}
