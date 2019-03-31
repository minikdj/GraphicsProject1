#include "SimplePolygon.h"


SimplePolygon::SimplePolygon(std::vector<dvec3> vertices, const color & material)
    : Plane(vertices, material)
{
}

bool SimplePolygon::intersectionInsidePolygon(std::vector<dvec3> vertices, HitRecord hr)
{
    double result = glm::dot(glm::cross(vertices[2] - vertices[1], 
                hr.interceptPoint - vertices[1]), glm::normalize(glm::cross(vertices[2] -
                        vertices[1], vertices[0] - vertices[1])));
    return (result > 0);
}
