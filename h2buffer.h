#ifndef H2BUFFER_H
#define H2BUFFER_H


#include <QColor>

#include "tools.h"
#include "h2point.h"
#include "h2geodesic.h"
#include "grouprepresentation.h"
#include "h2mesh.h"
#include "h2meshfunction.h"


class H2Buffer
{
    friend class H2CanvasDelegate;
    friend class H2CanvasDelegateDomain;
    friend class H2CanvasDelegateTarget;
    friend class ActionHandler;

public:
    bool getIsMeshEmpty() const;
    bool getIsRhoEmpty() const;
    bool getIsFunctionEmpty() const;

private:
    H2Buffer();

    void setIsMeshEmpty(bool isMeshEmpty);
    void setIsFunctionEmpty(bool isFunctionEmpty);
    void setIsRhoEmpty(bool isRhoEmpty);

    void setRhoPointer(IsomH2Representation *rho, const QColor &color);
    void setFunctionPointer(H2MeshFunction *function, const QColor &color);
    void setMeshPointer(H2Mesh *mesh, const QColor &color);

    void refreshRho();
    void refreshFunction();
    void refreshMesh();

    void refreshMeshOrFunctionSideTranslates();
    void refreshMeshOrFunctionArcsTranslatesAroundVertex();
    void refreshMeshOrFunctionArcsTranslatesAroundVertices();
    void refreshMeshOrFunctionTranslates(bool aroundVertex, bool aroundVertices);

    IsomH2Representation *rho;
    bool isRhoEmpty;
    std::vector<H2Geodesic> rhoAxes;
    QColor rhoColor;

    std::vector<H2Isometry> translationsAroundVertex, translationsAroundVertices;

    H2Mesh *mesh;
    bool isMeshEmpty;

    H2MeshFunction *function;
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
