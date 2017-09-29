
#include <time.h>
#include <common/Shader.h>
#include <common/Utilities.h>
#include <common/SingleMeshLoader.h>
#include <common/EulerCamera.h>
#include <common/Defines.h>
#include <GLM.h>
#include "glm/ext.hpp"
#include <common/text.h>
#include <common/Particle.h>

using namespace std;

const float width = 1200, height = 950;

SingleMesh monkeyhead_object, torch_object, wall_object, cube, bear_object, signReflect_object, signRefract_object, signNormal_object;
SingleMesh gem_object, sphere_object, particle_object, cube_object;


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

/*----------------------------------------------------------------------------
								SHADER VARIABLES
----------------------------------------------------------------------------*/

GLuint noTextureShaderID, textureShaderID, cubeMapTextureID, refractiveShaderID, cubeMapShaderID;
GLuint testID, normalisedShaderID, reflectiveShaderID, multiTextureShaderID, mirrorShaderID, debugSkyboxID;
Shader shaderFactory;
/*----------------------------------------------------------------------------
							CAMERA VARIABLES
----------------------------------------------------------------------------*/

glm::vec3 startingPos = { 0.0f, 0.0f, -20.0f };
GLfloat pitCam = 0, yawCam = 0, rolCam = 0, frontCam = 0, sideCam = 0, speed = 1;
float rotateY = 50.0f, rotateLight = 0.0f;

EulerCamera cam(startingPos, width, height);
RigidBody body;
/*----------------------------------------------------------------------------
							OTHER VARIABLES
----------------------------------------------------------------------------*/

const char* atlas_image = "../freemono.png";
const char* atlas_meta = "../freemono.meta";

