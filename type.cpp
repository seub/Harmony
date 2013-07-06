#include "types.h"


std::ostream & operator<<(std::ostream &out, const complex & z)
{
    out << real(z) << " + " << imag(z) << "i";
    return out;
}


int int_round(double x)
{
    return (x < 0) ? int(x - 0.5) : int(x + 0.5);
}
