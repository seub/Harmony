#ifndef H2PENTAGON_H
#define H2PENTAGON_H
#include "types.h"
#include "h2point.h"
#include "h2isometry.h"

class H2Triangle;

class H2OneHoledTorus
{
public:
    H2OneHoledTorus();
    H2OneHoledTorus(std::vector<H2Point> & vertices, H2Isometry & pair13, H2Isometry & pair24);

    void getVertices(const std::vector<H2Point> & vertices) const;
    void getAngles(std::vector<double> &angles);

    std::vector<H2Triangle> triangulate() const;

private:
    std::vector<H2Point> vertices;
    H2Isometry pair13, pair24;
};

#endif // H2PENTAGON_H
