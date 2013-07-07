#include "types.h"


std::ostream & operator<<(std::ostream &out, const complex & z)
{
    out << real(z) << " + " << imag(z) << "i";
    return out;
}


int intRound(double x)
{
    return (x < 0) ? int(x - 0.5) : int(x + 0.5);
}

double mod2Pi(double t)
{
    if (t>=0)
    {
        int q= int(t/(2*M_PI));
        return t - 2*M_PI*(q);
    }
    else
    {
        int q= -int(t/(2*M_PI)) + 1;
        return t + 2*M_PI*(q);
    }
}
