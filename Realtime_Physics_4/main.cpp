
#include <time.h>
#include <common/Shader.h>
#include <common/Utilities.h>
#include <common/EulerCamera.h>
#include <common/Defines.h>
#include <GLM.h>
#include "glm/ext.hpp"
#include <common/text.h>
#include <common/Collision.h>
#include <common/RigidBodySystem.h>

using namespace std;

const float width = 900, height = 900;
/*----------------------------------------------------------------------------
						MESH AND TEXTURE VARIABLES
----------------------------------------------------------------------------*/

SingleMesh skyBox, cube, sphere;

/*----------------------------------------------------------------------------
							CAMERA VARIABLES
----------------------------------------------------------------------------*/
glm::vec3 startingPos = { 0.0f, 0.0f, -20.0f };
GLfloat pitCam = 0, yawCam = 0, rolCam = 0, frontCam = 0, sideCam = 0, speed = 1;
float rotateY = 50.0f, rotateLight = 0.0f;

EulerCamera cam(startingPos, width, height);

/*----------------------------------------------------------------------------
								SHADER VARIABLES
----------------------------------------------------------------------------*/
GLuint simpleShaderID, noTextureShaderID, cubeMapShaderID;
Shader shaderFactory;
/*----------------------------------------------------------------------------
							OTHER VARIABLES
----------------------------------------------------------------------------*/

const char* atlas_image = "../freemono.png";
const char* atlas_meta = "../freemono.meta";

float fontSize = 25.0f;
int textID = -1;
bool pause = false;

RigidBodySystem bodySystem;

