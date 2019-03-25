#include "Plane.h"

/**
* Constructor for the Plane.
*/
Plane::Plane(const dvec3 & point, const dvec3 & normal, const color & material)
	: Surface(material), a(point), n(normalize(normal))
{
}

Plane::Plane(std::vector<dvec3> vertices, const color & material)
	: Surface(material)
{
	a = vertices[0];

	n = glm::normalize(glm::cross(vertices[2] - vertices[1], vertices[0] - vertices[1]));
}

SimplePolygon::SimplePolygon(std::vector<dvec3> vertices, const color & material)
    : Plane(vertices, material)
{
}

/*
 * returns true if the point in hr is left of the ray going form v_n to v_n+1
 */
bool SimplePolygon::intersectionInsidePolygon(std::vector<dvec3> vertices, HitRecord hr)
{
    double result = glm::dot(glm::cross(vertices[2] - vertices[1], 
                hr.interceptPoint - vertices[1]), glm::normalize(glm::cross(vertices[2] -
                        vertices[1], vertices[0] - vertices[1])));
    return (result > 0);
}

/*
* Checks a ray for intersection with the surface. Finds the closest point of intersection
* if one exits. Returns a HitRecord with the t parmeter set to FLT_MAX if there is no
* intersection.
*/
HitRecord Plane::findClosestIntersection( const Ray & ray )
{
	HitRecord hitRecord;

    if (glm::dot(ray.direct, n) == 0) return hitRecord;
    
    hitRecord.t = glm::dot(a - ray.origin, n) / glm::dot(ray.direct, n);

    if (hitRecord.t < 0) hitRecord.t = FLT_MAX;
    
    hitRecord.interceptPoint = ray.origin + hitRecord.t * ray.direct;
    hitRecord.material = material;
	
    return hitRecord;

} // end findClosestIntersection

