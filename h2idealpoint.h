#ifndef H2IDEALPOINT_H
#define H2IDEALPOINT_H
#include "tools.h"
#include "h2point.h"


class H2IdealPoint
{
public:
    H2IdealPoint();
    H2IdealPoint(const complex & z);


private:
    complex z;
};

#endif // H2IDEALPOINT_H
