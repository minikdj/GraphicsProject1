#include "Ray.h"
#include "RayTracer.h"
#include "Surface.h"

HitRecord findIntersection(const Ray & ray, const SurfaceVector & surfaces)
{
    HitRecord closest = HitRecord();
    closest.t = FLT_MAX;
    HitRecord curHR;
    for(auto surface : surfaces) {
        curHR = surface->findClosestIntersection(ray);
        
        if (curHR.t < closest.t) {
            closest = curHR;
        }
    }

    return closest;
}
