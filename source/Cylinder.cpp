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

    // if the dot product of n and direction is < 0, we're on the back side of the cylinder and need to invert the normal
   // if (glm::dot(hr.surfaceNormal, ray.direct) < 0) 
   // {
   //   hr.surfaceNormal = (-1.0) * hr.surfaceNormal;
   // }

    float tmp = glm::length(hr.interceptPoint - center);

    if (pow(tmp, 2) - pow(radius, 2) > pow(length / 2, 2)) 
    {
        hr.t = FLT_MAX;
        Ray newRay;
        newRay.origin = hr.interceptPoint + (ray.direct * EPSILON);
        newRay.direct = ray.direct;

        HitRecord newHR = Cylinder::findClosestIntersection(newRay);
        tmp = glm::length(newHR.interceptPoint - center);

        if (pow(tmp, 2) - pow(radius, 2) < pow(length / 2, 2))
        {
            return newHR;
        } 
    }

    return hr;
}
