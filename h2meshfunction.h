#ifndef H2MESHFUNCTION_H
#define H2MESHFUNCTION_H

#include "tools.h"
#include "h2mesh.h"

class H2MeshFunction
{
    friend class H2MeshFunctionIterator;
    friend class EquivariantHarmonicMapsFactory;
    friend void swap(H2MeshFunction &first, H2MeshFunction &second);
    friend class H2Buffer;

public:
    explicit H2MeshFunction(const H2Mesh *mesh, const GroupRepresentation<H2Isometry> &rhoImage);

    const std::vector<H2Point> & getValues() const;
    std::vector<H2Triangle> getTrianglesUp() const;

    H2Point getH2Point(uint index) const;
    std::vector<H2Point> getH2Neighbors(uint index) const;
    std::vector<H2Point> getKickedH2Neighbors(uint index) const;
    std::vector<H2Point> getPartnerPoints(uint index) const;
    H2Polygon getFundamentalDomain() const;
    H2Triangle getH2Triangle(uint index1, uint index2, uint index3) const;
    bool triangleContaining(const H2Point &point, H2Triangle &outputTriangle, uint &meshIndex1, uint &meshIndex2, uint &meshIndex3) const;
    bool isInteriorPoint(uint meshIndex) const;
    std::vector<H2GeodesicArc> getExteriorSides() const;
    std::vector<H2Point> getBoundary() const;
    std::vector<H2Point> getPoints() const;

    GroupRepresentation<H2Isometry> getTargetRepresentation() const;

    void initializePL(const H2Point &basePoint);
    void iterate(uint n=1);
    void initializePLsmart();

private:
    H2MeshFunction();
    explicit H2MeshFunction(const H2Mesh *mesh, const GroupRepresentation<H2Isometry> &rhoImage, const std::vector<H2Point> &values);
    void refreshSubdivisions();

    const H2Mesh *mesh;
    GroupRepresentation<H2Isometry> rhoImage;
    std::vector<H2Point> values;
    std::vector< TriangularSubdivision<H2Point> > functionSubdivisions;
};

#endif // H2MESHFUNCTION_H
