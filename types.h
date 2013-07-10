#ifndef TYPES_H
#define TYPES_H

#define ERROR 0.0000000001
#define I complex(0.0, 1.0)


template <typename T> class GroupRepresentation;

typedef std::string generatorName;
typedef int generatorIndex;
typedef std::pair<generatorIndex, int> letter;
typedef std::vector<letter> word;

typedef std::complex<double> complex;

class H2Isometry;
typedef GroupRepresentation<H2Isometry> IsomH2Representation;

#endif // TYPES_H
