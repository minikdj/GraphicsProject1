#include "Plane.h"

class SimplePolygon : public Plane
{
        public:

        SimplePolygon(std::vector<dvec3> vertices, const dvec3 & normal, const color & material);
        friend HitRecord Plane::findClosestIntersection(const Ray & ray);
        bool intersectionInsidePolygon(std::vector<dvec3> vertices, HitRecord hr);
};
