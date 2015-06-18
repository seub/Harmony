#include "tools.h"

std::ostream & operator<<(std::ostream &out, const Complex & z)
{
    out << real(z) << " + " << imag(z) << "i";
    return out;
}

/*std::string & operator<<(std::ostream &out, const int n)
{
    std::string s;
    out << n;
    s = out.str();
    return s;
}*/

int Tools::intRound(double x)
{
    return (x < 0) ? int(x - 0.5) : int(x + 0.5);
}

double Tools::mod2Pi(double t)
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

double Tools::randDouble(double min, double max)
{
    return min + static_cast <double> (rand()) / ( static_cast <double> (RAND_MAX/(max - min)));
}



std::string Tools::convertToString(int i)
{
    std::string s;
    std::stringstream out;
    out << i;
    s = out.str();
    return s;
}
