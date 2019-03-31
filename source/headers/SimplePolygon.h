#include "Plane.h"

class SimplePolygon : public Plane
{
        public:
        
        std::vector<dvec3> vertices;

        SimplePolygon(std::vector<dvec3> vertices, const color & material);
        HitRecord findClosestIntersection(const Ray & ray) override;
        bool intersectionInsidePolygon(dvec3 p);
};
