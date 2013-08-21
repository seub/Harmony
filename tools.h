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

#include "types.h"


namespace Tools
{

int intRound(double x);
double mod2Pi(double t);
int exponentiation(int base, int power);

word contractWord(word & w);
std::string convertToString(int i);
std::vector<int> findStringInList(const std::string &s, const std::vector<std::string> & list);
bool containsDuplicates(const std::vector<std::string> & list);
bool haveCommonElements(const std::vector<std::string> & list1, const std::vector<std::string> & list2);

}

std::ostream & operator<<(std::ostream &out, const Complex &z);
template<typename T> std::ostream & operator<<(std::ostream &out, const std::vector<T> &V)
{
    out << "[";
    for (auto it = V.begin(); it + 1 != V.end(); ++it)
    {
        out << *it << ", ";
    }
    if (!V.empty())
    {
        out << V.back() <<  "]";
    }
    return out;
}
template<typename T> std::vector<T> operator+(std::vector<T> V, T &x)
{
    for (auto &v : V)
    {
        v += x;
    }
    return V;
}

#endif // TOOLS_H
