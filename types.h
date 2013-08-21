#ifndef TYPES_H
#define TYPES_H

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
typedef std::complex<double> Complex;



class H2Isometry;
template <typename T> class GroupRepresentation;
typedef GroupRepresentation<H2Isometry> IsomH2Representation;

const Complex I(0.0, 1.0);
const double ERROR(0.00000001);

enum drawElementType {POINT, GEODESIC, GEODESICARC, POLYGON, GENERATORAXES, LISTOFAXES};




#endif // TYPES_H
