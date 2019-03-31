#include "Cylinder.h"

Cylinder::Cylinder(const dvec3 & position, const color & mat, double radius, double length)
    : QuadricSurface(position, mat), radius(radius), length(length)
{}

Cylinder::Cylinder(const dvec3 & position, const Material & mat, double radius, double length)
    : QuadricSurface(position, mat), radius(radius), length(length)
{}

HitRecord Cylinder::findClosestIntersection(const Ray & ray) 
{
    HitRecord hr = QuadricSurface::findClosestIntersection(ray);
    // TODO
    return hr;
}
