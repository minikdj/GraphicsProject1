#include "SimplePolygon.h"


SimplePolygon::SimplePolygon(std::vector<dvec3> vertices, const color & material)
    : Plane(vertices, material), vertices(vertices)
{
}

HitRecord SimplePolygon::findClosestIntersection(const Ray & ray)
{
    HitRecord hr = Plane::findClosestIntersection(ray);

    if (!intersectionInsidePolygon(hr.interceptPoint)) {
        hr.t = FLT_MAX;
    }

    return hr;
}

bool SimplePolygon::intersectionInsidePolygon(dvec3 p)
{
    double curResult;

    for (int i = 0; i < vertices.size(); i++) {
        curResult = glm::dot(glm::cross(vertices[(i+1) % vertices.size()] - vertices[i], p - vertices[i]), n);
        if (curResult <= 0) 
        {
            return false;
        }
    }

    return true;
}
