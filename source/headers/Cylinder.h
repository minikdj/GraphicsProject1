#include "QuadricSurface.h"

class Cylinder : public QuadricSurface
{
    public:
    double radius, length;

    Cylinder(const dvec3 & position, const color & mat, double radius, double length);
    Cylinder(const dvec3 & position, const Material & mat, double radius, double length);
    HitRecord findClosestIntersection(const Ray & ray) override;
};
