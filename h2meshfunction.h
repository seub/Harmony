#ifndef H2MESHFUNCTION_H
#define H2MESHFUNCTION_H

#include "tools.h"
#include "h2mesh.h"

class H2MeshFunction
{
    friend class H2MeshFunctionIterator;
    friend class EquivariantHarmonicMapsFactory;
    friend void swap(H2MeshFunction &first, H2MeshFunction &second); // swap function

public:
    explicit H2MeshFunction(const H2Mesh *mesh, const IsomH2Representation *rhoImage);

    const std::vector<H2Point> & getValues() const;
    std::vector<H2Triangle> getTriangles() const;

    void initializePL(const H2Point &basePoint);
    void iterate(int n=1);
    void initializePLsmart();

private:
<<<<<<< HEAD
    H2MeshFunction(); // Dummy constructor
    H2MeshFunction(const H2MeshFunction &other); // Copy constructor
    H2MeshFunction & operator=(H2MeshFunction other); // Copy-assignment operator
    explicit H2MeshFunction(const H2Mesh *mesh, const IsomH2Representation *rhoImage, const std::vector<H2Point> &values);
    const H2Mesh *mesh;
    const IsomH2Representation *rhoImage;
=======
    explicit H2MeshFunction(const H2Mesh * const mesh, const IsomH2Representation * const rhoImage, std::vector<H2Point> values);
    const H2Mesh * const mesh;
    const IsomH2Representation * const rhoImage;
>>>>>>> 91fedd6e4c65254211900beb9463362abc24b3a3
    std::vector<H2Point> values;
};

#endif // H2MESHFUNCTION_H