GLfloat vertices[] = {
	-10.0f, -10.0f, -10.0f,  0.0f,  0.0f, -1.0f,
	10.0f, -10.0f, -10.0f,  0.0f,  0.0f, -1.0f,
	10.0f,  10.0f, -10.0f,  0.0f,  0.0f, -1.0f,
	10.0f,  10.0f, -10.0f,  0.0f,  0.0f, -1.0f,
	-10.0f,  10.0f, -10.0f,  0.0f,  0.0f, -1.0f,
	-10.0f, -10.0f, -10.0f,  0.0f,  0.0f, -1.0f,

	-10.0f, -10.0f,  10.0f,  0.0f,  0.0f,  1.0f,
	10.0f, -10.0f,  10.0f,  0.0f,  0.0f,  1.0f,
	10.0f,  10.0f,  10.0f,  0.0f,  0.0f,  1.0f,
	10.0f,  10.0f,  10.0f,  0.0f,  0.0f,  1.0f,
	-10.0f,  10.0f,  10.0f,  0.0f,  0.0f,  1.0f,
	-10.0f, -10.0f,  10.0f,  0.0f,  0.0f,  1.0f,

	-10.0f,  10.0f,  10.0f, -1.0f,  0.0f,  0.0f,
	-10.0f,  10.0f, -10.0f, -1.0f,  0.0f,  0.0f,
	-10.0f, -10.0f, -10.0f, -1.0f,  0.0f,  0.0f,
	-10.0f, -10.0f, -10.0f, -1.0f,  0.0f,  0.0f,
	-10.0f, -10.0f,  10.0f, -1.0f,  0.0f,  0.0f,
	-10.0f,  10.0f,  10.0f, -1.0f,  0.0f,  0.0f,

	10.0f,  10.0f,  10.0f,  1.0f,  0.0f,  0.0f,
	10.0f,  10.0f, -10.0f,  1.0f,  0.0f,  0.0f,
	10.0f, -10.0f, -10.0f,  1.0f,  0.0f,  0.0f,
	10.0f, -10.0f, -10.0f,  1.0f,  0.0f,  0.0f,
	10.0f, -10.0f,  10.0f,  1.0f,  0.0f,  0.0f,
	10.0f,  10.0f,  10.0f,  1.0f,  0.0f,  0.0f,

	-10.0f, -10.0f, -10.0f,  0.0f, -1.0f,  0.0f,
	10.0f, -10.0f, -10.0f,  0.0f, -1.0f,  0.0f,
	10.0f, -10.0f,  10.0f,  0.0f, -1.0f,  0.0f,
	10.0f, -10.0f,  10.0f,  0.0f, -1.0f,  0.0f,
	-10.0f, -10.0f,  10.0f,  0.0f, -1.0f,  0.0f,
	-10.0f, -10.0f, -10.0f,  0.0f, -1.0f,  0.0f,

	-10.0f,  10.0f, -10.0f,  0.0f,  1.0f,  0.0f,
	10.0f,  10.0f, -10.0f,  0.0f,  1.0f,  0.0f,
	10.0f,  10.0f,  10.0f,  0.0f,  1.0f,  0.0f,
	10.0f,  10.0f,  10.0f,  0.0f,  1.0f,  0.0f,
	-10.0f,  10.0f,  10.0f,  0.0f,  1.0f,  0.0f,
	-10.0f,  10.0f, -10.0f,  0.0f,  1.0f,  0.0f
};
/*----------------------------------------------------------------------------
						FUNCTION DEFINITIONS
----------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/

void init()
{
	if (!init_text_rendering(atlas_image, atlas_meta, width, height)) 
	{
		fprintf(stderr, "ERROR init text rendering\n");
		exit(1);
	}
	simpleShaderID = shaderFactory.CompileShader(SIMPLE_VERT, SIMPLE_FRAG);
	noTextureShaderID = shaderFactory.CompileShader(NOTEXTURE_VERT, NOTEXTURE_FRAG);
	cubeMapShaderID = shaderFactory.CompileShader(SKY_VERT, SKY_FRAG);

	skyBox.initCubeMap(vertices, 36, "desert");
	cube.init(CUBE_MESH, NULL, NULL);
	cube.mode = GL_QUADS;
	sphere.init(SPHERE_MESH, NULL, NULL);

	bodySystem = RigidBodySystem(10, cube, sphere);
}

void display() 
{
	glEnable(GL_DEPTH_TEST);								// enable depth-testing
	glDepthFunc(GL_LESS);									// depth-testing interprets a smaller value as "closer"
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear the color and buffer bits to make a clean slate
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);					//Create a background	

	LightStruct lightStruct;
	glm::mat4 model;
	// light properties
	//vec3 Ls = vec3(0.06f, 0.03f, 0.06f);	//Specular Reflected Light
	//vec3 Ld = vec3(0.8f, 0.8f, 0.8f);	//Diffuse Surface Reflectance
	//vec3 La = vec3(0.8f, 0.8f, 0.8f);	//Ambient Reflected Light
	//vec3 light = vec3(1.8*cos(rotateLight), 1.8*sin(rotateLight) + 1.0f, -5.0f);//light source location
	//vec3 coneDirection = light + vec3(0.0f, -1.0f, 0.0f);
	//float coneAngle = 40.0f;
	//// object colour
	//vec3 Ks = vec3(0.01f, 0.01f, 0.01f); // specular reflectance
	//vec3 Kd = BLUE;
	//vec3 Ka = BLUE; // ambient reflectance
	//float specular_exponent = 10.0f; //specular exponent - size of the specular elements

	model = glm::mat4();
	drawCubeMap(cubeMapShaderID, cam, skyBox, lightStruct);

	for (int i = 0; i < bodySystem.numBodies; i++)
	{
		lightStruct.Ka = bodySystem.bodies[i].colour;
		drawObject(noTextureShaderID, cam, bodySystem.bodies[i].mesh, model, false, lightStruct);
		model = glm::translate(glm::mat4(), bodySystem.bodies[i].position);
		if (bodySystem.bodies[i].colour == RED)
		{
			lightStruct.Ka = bodySystem.bodies[i].colour;
			drawObject(noTextureShaderID, cam, bodySystem.bodies[i].boundingSphere, model, false, lightStruct);
		}
	}
	draw_texts();
	glutSwapBuffers();
}

void updateScene() 
{
	static clock_t lastFrame = clock();
	clock_t currFrame = clock();
	float delta = (currFrame - lastFrame) / (float)CLOCKS_PER_SEC;
	if (delta >= 0.03f) 
	{
		delta = 0.03f;
		lastFrame = currFrame;
		glutPostRedisplay();

		cam.movForward(frontCam*speed);
		cam.movRight(sideCam*speed);
		cam.changeFront(pitCam, yawCam, rolCam);
		update_text(textID, "Paused");
		if (!pause)
		{
			string output = "EULER_MODE: Front: " + glm::to_string(cam.getFront()) + "\n";
			output += "Position: " + glm::to_string(cam.getPosition()) + "\n";
			output += "Up: " + glm::to_string(cam.getUp()) + "\n";
			output += "Pitch: " + glm::to_string(cam.getPitch()) + "\n";
			output += "Yaw: " + glm::to_string(cam.getYaw()) + "\n";
			output += "Roll: " + glm::to_string(cam.getRoll()) + "\n";
			update_text(textID, output.c_str());

			bodySystem.applyForces(delta);
			bodySystem.checkSphericalCollisions();
			bodySystem.checkPlaneCollisions(glm::vec3(5.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 0.0), delta);
			bodySystem.checkPlaneCollisions(glm::vec3(-5.0, 0.0, 0.0), glm::vec3(-1.0, 1.0, 0.0), delta);
			bodySystem.checkPlaneCollisions(glm::vec3(5.0, 20.0, 0.0), glm::vec3(1.0, -1.0, 0.0), delta);
			bodySystem.checkPlaneCollisions(glm::vec3(-5.0, 20.0, 0.0), glm::vec3(-1.0, -1.0, 0.0), delta);

		}
	}
	
}

#pragma region INPUT FUNCTIONS

void keypress(unsigned char key, int x, int y) {
	switch (key)
	{
	case ((char)27):
		exit(0);
		break;
	case('w'):
	case('W'):
		frontCam = 1;
		std::printf("Moving Forward\n");
		break;
	case('s'):
	case('S'):
		frontCam = -1;
		std::printf("Moving Backward\n");
		break;
	case('a'):
	case('A'):
		sideCam = -1;
		std::printf("Moving Left\n");
		break;
	case('d'):
	case('D'):
		sideCam = 1;
		std::printf("Moving Right\n");
		break;
	case('q'):
	case('Q'):
		rolCam = -1;
		std::printf("Spinning Negative Roll\n");
		break;
	case('e'):
	case('E'):
		rolCam = 1;
		std::printf("Spinning Positive Roll\n");
		break;
	}
}

void keypressUp(unsigned char key, int x, int y){
	switch (key)
	{
	case('w'):
	case('W'):
	case('s'):
	case('S'):
		frontCam = 0;
		break;
	case('a'):
	case('A'):
	case('d'):
	case('D'):
		sideCam = 0;
		break;
	case('q'):
	case('Q'):
	case('e'):
	case('E'):
		rolCam = 0;
		break;
	case(' '):
		pause = !pause;
		break;
	}
}

void specialKeypress(int key, int x, int y){
	switch (key)
	{
	case (GLUT_KEY_SHIFT_L):
	case (GLUT_KEY_SHIFT_R):
		speed = 4;
		break;
	case (GLUT_KEY_LEFT):
		printf("Spinning Negative Yaw\n");
		yawCam = -1;
		break;
	case (GLUT_KEY_RIGHT):
		printf("Spinning Positive Yaw\n");
		yawCam = 1;
		break;
	case (GLUT_KEY_UP):
		printf("Spinning Positive Pit\n");
		pitCam = 1;
		break;
	case (GLUT_KEY_DOWN):
		printf("Spinning Negative Pit\n");
		pitCam = -1;
		break;
	}
}

void specialKeypressUp(int key, int x, int y){
	switch (key)
	{
	case (GLUT_KEY_SHIFT_L):
	case (GLUT_KEY_SHIFT_R):
		speed = 1;
		break;
	case (GLUT_KEY_LEFT):
	case (GLUT_KEY_RIGHT):
		yawCam = 0;
		break;
	case (GLUT_KEY_UP):
	case (GLUT_KEY_DOWN):
		pitCam = 0;
		break;
	}
}

void (mouse)(int x, int y){
}

#pragma endregion INPUT FUNCTIONS

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("GameApp");
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);


	// Tell glut where the display function is
	glutWarpPointer(width / 2, height / 2);
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);

	// Input Function Initialisers //
	glutKeyboardFunc(keypress);
	glutPassiveMotionFunc(mouse);
	glutSpecialFunc(specialKeypress);
	glutSpecialUpFunc(specialKeypressUp);
	glutKeyboardUpFunc(keypressUp);

	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	init();
	textID = add_text("hi",
		-0.95, 0.9, fontSize, 1.0f, 1.0f, 1.0f, 1.0f);

	glutMainLoop();
	return 0;
}