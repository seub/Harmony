#ifndef CP1POINT_H
#define CP1POINT_H
#include "types.h"


class SL2CMatrix;

class CP1Point
{
    friend bool operator==(const CP1Point &p1, const CP1Point &p2);
    friend std::ostream & operator<<(std::ostream & out, const CP1Point &p);
    friend CP1Point operator *(const SL2CMatrix & A, const CP1Point & z);

public:
    CP1Point();
    CP1Point(const complex & z);
    CP1Point(const complex & z1, const complex &z2);

    complex getComplexCoordinate() const;
    bool isInfinity() const;

private:
    complex z1, z2;
};

#endif // CP1POINT_H
