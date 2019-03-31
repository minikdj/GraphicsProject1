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

    if (hr.t == FLT_MAX)
    {
        return hr;
    }

    float tmp = glm::length(hr.interceptPoint - center);

    if (pow(tmp, 2) - pow(radius, 2) > pow(length / 2, 2)) 
    {
        hr = QuadricSurface::findClosestIntersection(hr.interceptPoint - 0.0001);
        tmp = glm::length(hr.interceptPoint - center);

        if (pow(tmp, 2) - pow(radius, 2) > pow(length / 2, 2)) 
        {
            hr.t = FLT_MAX;
        }
    }

    return hr;
}
