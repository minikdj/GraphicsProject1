#include "RayTracer.h"


RayTracer::RayTracer(FrameBuffer & cBuffer, color defaultColor )
:colorBuffer(cBuffer), defaultColor(defaultColor), recursionDepth(2)
{
    	
}


void RayTracer::setCameraFrame(const dvec3 & viewPosition, const dvec3 & viewingDirection, dvec3 up)
{

//    eye = viewPosition;

    w = glm::normalize(-viewingDirection);
    u = glm::normalize(glm::cross(up, w));
    v = glm::normalize(glm::cross(w, u));
} // end setCameraFrame


void RayTracer::calculatePerspectiveViewingParameters(const double & verticalFieldOfViewDegrees)
{

    double verticalFOV = glm::radians(verticalFieldOfViewDegrees / 2.0);
    
    distToPlane = 1 / glm::tan(verticalFOV );
    topLimit = distToPlane * tan(verticalFOV );
	rightLimit = topLimit * ((double)colorBuffer.getWindowWidth()/colorBuffer.getWindowHeight());
    leftLimit = -rightLimit;
    bottomLimit = -topLimit;
	nx = (double)colorBuffer.getWindowWidth();
	ny = (double)colorBuffer.getWindowHeight();
	renderPerspectiveView = true; // generate perspective view rays
	
} // end calculatePerspectiveViewingParameters


void RayTracer::calculateOrthographicViewingParameters(const double & viewPlaneHeight)
{
	topLimit = fabs(viewPlaneHeight) / 2.0;

	rightLimit = topLimit * ((double)colorBuffer.getWindowWidth()/colorBuffer.getWindowHeight()); // Set r based on aspect ratio and height of plane

	// Make view plane symetrical about the viewing direction
	leftLimit = -rightLimit; 
	bottomLimit = -topLimit;

	// Calculate the distance between pixels in the horizontal and vertical directions
	nx = (double)colorBuffer.getWindowWidth();
	ny = (double)colorBuffer.getWindowHeight();

	distToPlane = 0.0; // Rays start on the view plane

	renderPerspectiveView = false; // generate orthographic view rays
	
} // end calculateOrthographicViewingParameters


void RayTracer::raytraceScene(const SurfaceVector & surfaces, const LightVector & lights)
{
	this->surfacesInScene = surfaces;
	this->lightsInScene = lights;

	// Iterate through each and every pixel in the rendering window
    
    for(int i = 0; i < colorBuffer.getWindowWidth(); i ++) {
        for(int j = 0; j < colorBuffer.getWindowHeight(); j++) {
            Ray ray;
            renderPerspectiveView == true ? ray = getPerspectiveViewRay(i, j) : ray = getOrthoViewRay(i, j); 
            colorBuffer.setPixel(i, j, traceIndividualRay(ray, recursionDepth));
        }
    }
} // end raytraceScene



color RayTracer::traceIndividualRay(const Ray & viewRay, int recursionLevel)
{
    if (recursionLevel <= 0) {
        return BLACK;
    }
    HitRecord closest = HitRecord();
    closest = findIntersection(viewRay, surfacesInScene);

    if (closest.t < FLT_MAX) {
        color total = BLACK;
        Ray reflectRay = Ray(closest.interceptPoint + (EPSILON * closest.surfaceNormal), 
                glm::reflect(viewRay.direct, closest.surfaceNormal)); 
        total += 0.3 * RayTracer::traceIndividualRay(reflectRay, recursionLevel - 1);
 
        for (auto light : lightsInScene) {
            total += light->illuminate(viewRay.direct, closest, surfacesInScene);
            total += closest.material.emissiveColor;
        }
       return total;
    }
    return (closest.t != FLT_MAX) ? closest.material.diffuseColor : defaultColor; 

} // end traceRay


Ray RayTracer::getOrthoViewRay( const int x, const int y)
{
	Ray orthoViewRay;

	dvec2 uv = getImagePlaneCoordinates(x, y);
	
	orthoViewRay.origin = eye + uv.x * u + uv.y * v;
	orthoViewRay.direct = glm::normalize( -w );

	return orthoViewRay;

} // end getOrthoViewRay


Ray RayTracer::getPerspectiveViewRay(const int x, const int y)
{
	Ray perspectiveViewRay;
    perspectiveViewRay.origin = eye;

    dvec2 coords = getImagePlaneCoordinates(x, y);
    dvec3 numerator = (-distToPlane * w) + (coords.x * u) + (coords.y * v);
    perspectiveViewRay.direct = normalize(numerator);

	return perspectiveViewRay;

} // end getPerspectiveViewRay


dvec2 RayTracer::getImagePlaneCoordinates(const int x, const int y)
{
    double ux = leftLimit + (rightLimit - leftLimit) * ((x + 0.5) / nx);
    double vx = bottomLimit + (topLimit - bottomLimit) * ((y + 0.5) / ny);

	return dvec2(ux, vx);
}





