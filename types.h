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

class SL2RMatrix;
template <typename T> class GroupRepresentation;

typedef std::string generatorName;
typedef int generatorIndex;
typedef std::pair<generatorIndex, int> letter;
typedef std::vector<letter> word;

typedef std::complex<double> complex;

typedef GroupRepresentation<SL2RMatrix> RealRepresentation;


std::ostream & operator<<(std::ostream &out, const complex & z);


int intRound(double x);
double mod2Pi(double t);

#endif // TYPES_H
