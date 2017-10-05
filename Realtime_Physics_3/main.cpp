
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

glm::vec3 triangle[] =
{
	glm::vec3(0.0f, 0.5f, 0.0f),
	glm::vec3(0.5f, -0.5f, 0.0f),
	glm::vec3(-0.5f, -0.5f, 0.0f)
};


/*----------------------------------------------------------------------------
								SHADER VARIABLES
----------------------------------------------------------------------------*/
GLuint simpleShaderID;
Shader shaderFactory;
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

void drawLine(GLuint shaderID, glm::mat4 model, glm::mat4 proj, glm::vec3 origin, glm::vec3 destination, glm::vec3 colour);
void drawTriangle(GLuint shaderID, glm::mat4 model, glm::mat4 proj, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 colour);
void drawPoint(GLuint shaderID, glm::mat4 model, glm::mat4 proj, glm::vec3 point, glm::vec3 colour);

/*--------------------------------------------------------------------------*/

void init()
{
	if (!init_text_rendering(atlas_image, atlas_meta, width, height)) 
	{
		fprintf(stderr, "ERROR init text rendering\n");
		exit(1);
	}
	simpleShaderID = shaderFactory.CompileShader(SIMPLE_VERT, SIMPLE_FRAG);
}

void display() 
{
	glm::mat4 proj = glm::mat4();
	glm::mat4 view = glm::mat4();
	glEnable(GL_DEPTH_TEST);								// enable depth-testing
	glDepthFunc(GL_LESS);									// depth-testing interprets a smaller value as "closer"
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear the color and buffer bits to make a clean slate
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);					//Create a background

	drawLine(simpleShaderID, glm::mat4(), glm::mat4(), mouseLocation, closestPoint, PURPLE);

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

void drawLine(GLuint shaderID, glm::mat4 model, glm::mat4 proj, glm::vec3 origin, glm::vec3 destination, glm::vec3 colour)
{
	GLfloat points[] =
	{
		origin.x, origin.y, origin.z,
		destination.x, destination.y, destination.z
	};
	GLuint vao, vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), points, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glUseProgram(shaderID);
	glBindVertexArray(vao);
	glUniform3fv(glGetUniformLocation(shaderID, "colour"), 1, &colour[0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, &model[0][0]);
	glDrawArrays(GL_LINES, 0, 2);
}

void drawTriangle(GLuint shaderID, glm::mat4 model, glm::mat4 proj, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 colour)
{
	GLfloat points[] =
	{
		v1.x, v1.y, v1.z,
		v2.x, v2.y, v2.z,
		v3.x, v3.y, v3.z
	};
	GLuint vao, vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), points, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glUseProgram(shaderID);
	glBindVertexArray(vao);
	glUniform3fv(glGetUniformLocation(shaderID, "colour"), 1, &colour[0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, &model[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void drawPoint(GLuint shaderID, glm::mat4 model, glm::mat4 proj, glm::vec3 point, glm::vec3 colour)
{
	GLfloat p[] =
	{
		point.x, point.y, point.z
	};
	GLuint vao, vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), p, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glUseProgram(shaderID);
	glBindVertexArray(vao);
	glUniform3fv(glGetUniformLocation(shaderID, "colour"), 1, &colour[0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, &model[0][0]);
	glDrawArrays(GL_POINTS, 0, 1);
}