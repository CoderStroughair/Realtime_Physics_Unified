#include <time.h>
#include <common/Shader.h>
#include <common/Utilities.h>
#include <common/SingleMeshLoader.h>
#include <common/EulerCamera.h>
#include <common/Defines.h>
#include <GLM.h>
#include "glm/ext.hpp"
#include <common/text.h>
#include <common/Framebuffer.h>
#include <common/ParticleSystem.h>

using namespace std;

const int width = 800, height = 800;

SingleMesh torch_object, wall_object, cube;
SingleMesh particle_object;

//Cube Vertices for the Skybox
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


GLuint noTextureShaderID, textureShaderID, cubeMapTextureID, cubeMapShaderID;
GLuint normalisedShaderID;
/*----------------------------------------------------------------------------
							CAMERA VARIABLES
----------------------------------------------------------------------------*/

glm::vec3 startingPos = { 0.0f, 0.0f, -20.0f };
GLfloat pitCam = 0, yawCam = 0, rolCam = 0, frontCam = 0, sideCam = 0, speed = 1;
float rotateY = 50.0f, rotateLight = 0.0f;

EulerCamera cam(startingPos, width, height);

/*----------------------------------------------------------------------------
							OTHER VARIABLES
----------------------------------------------------------------------------*/

const char* atlas_image = "../freemono.png";
const char* atlas_meta = "../freemono.meta";

float fontSize = 25.0f;
int textID = -1;

const int numGems = 300;
glm::vec3 gemLocation[numGems];
glm::vec3 gemColour[numGems];
bool simple = true, mesh = true, pause = false;

ParticleSystem ps;
/*--------------------------------------------------------------------------*/

void init()
{
	if (!init_text_rendering(atlas_image, atlas_meta, width, height)) 
	{
		fprintf(stderr, "ERROR init text rendering\n");
		exit(1);
	}
	cam.setSensitivity(2.0f);
	//*************************//
	//*****Compile Shaders*****//
	//*************************//
	Shader* shaderFactory = Shader::getInstance();
	noTextureShaderID = shaderFactory->CompileShader(NOTEXTURE_VERT, NOTEXTURE_FRAG);
	cubeMapShaderID = shaderFactory->CompileShader(SKY_VERT, SKY_FRAG);
	textureShaderID = shaderFactory->CompileShader(TEXTURE_VERT, TEXTURE_FRAG);
	normalisedShaderID = shaderFactory->CompileShader(NORMAL_VERT, NORMAL_FRAG);
	Shader::resetInstance();
	//*********************//
	//*****Init Objects*****//
	//*********************//
	cube.initCubeMap(vertices, 36, "desert");
	torch_object.init(TORCH_MESH, NULL, NULL);
	torch_object.mode = GL_QUADS;
	wall_object.init(WALL_MESH, BRICK_TEXTURE, BRICK_NORMAL);
	particle_object.init(GEM_MESH, NULL, NULL);

	ps.init(1000, particle_object);
}

void display() 
{
	glEnable(GL_DEPTH_TEST);								// enable depth-testing
	glDepthFunc(GL_LESS);									// depth-testing interprets a smaller value as "closer"
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear the color and buffer bits to make a clean slate
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);					//Create a background

	LightStruct lightStruct;
	glm::mat4 model;
	// light properties
	lightStruct.Ls = glm::vec3(1.0f, 1.0f, 1.0f);	//Specular Reflected Light
	lightStruct.Ld = glm::vec3(0.99f, 0.99f, 0.99f);	//Diffuse Surface Reflectance
	lightStruct.La = glm::vec3(1.0f, 1.0f, 1.0f);	//Ambient Reflected Light
	lightStruct.lightLocation = glm::vec3(5 * sin(rotateLight), 10, -5.0f*cos(rotateLight));//light source location
	lightStruct.coneDirection = lightStruct.lightLocation + glm::vec3(0.0f, -1.0f, 0.0f);
	float coneAngle = 10.0f;
	// object colour
	lightStruct.Ks = glm::vec3(0.1f, 0.1f, 0.1f); // specular reflectance
	lightStruct.Kd = BROWN;
	lightStruct.Ka = glm::vec3(0.05f, 0.05f, 0.05f); // ambient reflectance
	lightStruct.specular_exponent = 0.5f; //specular exponent - size of the specular elements

	drawCubeMap(cubeMapShaderID, cam, cube, lightStruct);

	model = glm::translate(glm::mat4(), lightStruct.lightLocation);
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
	lightStruct.Kd = WHITE;
	drawObject(noTextureShaderID, cam, cube, model, false, lightStruct);

	model = glm::translate(glm::mat4(), glm::vec3(0.0f, -6.3f, 0.0f));
	lightStruct.Kd = BROWN;
	drawObject(noTextureShaderID, cam, torch_object, model, false, lightStruct);

	lightStruct.Kd = GREY;
	model = glm::translate(glm::mat4(), glm::vec3(-7.5, -6.5f, -7.5));
	model = glm::rotate(model, 90.0f, glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 root = model;
	lightStruct.specular_exponent = 50.0f;
	lightStruct.Ks = glm::vec3(1.0, 1.0, 1.0);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			model = glm::translate(glm::mat4(), glm::vec3(16.0f*i, 0.0, 0.0));
			model = glm::translate(model, glm::vec3(0.0, 0.0, 32.0f*j));
			model = model * root;
			drawObject(normalisedShaderID, cam, wall_object, model, false, lightStruct);
		}
	}

	lightStruct.specular_exponent = 0.9f; //specular exponent - size of the specular elements
	lightStruct.Ks = glm::vec3(0.8f, 0.8f, 0.8f);
	lightStruct.Ka = glm::vec3(1.0f, 1.0f, 1.0f);
	for (int i = 0; i < ps.numParticles; i++)
	{
		ps.particles[i].evolve();
		lightStruct.Ka = ps.particles[i].colour / 2.0f;
		lightStruct.Kd = ps.particles[i].colour;
		model = glm::translate(glm::mat4(), ps.particles[i].position);
		model = glm::scale(model, glm::vec3(ps.particles[i].scale, ps.particles[i].scale, ps.particles[i].scale));

		drawObject(noTextureShaderID, cam, ps.particles[i].mesh, model, false, lightStruct);
	}
	model = glm::translate(glm::mat4(), glm::vec3(0.0, 21.0, 0.0));
	model = glm::scale(model, glm::vec3(2.0, 2.0, 2.0));
	model = glm::rotate(model, 132.0f, glm::vec3(0.0, 0.0, 1.0));

	lightStruct.Kd = ps.particles[0].colour;
	drawObject(noTextureShaderID, cam, ps.particles[0].mesh, model, false, lightStruct);
	glutSwapBuffers();
}

