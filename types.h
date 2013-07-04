#ifndef TYPES_H
#define TYPES_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <complex>
#include <cassert>

typedef std::string generatorName;
typedef std::pair<generatorName, int> letter;
typedef std::vector<letter> word;

typedef std::complex<double> complex;



std::ostream & operator<<(std::ostream &out, const letter & l);
std::ostream & operator<<(std::ostream &out, const word & w);

#endif // TYPES_H
