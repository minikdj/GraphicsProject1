#include "Ellipsoid.h"

Ellipsoid::Ellipsoid(const dvec3 & position, const color & mat, double a, double b, double c)
    : QuadricSurface(position, mat), a(a), b(b), c(c)
{
    // a = 1, b = 2, c = 2, centroid (position) = <0,0,0>
    A = 1/1.0;
    B = 1/4.0;
    C = 1/4.0;
    D = 0;
    E = 0;
    F = 0;
    G = 0;
    H = 0;
    I = 0;
    J = -1;
}


Ellipsoid::Ellipsoid(const dvec3 & position, const Material & mat, double a, double b, double c)
    : QuadricSurface(position, mat), a(a), b(b), c(c)
{
    // a = 1, b = 2, c = 2
    A = 1/1.0;
    B = 1/4.0;
    C = 1/4.0;
    D = 0;
    E = 0;
    F = 0;
    G = 0;
    H = 0;
    I = 0;
    J = -1;
}

