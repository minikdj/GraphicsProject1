#include "RasterUser.h"

//******** Global Variables ***********

// Frame buffer holding the color values for each pixel
FrameBuffer frameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

// Some predefined colors.
const color LIGHT_BLUE(0.784, 0.784, 1.0, 1.0);

// Raytracer
RayTracer rayTrace(frameBuffer);

// Vector holding all the surfaces in the scene
SurfaceVector surfaces;

// Vector holding all the light sources in the scene
LightVector lights;

shared_ptr<LightSource> ambientLight ;
shared_ptr<PositionalLight> lightPos ;
shared_ptr<DirectionalLight> lightDir;

shared_ptr<Spotlight> spotlight;

// boolean to keep track of it being day or night
bool isNight = false;

/**
* Acts as the display function for the window. 
*/
static void RenderSceneCB()
{
	int frameStartTime = glutGet( GLUT_ELAPSED_TIME ); // Get start time

	// Clear the color buffer

	// Ray trace the scene to determine the color of all the pixels in the scene
	rayTrace.raytraceScene( surfaces, lights);

	// Display the color buffer
	frameBuffer.showColorBuffer();

	// Calculate and display time required to render scene.
	int frameEndTime = glutGet( GLUT_ELAPSED_TIME ); // Get end time
	float totalTimeSec = (frameEndTime - frameStartTime) / 1000.0f;

	std::cout << "Render time: " << totalTimeSec << " sec." << std::endl;

} // end RenderSceneCB


// Reset viewport limits for full window rendering each time the window is resized.
// This function is called when the program starts up and each time the window is 
// resized.
static void ResizeCB(int width, int height)
{
	// Size the color buffer to match the window size.
	frameBuffer.setFrameBufferSize( width, height );

	rayTrace.setCameraFrame( dvec3( 0, 0, 0 ), dvec3( 0, 0, -1 ), dvec3( 0, 1, 0 ) );

	rayTrace.calculatePerspectiveViewingParameters(45.0);

	// Signal the operating system to re-render the window
	glutPostRedisplay();

} // end ResizeCB

void switchTimeOfDay(char c)
{
    ambientLight->enabled = true;
    lightPos->enabled = true;
    lightDir->enabled = true;
    spotlight->enabled = true;

    ambientLight->ambientLightColor = color(0.15, 0.15, 0.15, 1.0);
	lightPos->diffuseLightColor = color(1.0, 1.0, 1.0, 1);
	lightDir->diffuseLightColor = color(0.75, 0.75, 0.75, 1);
    spotlight->diffuseLightColor = color(0.75, 0.75, 0.75, 1.0);
 
    if (c == 'n')
    {
        ambientLight->ambientLightColor =  ambientLight->ambientLightColor * 0.0002;
        lightPos->diffuseLightColor = lightPos->diffuseLightColor * 0.0002;
        lightDir->diffuseLightColor = lightDir->diffuseLightColor * 0.0002;
        spotlight->enabled = false; 
    } 
}

// Responds to 'f' and escape keys. 'f' key allows 
// toggling full screen viewing. Escape key ends the
// program. Allows lights to be individually turned on and off.
static void KeyboardCB(unsigned char key, int x, int y)
{
	switch(key) {

	case('f'): case('F') :
		break;
	case(27): // Escape key
		break;
	case('0') :
		rayTrace.setRecursionDepth( 0 );
		break;
    case('a'):
        ambientLight->enabled = ambientLight->enabled ? false : true;
        break;
    case('p'):
        lightPos->enabled = lightPos->enabled ? false : true;
        break;
    case('d'):
        lightDir->enabled = lightDir->enabled ? false : true;
        break;
    case('s'):
        spotlight->enabled = spotlight->enabled ? false : true;
        break;
    case('m'):
       switchTimeOfDay('m');
       break;
    case('n'):
        switchTimeOfDay('n');
        break;
    case('1') :
        rayTrace.setRecursionDepth( 1 );
        break;
	case('2') :
		rayTrace.setRecursionDepth( 2 );
		break;
	case('3') :
		rayTrace.setRecursionDepth( 3 );
		break;
	case('4') :
		rayTrace.setRecursionDepth( 4 );
		break;
	default:
		std::cout << key << " key pressed." << std::endl;
	}

	glutPostRedisplay();

} // end KeyboardCB


