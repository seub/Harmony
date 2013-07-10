#ifndef TOOLS_H
#define TOOLS_H


#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <complex>
#include <cassert>
#include <algorithm>

#include "types.h"

std::ostream & operator<<(std::ostream &out, const complex & z);

namespace Tools
{

int intRound(double x);
double mod2Pi(double t);

std::string convertToString(int i);


std::vector<int> findStringInList(const std::string &s, const std::vector<std::string> & list);
bool containsDuplicates(const std::vector<std::string> & list);
bool haveCommonElements(const std::vector<std::string> & list1, const std::vector<std::string> & list2);
}

#endif // TOOLS_H
