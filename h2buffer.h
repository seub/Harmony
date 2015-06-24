#ifndef H2BUFFER_H
#define H2BUFFER_H


#include <QColor>

#include "tools.h"
#include "h2point.h"
#include "h2geodesic.h"
#include "grouprepresentation.h"
#include "liftedgraph.h"

class H2Mesh; class H2MeshFunction;


class H2Buffer
{
    friend class H2CanvasDelegate;
    friend class H2CanvasDelegateDomain;
    friend class H2CanvasDelegateTarget;
    friend class ActionHandler;
    friend class FenchelNielsenUser;

public:
    H2Buffer(const H2Buffer &) = delete;
    H2Buffer & operator=(const H2Buffer &) = delete;

    bool getIsMeshEmpty() const;
    bool getIsRhoEmpty() const;
    bool getIsFunctionEmpty() const;

private:
    H2Buffer();

    void setIsMeshEmpty(bool isMeshEmpty);
    void setIsFunctionEmpty(bool isFunctionEmpty);
    void setIsRhoEmpty(bool isRhoEmpty);

    void setRhoPointer(const GroupRepresentation<H2Isometry> *rho, const QColor &color);
    void setFunctionPointer(const H2MeshFunction *function, const QColor &color);
    void setMeshPointer(const H2Mesh *mesh, const QColor &color);

    void refreshRho();
    void refreshFunction();
    void refreshMesh();

    void refreshMeshOrFunctionSideTranslates();
    void refreshMeshOrFunctionArcsTranslatesAroundVertex();
    void refreshMeshOrFunctionArcsTranslatesAroundVertices();
    void refreshMeshOrFunctionTranslates(bool aroundVertex, bool aroundVertices);

    const GroupRepresentation<H2Isometry> *rho;
    bool isRhoEmpty;
    std::vector<H2Geodesic> rhoAxes;
    QColor rhoColor;

    std::vector<H2Isometry> translationsAroundVertex, translationsAroundVertices;

    const H2Mesh *mesh;
    bool isMeshEmpty;

    const H2MeshFunction *function;
    bool isFunctionEmpty;

    std::vector<H2GeodesicArc> meshOrFunctionArcs;
    std::vector<H2GeodesicArc> meshOrFunctionSides;
    QColor meshOrFunctionColor;

    std::vector<H2GeodesicArc> meshOrFunctionArcsTranslatesAroundVertex, meshOrFunctionArcsTranslatesAroundVertices;
    std::vector<H2GeodesicArc> meshOrFunctionSidesTranslatesAroundVertex, meshOrFunctionSidesTranslatesAroundVertices;

    H2Triangle triangleHighlighted;
    std::vector<H2Point> pointsHighlightedRed;
    std::vector<H2Point> pointsHighlightedBlue;
    std::vector<H2Point> pointsHighlightedGreen;
};

#endif // H2BUFFER_H
