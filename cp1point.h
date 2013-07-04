#ifndef CP1POINT_H
#define CP1POINT_H
#include "types.h"


class SL2Cmatrix;

class CP1point
{
    friend bool operator==(const CP1point &p1, const CP1point &p2);
    friend std::ostream & operator<<(std::ostream & out, const CP1point &p);
    friend CP1point operator *(const SL2Cmatrix & A, const CP1point & z);

public:
    CP1point();
    CP1point(const complex & z);
    CP1point(const complex & z1, const complex &z2);

    bool isInfinity() const;

private:
    complex z1, z2;
};

#endif // CP1POINT_H
