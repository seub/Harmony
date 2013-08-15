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

int Tools::exponentiation(int base, int power)
{
    if (power >= 0)
    {
        int res = 1;
        int pow = base;
        while (power > 0)
        {
            if (power & 1)
            {
                res*=pow;
            }
            pow *= pow;
            power >>= 1;
        }
        return res;
    }
    std::cout << "ERROR in Tools::exponentiation: no negative exponents!" << std::endl;
    throw(0);
}

word Tools::contractWord(word &w)
{
    letter l;
    word result;
    int index = w[0].first;
    int power=0;
    for(int i=0; w.size(); i++)
    {
        if(w[i].first==index)
        {
            power+=w[i].second;
        }
        else
        {
            if (power!=0)
            {
                l.first = index;
                l.second = power;
                result.push_back(l);
            }
            index = w[i].first;
            power = w[i].second;
        }
    }
    return result;
}

std::vector<int> Tools::findStringInList(const std::string &s, const std::vector<std::string> & list)
{
    std::vector<int> outputIndices;
    for (unsigned int i=0; i<list.size(); i++)
    {
        if (list[i] == s)
        {
            outputIndices.push_back(i);
        }
    }
    return outputIndices;
}

bool Tools::containsDuplicates(const std::vector<std::string> &list)
{
    int i,j, N=list.size();
    for (i=0; i<N-1; i++)
    {
        for (j=i+1; j<N; j++)
        {
            if (list[i] == list[j])
            {
                return true;
            }
        }
    }
    return false;
}


bool Tools::haveCommonElements(const std::vector<std::string> & list1, const std::vector<std::string> & list2)
{
    unsigned int i,j, N1 = list1.size(), N2 = list2.size();
    for (i=0; i<N1; i++)
    {
        for (j=0; j<N2; j++)
        {
            if (list1[i] == list2[j])
            {
                return true;
            }
        }
    }
    return false;
}

std::string Tools::convertToString(int i)
{
    std::string s;
    std::stringstream out;
    out << i;
    s = out.str();
    return s;
}

