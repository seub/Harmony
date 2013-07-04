#include "cp1point.h"

CP1point::CP1point()
{
}

CP1point::CP1point(const complex &z)
{
    z1 = z;
    z2 = 1.0;
}

CP1point::CP1point(const complex &z1, const complex &z2) : z1(z1), z2(z2)
{
}

bool CP1point::isInfinity() const
{
    return z2 == 0.0;
}

bool operator==(const CP1point & p1, const CP1point & p2)
{
    complex test = p1.z1*p2.z2 - p1.z2*p2.z1;
    return test == 0.0;
}

std::ostream & operator<<(std::ostream & out, const CP1point &p)
{
    if (p.z2 == 0.0)
    {
        out << "infinity";
    }
    else
    {
        out << p.z1 / p.z2 ;
    }
    return out;
}
