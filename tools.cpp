#include "tools.h"

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


std::vector<int> findStringInList(const std::string &s, const std::vector<std::string> & list)
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

bool containsDuplicates(const std::vector<std::string> &list)
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


std::vector< std::pair<int, int> > CommonElements(const std::vector<std::string> & list1, const std::vector<std::string> & list2)
{
    std::vector< std::pair<int, int> > outputPairsOfIndices;
    unsigned int i,j, N1 = list1.size(), N2 = list2.size();
    for (i=0; i<N1; i++)
    {
        for (j=0; j<N2; j++)
        {
            if (list1[i] == list2[j])
            {
                outputPairsOfIndices.push_back(std::pair<int,int>(i, j));
            }
        }
    }
    return outputPairsOfIndices;
}
