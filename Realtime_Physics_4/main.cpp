
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
GLuint simpleShaderID, noTextureShaderID, cubeMapShaderID, textureShaderID;
Shader shaderFactory;
/*----------------------------------------------------------------------------
							OTHER VARIABLES
----------------------------------------------------------------------------*/

const char* atlas_image = "../freemono.png";
const char* atlas_meta = "../freemono.meta";

float fontSize = 25.0f;
int textID = -1;
bool pause = false;
bool boundingMethod = true;
int direction = 0;

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
	textureShaderID = shaderFactory.CompileShader(TEXTURE_VERT, TEXTURE_FRAG);


	skyBox.initCubeMap(vertices, 36, "desert");
	cube.init(CUBE_MESH, NULL, NULL);
	cube.mode = GL_QUADS;
	sphere.init(SPHERE_MESH, NULL, NULL);

	bodySystem = RigidBodySystem(2, CUBE_MESH);
	bodySystem.bodies[0].setPosition(glm::vec3(3.0, 0.0, 0.0));
	bodySystem.bodies[1].setPosition(glm::vec3(-3.0, 0.0, 0.0));
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
	lightStruct.Ls = glm::vec3(0.1f, 0.1f, 0.1f);	//Specular Reflected Light
	lightStruct.Ld = glm::vec3(0.99f, 0.99f, 0.99f);	//Diffuse Surface Reflectance
	lightStruct.La = glm::vec3(0.4f, 0.4f, 0.4f);	//Ambient Reflected Light
	lightStruct.lightLocation = glm::vec3(5 * sin(rotateLight), 10, -5.0f*cos(rotateLight));//light source location
	// object colour
	lightStruct.Ks = glm::vec3(0.1f, 0.1f, 0.1f); // specular reflectance
	lightStruct.Kd = BROWN;
	lightStruct.Ka = glm::vec3(0.5f, 0.5f, 0.5f); // ambient reflectance
	lightStruct.specular_exponent = 0.5f; //specular exponent - size of the specular elements

	drawCubeMap(cubeMapShaderID, cam, skyBox, lightStruct);

	for (int i = 0; i < bodySystem.bodies.size(); i++)
	{
		model = glm::mat4();
		lightStruct.Ka = bodySystem.bodies[i].colour;
		drawObject(textureShaderID, cam, bodySystem.bodies[i].mesh, model, false, lightStruct);

		if (bodySystem.bodies[i].colour == RED)
		{
			string output = "Collision Detected\n";
			update_text(textID, output.c_str());
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
			rotateLight = rotateLight + 0.01f;
			if (rotateLight >= 360.0f)
				rotateLight = 0.0f;
			string output = "No Collisions Detected\n";
			update_text(textID, output.c_str());

			bodySystem.bodies[0].clearMomentum();
			bodySystem.bodies[0].addForce(glm::vec3(2000.0, 0.0, 0.0) * (1.0f*direction), glm::vec3(0.0, 0.0, 0.0));
			bodySystem.bodies[0].resolveForce(delta);
			bodySystem.checkCollisions(boundingMethod);
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
	case(char(13)):
		boundingMethod = !boundingMethod;
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
		direction = 1;
		break;
	case (GLUT_KEY_RIGHT):
		direction = -1;
		break;
	case (GLUT_KEY_UP):
		break;
	case (GLUT_KEY_DOWN):
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
		direction = 0;
		break;
	case (GLUT_KEY_UP):
	case (GLUT_KEY_DOWN):
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