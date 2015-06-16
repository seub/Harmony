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
    explicit H2MeshFunction(const H2Mesh *mesh, const IsomH2Representation &rhoImage);

    const std::vector<H2Point> & getValues() const;
    std::vector<H2Triangle> getTrianglesUp() const;

    H2Point getH2Point(int index) const;
    std::vector<H2Point> getH2Neighbors(int index) const;
    std::vector<H2Point> getKickedH2Neighbors(int index) const;
    std::vector<H2Point> getPartnerPoints(int index) const;
    H2Polygon getFundamentalDomain() const;
    H2Triangle getH2Triangle(int index1, int index2, int index3) const;
    bool triangleContaining(const H2Point &point, H2Triangle &outputTriangle, int &meshIndex1, int &meshIndex2, int &meshIndex3) const;
    bool isInteriorPoint(int meshIndex) const;
    std::vector<H2GeodesicArc> getExteriorSides() const;
    std::vector<H2Point> getPoints() const;
    IsomH2Representation getRepresentation() const;

    void initializePL(const H2Point &basePoint);
    void iterate(int n=1);
    void initializePLsmart();

private:
    H2MeshFunction(); // Dummy constructor
    H2MeshFunction(const H2MeshFunction &other); // Copy constructor
    H2MeshFunction & operator=(H2MeshFunction other); // Copy-assignment operator
    explicit H2MeshFunction(const H2Mesh *mesh, const IsomH2Representation &rhoImage, const std::vector<H2Point> &values);
    void refreshSubdivisions();

    const H2Mesh *mesh;
    IsomH2Representation rhoImage;
    std::vector<H2Point> values;
    std::vector<H2TriangleSubdivision> functionSubdivisions;
};

#endif // H2MESHFUNCTION_H
