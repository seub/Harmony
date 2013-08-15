#ifndef TYPES_H
#define TYPES_H

#define ERROR 0.00000001

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <complex>
#include <cassert>
#include <algorithm>



typedef std::string generatorName;
typedef int generatorIndex;
typedef std::pair<generatorIndex, int> letter;
typedef std::vector<letter> word;
typedef std::complex<double> complex;

class H2Isometry;
template <typename T> class GroupRepresentation;
typedef GroupRepresentation<H2Isometry> IsomH2Representation;


enum drawElementType {POINT, GEODESIC, GEODESICARC, POLYGON, GENERATORAXES, LISTOFAXES};




#endif // TYPES_H
