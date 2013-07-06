#ifndef PLANARLINE_H
#define PLANARLINE_H
#include "types.h"

class PlanarLine
{
public:
    PlanarLine();
    PlanarLine(complex p, complex direction);


private:
    complex p, direction;
};

#endif // PLANARLINE_H
