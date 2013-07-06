#include "h2geodesic.h"


H2Geodesic::H2Geodesic()
{    
}

H2Geodesic::H2Geodesic(const double &angle1, const double &angle2) : angle1(angle1), angle2(angle2)
{
}

H2Geodesic::H2Geodesic(const H2point &p1, const H2point &p2)
{
    complex p1Klein, p2Klein;
    p1Klein = p1.getKleinCoordinate();
    p2Klein = p2.getKleinCoordinate();

}
