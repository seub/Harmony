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

bool Tools::twoTriosShareTwoCommonElements(const uint &i1, const uint &i2, const uint &i3,
                                           const uint &j1, const uint &j2, const uint &j3,
                                           uint &sharedInt1, uint &sharedInt2,
                                           uint &unsharedInt1, uint &unsharedInt2)
{
    if (i1==j1 && i2==j2 && i3==j3)
    {
        return false;
    }
    else if (i1==j1)
    {
        if (i2==j2)
        {
            sharedInt1 = i1;
            sharedInt2 = i2;
            unsharedInt1 = i3;
            unsharedInt2 = j3;
            return true;
        }
        else if (i2==j3)
        {
            sharedInt1 = i1;
            sharedInt2 = i2;
            unsharedInt1 = i3;
            unsharedInt2 = j2;
            return true;
        }
        else if (i3==j2)
        {
            sharedInt1 = i1;
            sharedInt2 = i3;
            unsharedInt1 = i2;
            unsharedInt2 = j3;
            return true;
        }
        else if (i3==j3)
        {
            sharedInt1 = i1;
            sharedInt2 = i3;
            unsharedInt1 = i2;
            unsharedInt2 = j2;
            return true;
        }
    }
    else if (i1==j2)
    {
        if (i2==j1)
        {
            sharedInt1 = i1;
            sharedInt2 = i2;
            unsharedInt1 = i3;
            unsharedInt2 = j3;
            return true;
        }
        else if (i2==j3)
        {
            sharedInt1 = i1;
            sharedInt2 = i2;
            unsharedInt1 = i3;
            unsharedInt2 = j1;
            return true;
        }
        else if (i3==j1)
        {
            sharedInt1 = i1;
            sharedInt2 = i3;
            unsharedInt1 = i2;
            unsharedInt2 = j3;
            return true;
        }
        else if (i3==j3)
        {
            sharedInt1 = i1;
            sharedInt2 = i3;
            unsharedInt1 = i2;
            unsharedInt2 = j1;
            return true;
        }
    }
    else if (i1==j3)
    {
        if (i2==j1)
        {
            sharedInt1 = i1;
            sharedInt2 = i2;
            unsharedInt1 = i3;
            unsharedInt2 = j2;
            return true;
        }
        else if (i2==j2)
        {
            sharedInt1 = i1;
            sharedInt2 = i2;
            unsharedInt1 = i3;
            unsharedInt2 = j1;
            return true;
        }
        else if (i3==j1)
        {
            sharedInt1 = i1;
            sharedInt2 = i3;
            unsharedInt1 = i2;
            unsharedInt2 = j2;
            return true;
        }
        else if (i3==j2)
        {
            sharedInt1 = i1;
            sharedInt2 = i3;
            unsharedInt1 = i2;
            unsharedInt2 = j1;
            return true;
        }
    }
    else if (i2==j1)
    {
        if (i3==j2)
        {
            sharedInt1 = i2;
            sharedInt2 = i3;
            unsharedInt1 = i1;
            unsharedInt2 = j3;
            return true;
        }
        else if (i3==j3)
        {
            sharedInt1 = i2;
            sharedInt2 = i3;
            unsharedInt1 = i1;
            unsharedInt2 = j2;
            return true;
        }
    }
    else if (i2==j2)
    {
        if (i3==j1)
        {
            sharedInt1 = i2;
            sharedInt2 = i3;
            unsharedInt1 = i1;
            unsharedInt2 = j3;
            return true;
        }
        else if (i3==j3)
        {
            sharedInt1 = i2;
            sharedInt2 = i3;
            unsharedInt1 = i1;
            unsharedInt2 = j1;
            return true;
        }
    }
    else if (i2==j3)
    {
        if (i3==j1)
        {
            sharedInt1 = i2;
            sharedInt2 = i3;
            unsharedInt1 = i1;
            unsharedInt2 = j2;
            return true;
        }
        else if (i3==j2)
        {
            sharedInt1 = i2;
            sharedInt2 = i3;
            unsharedInt1 = i1;
            unsharedInt2 = j1;
            return true;
        }
    }
    return false;
}
