#pragma once

#include "Defines.h"
#include "HitRecord.h"
#include "Surface.h"
#include "Ray.h"

HitRecord findIntersection( const Ray & ray, const SurfaceVector & surfaces );


/**
* Base struct for all types of lights. Supports only specification of the 
* color and intensity of light sources. Parent of sub-structs that simulate
* positional, directional, and spot lights.
*
* Instances of this struct an be used to simulate only ambient light. Ambient
* light is described as bounced light that has been scattered so much that it
* is impossible to tell the direction to its source. If using a LightSource 
* instantiation to simulate ambient light, the overall intensity of the light
* should be low in order to avoid washing out shadows as well as diffuse and 
* specular effects that can be achieved when using children of this struct.
*/
struct LightSource
{
    bool enabled = true;
    
	LightSource(const color & lightColor) 
	: diffuseLightColor(lightColor)
	{
		ambientLightColor = 0.15 * diffuseLightColor;
		specularLightColor = WHITE;
	}

	virtual color illuminate(const dvec3 & eyeVector, HitRecord & closestHit, const SurfaceVector & surfaces)
	{
        if (enabled) {
            return closestHit.material.ambientColor * ambientLightColor;
        }
        return BLACK;
	}

	/*
	* Ambient color and intensity of the light.
	*/
	color ambientLightColor;

	/* 
	* Diffuse color and intensity of the light.
	*/
	color diffuseLightColor; 

	/*
	 * Specular color and intensity of the light.
	 */
	color specularLightColor;
};

/**
* Struct for simulating light sources that have an explicit position and 
* shine equally in all directions. Instantiations of the struct will have
* a position property and a color and intensity of the light given off
* by the light.
*/
struct PositionalLight : public LightSource
{
	PositionalLight(glm::dvec3 position, const color & lightColor)
	: LightSource(lightColor), lightPosition(position)
	{}

	virtual color illuminate(const glm::dvec3 & eyeVector, HitRecord & closestHit, const SurfaceVector & surfaces)
	{
        color totalLight = BLACK;
        if(enabled) {

            dvec3 lightDirection = (lightPosition - closestHit.interceptPoint) 
                                / glm::length(lightPosition - closestHit.interceptPoint);
            dvec3 reflectionVec = glm::normalize(glm::reflect(lightDirection, closestHit.surfaceNormal));

            Ray shadow(closestHit.interceptPoint + (EPSILON * closestHit.surfaceNormal), (lightDirection));
            HitRecord hr = findIntersection(shadow, surfaces);
            if (hr.t == FLT_MAX){
                totalLight += glm::max(glm::dot(lightDirection, closestHit.surfaceNormal), 0.0) *
                          diffuseLightColor * closestHit.material.diffuseColor;
                totalLight += glm::pow(glm::max(0.0, glm::dot(reflectionVec, eyeVector)),
                         closestHit.material.shininess) * specularLightColor * closestHit.material.specularColor;
                totalLight += LightSource::illuminate(eyeVector, closestHit, surfaces);
            } 
            return totalLight;
        }
        return totalLight;
	}


	/**
	* x, y, z position of the light source. 
	*/
	glm::dvec3 lightPosition; 
};

/**
* Struct for simulating light sources that do not have an explicit position.
* Such light sources have only a direction against which they are shinning.
* Instantiations of the struct will have this direction properties along with
* a color and intensity of the light given off by the light source.
*/
struct DirectionalLight : public LightSource
{
	DirectionalLight(dvec3 direction, const color & lightColor)
	: LightSource(lightColor), lightDirection(glm::normalize(direction))
	{}

	virtual color illuminate(const dvec3 & eyeVector, HitRecord & closestHit, const SurfaceVector & surfaces)
	{
        if (enabled) {
            color totalLight = closestHit.material.emissiveColor;
            dvec3 reflectionVec = glm::normalize(glm::reflect(lightDirection, closestHit.surfaceNormal));
            
            Ray shadow(closestHit.interceptPoint + (EPSILON * closestHit.surfaceNormal), (lightDirection));
            HitRecord hr = findIntersection(shadow, surfaces);
            if (hr.t == FLT_MAX){

                //ambient
                totalLight += (LightSource::illuminate(eyeVector, closestHit, surfaces));

                //diffuse
                totalLight += glm::max(glm::dot(lightDirection, closestHit.surfaceNormal), 0.0) *
                          diffuseLightColor * closestHit.material.diffuseColor;

                // specular color
                totalLight += glm::pow(glm::max(0.0, glm::dot(reflectionVec, eyeVector)),
                         closestHit.material.shininess) * specularLightColor * closestHit.material.specularColor;
            }  
            return totalLight;
        }
        return BLACK;
	}

	/**
	* Unit vector that points in the direction that is opposite 
	* the direction in which the light is shining.
	*/
	glm::dvec3 lightDirection; 
};

/**
* Struct for simulating light sources that have an explicit position
* and shine in a specified direction.Width of the associated beam of
* light is controlled using a spot cutoff cosine. Instantiations 
* of the struct will have position and direction properties along with
* a color and intensity of the light given off by the light source.
*/
struct Spotlight: public PositionalLight {
    // unit vector that points in the direction 
    // that the light is shining
    dvec3 spotDirection;

    //angle in radians of half the spot light beam;
    double cutOffCosineRadians;

    Spotlight(dvec3 position, dvec3 direction, double cutOffCosineRadians, const color & colorOfLight ): 
              PositionalLight(position, colorOfLight), spotDirection(glm::normalize(direction)),
              cutOffCosineRadians(glm::radians(cutOffCosineRadians)) {}

    virtual color illuminate(const glm::dvec3& eyeVector,HitRecord& closestHit,const SurfaceVector& surfaces) {

        dvec3 lightDirection = (PositionalLight::lightPosition - closestHit.interceptPoint) 
                           / glm::length(lightPosition - closestHit.interceptPoint);
        
        double spotCosine = glm::dot(-lightDirection, spotDirection);

        if(spotCosine > cutOffCosineRadians) {
            double falloffFactor = (1-(1-spotCosine)) / (1-cutOffCosineRadians);
            return falloffFactor * PositionalLight::illuminate(eyeVector, closestHit, surfaces);
        }

        return BLACK; 
    }
};


