
#include <time.h>
#include <common/Shader.h>
#include <common/Utilities.h>
#include <common/EulerCamera.h>
#include <common/Defines.h>
#include <GLM.h>
#include "glm/ext.hpp"
#include <common/text.h>
#include <common/Collision.h>

using namespace std;

const float width = 900, height = 900;
/*----------------------------------------------------------------------------
						MESH AND TEXTURE VARIABLES
----------------------------------------------------------------------------*/
glm::vec3 mouseLocation = glm::vec3(0.0f, 0.5f, 0.0f);
glm::vec3 closestPoint = glm::vec3(0.5f, -0.5f, 0.0f);
glm::vec3 trianglePoints[2];


glm::vec3 triangle[] =
{
	glm::vec3(0.0f, 0.5f, 0.0f),
	glm::vec3(0.5f, -0.5f, 0.0f),
	glm::vec3(-0.5f, -0.5f, 0.0f)
};

vector<glm::vec3> mouseTriangle(triangle, triangle + (sizeof triangle / sizeof triangle[0]));


/*----------------------------------------------------------------------------
								SHADER VARIABLES
----------------------------------------------------------------------------*/
GLuint simpleShaderID;
/*----------------------------------------------------------------------------
							OTHER VARIABLES
----------------------------------------------------------------------------*/

const char* atlas_image = "../freemono.png";
const char* atlas_meta = "../freemono.meta";

float fontSize = 25.0f;
int textID = -1;
/*----------------------------------------------------------------------------
						FUNCTION DEFINITIONS
----------------------------------------------------------------------------*/

void rotateTriangle(float degrees);

void PointToTriangle();

void TriangleToTriangle();

/*--------------------------------------------------------------------------*/

void init()
{
	if (!init_text_rendering(atlas_image, atlas_meta, width, height)) 
	{
		fprintf(stderr, "ERROR init text rendering\n");
		exit(1);
	}
	Shader* factory = Shader::getInstance();
	simpleShaderID = factory->CompileShader(SIMPLE_VERT, SIMPLE_FRAG);
	Shader::resetInstance();
}

void display() 
{
	glEnable(GL_DEPTH_TEST);								// enable depth-testing
	glDepthFunc(GL_LESS);									// depth-testing interprets a smaller value as "closer"
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear the color and buffer bits to make a clean slate
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);					//Create a background

	TriangleToTriangle();

	glutSwapBuffers();
}

void updateScene() 
{
	static clock_t lastFrame = clock();
	clock_t currFrame = clock();
	float delta = (currFrame - lastFrame) / (float)CLOCKS_PER_SEC;
	if (delta >= 0.03f)
	{
		lastFrame = currFrame;
		glutPostRedisplay();
		rotateTriangle(0.5);
		closestPoint = getClosestPointTriangle(triangle, mouseLocation);
		mouseTriangle[0] = triangle[0]/2.0f + mouseLocation;
		mouseTriangle[1] = triangle[1]/2.0f + mouseLocation;
		mouseTriangle[2] = triangle[2]/2.0f + mouseLocation;
		getClosestTriangleTriangle(triangle, &mouseTriangle[0], trianglePoints[0], trianglePoints[1]);
	}
	
}

#pragma region INPUT FUNCTIONS

void keypress(unsigned char key, int x, int y) 
{
	if (key == (char)27)	//Pressing Escape Ends the game
	{
		exit(0);
	}
}

void keypressUp(unsigned char key, int x, int y)
{
}

void specialKeypress(int key, int x, int y) 
{
}

void specialKeypressUp(int key, int x, int y) 
{
}

void (mouse)(int x, int y)
{
	mouseLocation.x = ((2 / width) * x) - 1;
	mouseLocation.y = 1 - (((2 / height) * y));
}

#pragma endregion INPUT FUNCTIONS

int main(int argc, char** argv) 
{

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

void rotateTriangle(float degrees)
{
	glm::mat4 rotate = glm::rotate(glm::mat4(), degrees, glm::vec3(0.0f, 0.0f, 1.0f));
	for (int i = 0; i < 3; i++)
	{
		glm::vec4 t = rotate * glm::vec4(triangle[i], 1.0f);
		triangle[i] = glm::vec3(t.x, t.y, t.z);
	}
}

void PointToTriangle()
{
	drawLine(simpleShaderID, glm::mat4(), glm::mat4(), closestPoint, mouseLocation, PURPLE);

	//Check whether the mouse is within the triangle face

	if (closestPoint == mouseLocation)
		drawTriangle(simpleShaderID, glm::mat4(), glm::mat4(), triangle[0], triangle[1], triangle[2], PURPLE);
	else
		drawTriangle(simpleShaderID, glm::mat4(), glm::mat4(), triangle[0], triangle[1], triangle[2], YELLOW);


	glm::vec3 v12 = glm::normalize(triangle[0] - triangle[1]);
	glm::vec3 v23 = glm::normalize(triangle[1] - triangle[2]);
	glm::vec3 v31 = glm::normalize(triangle[2] - triangle[0]);

	//First, check that the closest point isn't a vertice, then check whether its on an edge

	if (closestPoint != triangle[0] && closestPoint != triangle[1] && closestPoint != triangle[2] && closestPoint != mouseLocation)
	{
		//<p1, p2>
		if (glm::dot(glm::normalize(triangle[0] - closestPoint), v12) >= 0.9999)
			drawTriangle(simpleShaderID, glm::mat4(), glm::mat4(), triangle[0], triangle[1], mouseLocation, PURPLE);
		//<p2, p3>
		if (glm::dot(glm::normalize(triangle[1] - closestPoint), v23) >= 0.9999)
			drawTriangle(simpleShaderID, glm::mat4(), glm::mat4(), triangle[1], triangle[2], mouseLocation, PURPLE);
		//<p3, p1>
		if (glm::dot(glm::normalize(triangle[2] - closestPoint), v31) >= 0.9999)
			drawTriangle(simpleShaderID, glm::mat4(), glm::mat4(), triangle[2], triangle[0], mouseLocation, PURPLE);
	}
	string text;
	text += "P1 = " + glm::to_string(triangle[0]) + "\n";
	text += "P2 = " + glm::to_string(triangle[1]) + "\n";
	text += "P3 = " + glm::to_string(triangle[2]) + "\n";
	text += "Distance = " + glm::to_string(getDistance(mouseLocation, closestPoint)) + "\n";
	text += "v12 = " + glm::to_string(v12) + "\n";
	text += "v23 = " + glm::to_string(v23) + "\n";
	text += "v31 = " + glm::to_string(v31) + "\n";
	text += "dot(p0,v12) = " + glm::to_string(glm::dot(glm::normalize(triangle[0] - closestPoint), v12)) + "\n";
	text += "dot(p0,v23) = " + glm::to_string(glm::dot(glm::normalize(triangle[1] - closestPoint), v23)) + "\n";
	text += "dot(p0,v31) = " + glm::to_string(glm::dot(glm::normalize(triangle[2] - closestPoint), v31)) + "\n";
	update_text(textID, text.c_str());
}

void TriangleToTriangle()
{
	drawTriangle(simpleShaderID, glm::mat4(), glm::mat4(), triangle[0], triangle[1], triangle[2], YELLOW);

	drawTriangle(simpleShaderID, glm::mat4(), glm::mat4(), mouseTriangle[0], mouseTriangle[1], mouseTriangle[2], YELLOW);

	drawLine(simpleShaderID, glm::mat4(), glm::mat4(), trianglePoints[0], trianglePoints[1], PURPLE);
}