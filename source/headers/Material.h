#pragma once

#include "Defines.h"


/**
* Simple struct that represents the materials properties of a surface. Material properties
* deterine the color of and object and how it interacts with light sources in a scene.
*/
struct Material
{
	// Ambient color of the surface (usually the same as the diffuseColor).
	color ambientColor;
	
	// Diffuse color of the surface.
	color diffuseColor;

    double shininess = 128.0;
   
    // emissive color of the surface
    color emissiveColor = BLACK;

	// Specular color of the surface (white for a shiny surface).
	color specularColor = WHITE;

	Material( const color & diffuseColor = WHITE )
		: diffuseColor( diffuseColor ), ambientColor( diffuseColor )
	{ }

};
