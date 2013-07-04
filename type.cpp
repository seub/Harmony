#include "types.h"


std::ostream & operator<<(std::ostream &out, const letter & l)
{
    out << l.first;
    if (l.second!=1)
    {
        out << "^" << l.second;
    }
    return out;
}

std::ostream & operator<<(std::ostream &out, const word & w)
{
    unsigned int i;
    for(i=0;i<w.size();i++)
    {
        out << w[i] << " ";
    }
    return out;
}
