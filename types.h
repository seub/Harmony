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
typedef std::complex<double> Complex;



class H2Isometry;
template <typename T> class GroupRepresentation;
typedef GroupRepresentation<H2Isometry> IsomH2Representation;

const Complex I(0.0, 1.0);
const double ERROR(0.00000001);

enum drawElementType {POINT, GEODESIC, GEODESICARC, POLYGON, GENERATORAXES, LISTOFAXES};
enum CanvasDelegateType {H2DELEGATE, H3DELEGATE, H2DELEGATEDOMAIN, H2DELEGATETARGET};


enum actionHandlerMessage {HIGHLIGHTED_LEFT, HIGHLIGHTED_RIGHT, HIGHLIGHTED_RESET};


#endif // TYPES_H
