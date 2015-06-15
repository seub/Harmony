#ifndef TOOLS_H
#define TOOLS_H


#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <complex>
#include <cassert>
#include <algorithm>
#include <time.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#include <QDebug>
#include <QString>

#include "types.h"


namespace Tools
{

int intRound(double x);
double mod2Pi(double t);

template <typename T> T exponentiation(T base, int power)
{    
    T res = 1;
    if (power >= 0)
    {
        T pow = base;
        while (power > 0)
        {
            if (power & 1)
            {
                res = res*pow;
            }
            pow = pow*pow;
            power >>= 1;
        }
    }
    else
    {
        std::cerr << "ERROR in Tools::exponentiation: no negative exponents!" << std::endl;
    }
    return res;
}

std::string convertToString(int i);

template <typename T> std::vector<int> findInList(const T &x, const std::vector<T> &V)
{
    std::vector<int> outputIndices;
    for (unsigned int i=0; i<V.size(); i++)
    {
        if (V[i] == x)
        {
            outputIndices.push_back(i);
        }
    }
    return outputIndices;
}

template <typename T> bool isInList(const T &x, const std::vector<T> &V)
{
    for (const auto &v : V)
    {
        if (v == x)
        {
            return true;
        }
    }
    return false;
}

template <typename T> bool containsDuplicates(const std::vector<T> & V)
{
    int i,j, N=V.size();
    for (i=0; i<N-1; i++)
    {
        for (j=i+1; j<N; j++)
        {
            if (V[i] == V[j])
            {
                return true;
            }
        }
    }
    return false;
}

template <typename T> bool haveCommonElements(const std::vector<T> & V1, const std::vector<T> & V2)
{
    unsigned int i, j, N1 = V1.size(), N2 = V2.size();
    for (i=0; i<N1; i++)
    {
        for (j=0; j<N2; j++)
        {
            if (V1[i] == V2[j])
            {
                return true;
            }
        }
    }
    return false;
}

template <typename T> int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}

}

std::ostream & operator<<(std::ostream &out, const Complex &z);

template<typename T> std::ostream & operator<<(std::ostream &out, const std::vector<T> &V)
{
    out << "[";
    for (unsigned int i = 0; i+1 < V.size(); ++i)
    {
        out << V[i] << ", ";
    }
    if (!V.empty())
    {
        out << V.back() <<  "]";
    }
    return out;
}

template<typename T> std::vector<T> operator+(std::vector<T> V, const T &x)
{
    for (auto &v : V)
    {
        v += x;
    }
    return V;
}

template<typename T, typename F> std::vector<T> operator*(const F & f, std::vector<T> V)
{
    for (auto &v : V)
    {
        v = f*v;
    }
    return V;
}

template<typename T, typename F> std::vector<T> operator*(std::vector<T> V, const F & f)
{
    for (auto &v : V)
    {
        v = v*f;
    }
    return V;
}

#endif // TOOLS_H
