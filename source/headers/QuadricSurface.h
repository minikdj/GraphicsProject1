#pragma once
#include "Surface.h"

/*

Super class to support intersection testing with quadric surfaces. These shapes can be
described by the general quadric surface equation 

Ax2 + By2 + Cz2 + Dxy+ Exz + Fyz + Gx + Hy + Iz + J = 0

*/
class QuadricSurface : 	public Surface
{
public:

	/**
	* Constructor for qudric surface.
	* @param - position: specifies an xyz position of the center of the surface
	* @param - mat: diffuse color of the surface.
	*/
	QuadricSurface(const dvec3 & position, const color & mat);

	/**
	* Constructor for qudric surface.
	* @param - position: specifies an xyz position of the center of the surface
	* @param - mat: material properties of the surface.
	*/
	QuadricSurface( const dvec3 & position, const Material & mat );

	/**
	* Checks a ray for intersection with the surface. Finds the closest point of intersection
	* if one exits. Returns a HitRecord with the t parmeter set to FLT_MAX if there is no
	* intersection.
	* @param rayOrigin - Origin of the ray being check for intersetion
	* @param rayDirection - Unit vector represention the direction of the ray.
	* returns HitRecord containing intormation about the point of intersection.
	*/
	virtual HitRecord findClosestIntersection( const Ray & ray );

	/**
	* xyz location of the center of the surface
	*/
	dvec3 center;

	protected:

	/**
	* Coeficients is the  quadric surface equation
	* Ax2 + By2 + Cz2 + Dxy+ Exz + Fyz + Gx + Hy + Iz + J = 0
	*/
	double A, B, C, D, E, F, G, H, I, J;

};

class Ellipsoid : public QuadricSurface
{
    public:
    // a, b, and c for the equation of an ellipsoid
    double a, b, c;

    Ellipsoid(const dvec3 & position, const color & mat, double a, double b, double c);
    Ellipsoid(const dvec3 & position, const Material & mat, double a, double b, double c);
    HitRecord findClosestIntersection(const Ray & ray) override;
};

class Cylinder : public QuadricSurface
{
    public:
    double radius, length;

    Cylinder(const dvec3 & position, const color & mat, double radius, double length);
    Cylinder(const dvec3 & position, const Material & mat, double radius, double length);
    HitRecord findClosestIntersection(const Ray & ray) override;
};