void updateScene() {
	static clock_t lastFrame = clock();
	clock_t currFrame = clock();
	float delta = (currFrame - lastFrame) / (float)CLOCKS_PER_SEC;
	if (delta >= 0.03f)
	{
		lastFrame = currFrame;
		glutPostRedisplay();
		rotateY = rotateY + 0.5f;
		rotateLight = rotateLight + 0.01f;
		if (rotateY >= 360.0f)
			rotateY = 0.0f;
		if (rotateLight >= 360.0f)
			rotateLight = 0.0f;
		cam.movForward(frontCam*speed);
		cam.movRight(sideCam*speed);
		cam.changeFront(pitCam, yawCam, rolCam);
		if (!pause)
		{
			ps.applyForces(delta);
			ps.checkCollisions(glm::vec3(0.0, -6.5, 0.0), glm::vec3(0.0, 0.5, -0.5), delta);
			ps.checkCollisions(glm::vec3(0.0, -6.5, 0.0), glm::vec3(0.0, 0.5, 0.5), delta);
			ps.checkCollisions(glm::vec3(0.0, -6.5, 0.0), glm::vec3(0.5, 0.5, 0.0), delta);
			ps.checkCollisions(glm::vec3(0.0, -6.5, 0.0), glm::vec3(-0.5, 0.5, 0.0), delta);
			ps.checkCollisions(glm::vec3(0.0, +2.5, 0.0), glm::vec3(0.0, 1.0, 0.0), delta);
		}
	}
	
}

#pragma region INPUT FUNCTIONS

void keypress(unsigned char key, int x, int y) 
{
	if (key == (char)27)	//Pressing Escape Ends the game
	{
		exit(0);
	}
	else if (key == 'w' || key == 'W')
		frontCam = 1;
	else if ((key == 's') || (key == 'S'))
		frontCam = -1;
	if ((key == 'a') || (key == 'A'))
		sideCam = -1;
	else if ((key == 'd') || (key == 'D'))
		sideCam = 1;
	if (key == ' ')
	{
		simple = !simple;
		if (simple)
			update_text(textID, "Simplified Version - This only shows a single cubemap");
		else
			update_text(textID, "Complex Version - This shows the use of all four cubemaps");
	}
	if (key == 'm')
	{
		mesh = !mesh;
	}
}

void keypressUp(unsigned char key, int x, int y)
{
	if ((key == 'w') || (key == 'W'))
		frontCam = 0;
	else if ((key == 's') || (key == 'S'))
		frontCam = 0;
	if ((key == 'a') || (key == 'A'))
		sideCam = 0;
	else if ((key == 'd') || (key == 'D'))
		sideCam = 0;
	if (key == ' ')
		pause = !pause;
}

void specialKeypress(int key, int x, int y) 
{
	switch (key)
	{
	case (GLUT_KEY_SHIFT_L):
	case (GLUT_KEY_SHIFT_R):
		speed = 4;
		break;
	case (GLUT_KEY_LEFT):
		yawCam = -1;
		break;
	case (GLUT_KEY_RIGHT):
		yawCam = 1;
		break;
	case (GLUT_KEY_UP):
		pitCam = 1;
		break;
	case (GLUT_KEY_DOWN):
		pitCam = -1;
		break;
	}
}

void specialKeypressUp(int key, int x, int y) 
{
	switch (key)
	{
	case (GLUT_KEY_SHIFT_L):
	case (GLUT_KEY_SHIFT_R):
		speed = 1;
		break;
	case (GLUT_KEY_LEFT):
		yawCam = 0;
		break;
	case (GLUT_KEY_RIGHT):
		yawCam = 0;
		break;
	case (GLUT_KEY_UP):
		pitCam = 0;
		break;
	case (GLUT_KEY_DOWN):
		pitCam = 0;
		break;
	}
}

void (mouse)(int x, int y)
{
	
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
	textID = add_text(
		"Simplified Version - This only shows a single cubemap",
		-0.9f, 0.8f, fontSize, 1.0f, 1.0f, 1.0f, 1.0f);
	for (int i = 0; i < sizeof(gemLocation) / sizeof(*gemLocation); i++)
	{
		gemLocation[i] = glm::vec3(rand() % 50 - 25, -6.5f, rand() % 50 - 23);
		int colourNumber = rand() % 4;
		if (colourNumber == 0)
			gemColour[i] = RED;
		else if (colourNumber == 1)
			gemColour[i] = PURPLE;
		else if (colourNumber == 2)
			gemColour[i] = GREEN;
		else
			gemColour[i] = YELLOW;
	}
	glutMainLoop();
	return 0;
}