// Responds to presses of the arrow keys
static void SpecialKeysCB(int key, int x, int y)
{
	switch(key) {
	
	case(GLUT_KEY_RIGHT):
		
		break;
	case(GLUT_KEY_LEFT):
		
		break;
	default:
		std::cout << key << " key pressed." << std::endl;
	}

	glutPostRedisplay();

} // end SpecialKeysCB


void buildScene()
{
	// Initialize random seed - used to create random colors
	srand((unsigned int)time(NULL));

    Material redMat(RED);
    redMat.emissiveColor = .03 * RED;

    std::vector<dvec3> polygonVector = {dvec3(2, 0, -10), dvec3(2, 2, -10), dvec3(-2, 2, -10), dvec3(-2, 0, -10)};

    shared_ptr<SimplePolygon> polygon = make_shared<SimplePolygon>(polygonVector, RED);
    shared_ptr<Ellipsoid> ellipsoid = make_shared<Ellipsoid>(dvec3(-3.0, 0.0, -10.0), BLACK, 1, 2, 2);
    shared_ptr<Cylinder> cylinder = make_shared<Cylinder>(dvec3(2.7, 0.8, -10.0), GREEN, 1, 2 );
	shared_ptr<Sphere> redBall = make_shared<Sphere>(dvec3( 0.0, 0.0, -10.0 ), 1.5, RED);
	shared_ptr<Sphere> blueBall = make_shared<Sphere>(dvec3( -2.0, 1.0, -10.0 ), 0.5, BLUE);
	shared_ptr<Sphere> whiteBall = make_shared<Sphere>(dvec3( -1.8, -1.0, -10.0 ), 0.5, WHITE);
	shared_ptr<Plane> plane = make_shared<Plane>(dvec3(0, -20.0, 0.0), dvec3(0, 1, 0), WHITE);
    redBall->material = redMat;    
    

    surfaces.push_back(plane);
    surfaces.push_back(ellipsoid);
	surfaces.push_back(whiteBall);
	surfaces.push_back(blueBall);
	surfaces.push_back(redBall);
    surfaces.push_back(cylinder);
    surfaces.push_back(polygon); 


    ambientLight = make_shared<LightSource>(BLACK);
    ambientLight->ambientLightColor = color(0.15, 0.15, 0.15, 1.0);
	lightPos = make_shared<PositionalLight>(dvec3(-10.0, 10.0, 10.0), color(1.0, 1.0, 1.0, 1));
	lightDir = make_shared<DirectionalLight>(dvec3(-10.0,10.0 ,-10.0), color(0.75, 0.75, 0.75, 1));
    spotlight = make_shared<Spotlight>(dvec3(500, 1000, -10), dvec3(0,-1,0), glm::cos(glm::radians(15.0)), color(0.75, 0.75, 0.75, 1.0));

    lights.push_back(spotlight);
	lights.push_back(lightPos);
	lights.push_back(lightDir);
	lights.push_back(ambientLight);
}


// Register as the "idle" function to have the screen continously
// repainted. Due to software rendering, the frame rate will not
// be fast enough to support motion simulation
static void animate()  
{
	glutPostRedisplay();

} // end animate


int main(int argc, char** argv)
{
	// freeGlut and Window initialization ***********************

    // Pass any applicable command line arguments to GLUT. These arguments
	// are platform dependent.
    glutInit(&argc, argv);

	// Set the initial display mode.
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA );

	// Set the initial window size
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	// Create a window using a string and make it the current window.
	GLuint world_Window = glutCreateWindow("Ray Trace");

	// Indicate to GLUT that the flow of control should return to the program after
	// the window is closed and the GLUTmain loop is exited.
	// glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Request that the window be made full screen
	//glutFullScreenToggle();

	// Set red, green, blue, and alpha to which the color buffer is cleared.
	frameBuffer.setClearColor(color(0,0,0,1));

	// Set the color to which pixels will be cleared if there is no intersection.
    rayTrace.setDefaultColor(LIGHT_BLUE);

	// Callback for window redisplay
	glutDisplayFunc(RenderSceneCB);		
	glutReshapeFunc(ResizeCB);
	glutKeyboardFunc(KeyboardCB);
	glutSpecialFunc(SpecialKeysCB);
	//glutIdleFunc( RenderSceneCB );

	// Create the objects and light sources.
	buildScene();

	// Enter the GLUT main loop. Control will not return until the window is closed.
    glutMainLoop();

	// To keep the console open on shutdown, start the project with Ctrl+F5 instead of just F5.

	return 0;

} // end main
