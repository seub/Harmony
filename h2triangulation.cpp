#include "h2triangulation.h"

H2Triangulation::H2Triangulation(const H2Point & a, const H2Point & b, const H2Point & c, int depth) : depth(depth)
{
    this->a = new H2Point();
    this->b = new H2Point();
    this->c = new H2Point();
    *(this->a) = a;
    *(this->b) = b;
    *(this->c) = c;

}
