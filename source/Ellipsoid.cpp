#include "Ellipsoid.h"

    // Ellipsoid
    // a = 2, b = 3, c = 3, centroid (position) = <0,0,0>
    // A = 1/4.0;
    // B = 1/9.0;
    // C = 1/9.0;
    // D = 0;
    // E = 0;
    // F = 0;
    // G = 0;
    // H = 0;
    // I = 0;
    // J = -1;

Ellipsoid::Ellipsoid(const dvec3 & position, const color & mat, double a, double b, double c)
    : QuadricSurface(position, mat), a(a), b(b), c(c)
{}

Ellipsoid::Ellipsoid(const dvec3 & position, const Material & mat, double a, double b, double c)
    : QuadricSurface(position, mat), a(a), b(b), c(c)
{}

HitRecord Ellipsoid::findClosestIntersection(const Ray & ray)
{
    HitRecord hr = QuadricSurface::findClosestIntersection(ray);
    // TODO
    return hr;
}
