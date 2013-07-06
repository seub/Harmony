#ifndef TYPES_H
#define TYPES_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <complex>
#include <cassert>

#define ERROR 0.0000000001
#define I complex(0.0, 1.0)

typedef std::string generatorName;
typedef int generatorIndex;
typedef std::pair<generatorIndex, int> letter;
typedef std::vector<letter> word;

typedef std::complex<double> complex;


std::ostream & operator<<(std::ostream &out, const complex & z);


int int_round(double x);

#endif // TYPES_H