float fontSize = 25.0f;
int textID = -1;
bool torque = false;
bool force = false;
bool gravity = false;
bool force1 = false, force2 = false;

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
	refractiveShaderID = shaderFactory.CompileShader(NOTEXTURE_VERT, FRESNEL_FRAG);
	noTextureShaderID = shaderFactory.CompileShader(NOTEXTURE_VERT, NOTEXTURE_FRAG);
	cubeMapShaderID = shaderFactory.CompileShader(SKY_VERT, SKY_FRAG);
	textureShaderID = shaderFactory.CompileShader(TEXTURE_VERT, TEXTURE_FRAG);
	normalisedShaderID = shaderFactory.CompileShader(NORMAL_VERT, NORMAL_FRAG);
	reflectiveShaderID = shaderFactory.CompileShader(NOTEXTURE_VERT, REFLECTIVE_FRAG);
	multiTextureShaderID = shaderFactory.CompileShader(TEXTURE_VERT, TEXTURE_FRAG);
	mirrorShaderID = shaderFactory.CompileShader(MIRROR_VERT, MIRROR_FRAG);
	testID = shaderFactory.CompileShader(TEST_VERT, TEST_FRAG);
	//*********************//
	//*****Init Objects*****//
	//*********************//
	monkeyhead_object.init(MONKEYHEAD_MESH, NULL, NULL);
	cube.initCubeMap(vertices, 36, "desert");
	torch_object.init(TORCH_MESH, NULL, NULL);
	torch_object.mode = GL_QUADS;
	bear_object.init(BEAR_MESH, BEAR_TEXTURE, NULL);
	wall_object.init(WALL_MESH, BRICK_TEXTURE, BRICK_NORMAL);
	signReflect_object.init(SIGN_MESH, REFLECT_TEXTURE, NULL);
	signRefract_object.init(SIGN_MESH, REFRACT_TEXTURE, NULL);
	signNormal_object.init(SIGN_MESH, NORMAL_TEXTURE, NULL);
	sphere_object.init(SPHERE_MESH, NULL, NULL);
	gem_object.init(GEM_MESH, NULL, NULL);
	particle_object.init(GEM_MESH, NULL, NULL);
	cube_object.init(CUBE_MESH, BEAR_TEXTURE, NULL);
	cube_object.mode = GL_QUADS;
	body = RigidBody(glm::vec4(0.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 0.0, 0.0), 5, 1.0, 1.0, 1.0, cube_object);
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
	// object colour
	lightStruct.Ks = glm::vec3(0.1f, 0.1f, 0.1f); // specular reflectance
	lightStruct.Kd = BROWN;
	lightStruct.Ka = glm::vec3(0.05f, 0.05f, 0.05f); // ambient reflectance
	lightStruct.specular_exponent = 0.5f; //specular exponent - size of the specular elements

	model = glm::mat4();
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

	model = body.orientationMat;
	model = glm::translate(model, glm::vec3(body.position.x, body.position.y, body.position.z));
	lightStruct.Kd = BLUE;
	lightStruct.specular_exponent = 50.0f;
	drawObject(noTextureShaderID, cam, body.mesh, model, false, lightStruct);
	draw_texts();
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
		rotateLight = rotateLight + 0.01f;
		if (rotateLight >= 360.0f)
			rotateLight = 0.0f;

		body.force = glm::vec4(0.0, 0.0, 0.0, 1.0);
		body.torque = glm::vec4(0.0, 0.0, 0.0, 1.0);
		body.addForce(glm::vec3(50.0, 0.0, 0.0) * glm::vec3(force1, force1, force1), glm::vec3(0.5, 0.0, 0.5));
		body.addForce(glm::vec3(2.0, 50.0, 0.0) * glm::vec3(force2, force2, force2), glm::vec3(0.0, 1.0, 0.0));
		if (gravity)
			body.torque += glm::vec4(0.0, 0.0, 50.0, 1.0);
		if (torque)
			body.torque += glm::vec4(50.0, 0.0, 0.0, 1.0);
		if (force)
			body.torque += glm::vec4(0.0, 50.0, 0.0, 1.0);
		cam.movForward(frontCam*speed);
		cam.movRight(sideCam*speed);
		cam.changeFront(pitCam, yawCam, rolCam);

		//body.resolveForce(delta);
		string values = "Force: [" + to_string(body.force.x) + ", " + to_string(body.force.y) + ", " + to_string(body.force.z) + "]\n";
		values += "Torque: [" + to_string(body.torque.x) + ", " + to_string(body.torque.y) + ", " + to_string(body.torque.z) + "]\n";
		values += "Linear Momentum: [" + to_string(body.linMomentum.x) + ", " + to_string(body.linMomentum.y) + ", " + to_string(body.linMomentum.z) + "]\n";
		values += "Angular Momentum: [" + to_string(body.angMomentum.x) + ", " + to_string(body.angMomentum.y) + ", " + to_string(body.angMomentum.z) + "]\n";
		values += "\n";
		values += "Orientation Matrix: \n|"+ to_string(body.orientationMat[0][0]) + ", " + to_string(body.orientationMat[1][0]) + ", " + to_string(body.orientationMat[2][0]) + "|\n";
		values += "|" + to_string(body.orientationMat[0][1]) + ", " + to_string(body.orientationMat[1][1]) + ", " + to_string(body.orientationMat[2][1]) + "|\n";
		values += "|" + to_string(body.orientationMat[0][2]) + ", " + to_string(body.orientationMat[1][2]) + ", " + to_string(body.orientationMat[2][2]) + "|\n";
		values += "\n";

		values += "Vertices: \n";
		vector<float> vertices;
		for (int i = 0; i < body.mesh.newpoints.size(); i=i+3)
		{
			vertices.push_back(body.mesh.newpoints[i]);
			vertices.push_back(body.mesh.newpoints[i+1]);
			vertices.push_back(body.mesh.newpoints[i+2]);
			bool add = true;
			for (int j = 0; j < i; j=j+3)
			{
				if ((vertices[i] == vertices[j]) && (vertices[i+1] == vertices[j+1]) && (vertices[i+2] == vertices[j+2]))
				{
					add = false;
					break;
				}
			}
			if (add)
				values += "[" + to_string(body.mesh.newpoints[i]) + "," + to_string(body.mesh.newpoints[i+1]) + "," + to_string(body.mesh.newpoints[i+2]) + "]\n";
		}
		values += "\n";

		update_text(textID, values.c_str());
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
	if ((key == 't') || (key == 'T'))
		torque = true;
	if ((key == 'f') || (key == 'F'))
		force = true;
	if ((key == 'g') || key == 'G')
		gravity = true;
	if (key == '1')
		force1 = true;
	if (key == '2')
		force2 = true;
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
	if ((key == 't') || (key == 'T'))
		torque = false;
	if ((key == 'f') || (key == 'F'))
		force = false;
	if (key == '1')
		force1 = false;
	if (key == '2')
		force2 = false;
	//if (key == ' ')
	//{
	//	body.reset(glm::vec4(0.0, 0.0, 0.0, 0.0), glm::vec4(0.0, 0.0, 0.0, 0.0));
	//}
	//else if (key == '\\')
	//{
	//	body.reset(glm::vec4(0.0, 0.0, 0.0, 0.0), glm::vec4(500.0, 500.0, 500.0, 0.0));
	//}
	if ((key == 'g') || key == 'G')
	{
		gravity = false;
	}
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
	textID = add_text("",
		-0.95, 0.9, fontSize, 1.0f, 1.0f, 1.0f, 1.0f);
	glutMainLoop();
	return 0;
}