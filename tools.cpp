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

word Tools::contractWord(word &w)
{
    letter l;
    word result;
    int index = w[0].first;
    int power=0;
    for(auto x : w)
    {
        if(x.first==index)
        {
            power+=x.second;
        }
        else
        {
            if (power!=0)
            {
                l.first = index;
                l.second = power;
                result.push_back(l);
            }
            index = x.first;
            power = x.second;
        }
    }
    return result;
}

std::string Tools::convertToString(int i)
{
    std::string s;
    std::stringstream out;
    out << i;
    s = out.str();
    return s;
}

