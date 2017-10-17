#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLM.h>
#include "EulerCamera.h"
#include "SingleMeshLoader.h"

/*----------------------------------------------------------------------------
								  STRUCTS
----------------------------------------------------------------------------*/

struct LightStruct {

	// light properties
	glm::vec3 Ls = glm::vec3(1.0f, 1.0f, 1.0f);	//Specular Reflected Light
	glm::vec3 Ld = glm::vec3(0.5f, 0.5f, 0.5f);	//Diffuse Surface Reflectance
	glm::vec3 La = glm::vec3(0.1f, 0.1f, 0.1f);	//Ambient Reflected Light
	glm::vec3 lightLocation = glm::vec3(0, 10, -5.0f);//light source location
	glm::vec3 coneDirection = lightLocation + glm::vec3(0.0f, -1.0f, 0.0f);
	float coneAngle = 40.0f;

	// object colour
	glm::vec3 Ks = glm::vec3(0.001f, 0.001f, 0.001f); // specular reflectance
	glm::vec3 Kd = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 Ka = glm::vec3(1.0f, 1.0f, 1.0f); // ambient reflectance
	float specular_exponent = 0.5f; //specular 'power'?

};

/*----------------------------------------------------------------------------
								DEFINITIONS
----------------------------------------------------------------------------*/
GLuint createQuad(int location);

void drawObject(GLuint shaderID, EulerCamera cam, SingleMesh &mesh, glm::mat4 model, bool cone, LightStruct lightLocation);

void drawCubeMap(GLuint shaderID, EulerCamera cam, SingleMesh &mesh, LightStruct lightLocation);

void drawLine(GLuint shaderID, glm::mat4 model, glm::mat4 proj, glm::vec3 origin, glm::vec3 destination, glm::vec3 colour);

void drawTriangle(GLuint shaderID, glm::mat4 model, glm::mat4 proj, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 colour);

void drawPoint(GLuint shaderID, glm::mat4 model, glm::mat4 proj, glm::vec3 point, glm::vec3 colour);
/*----------------------------------------------------------------------------
								IMPLEMENTATIONS
----------------------------------------------------------------------------*/

GLuint createQuad(int location) {
	GLuint vao = 0, vbo = 0;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	float* verts;
	switch (location)
	{
	case 0:	//Bottom left of screen
		verts = new float[16]
		{
			//positions		//textures
			-1.0f, -1.0f,	0.0f, 0.0f,	//Bottom Left
			-1.0f, -0.5f,	0.0f, 1.0f,	//Top Left
			-0.5f, -1.0f,	1.0f, 0.0f,	//Bottom Right
			-0.5f, -0.5f,	1.0f, 1.0f	//Top Right
		};
		break;
	case 1:	//Bottom Right of screen
		verts = new float[16]
		{
			//positions		//textures
			0.5f, -1.0f,	0.0f, 0.0f,	//Bottom Left
			0.5f, -0.5f,	0.0f, 1.0f,	//Top Left
			1.0f, -1.0f,	1.0f, 0.0f,	//Bottom Right
			1.0f, -0.5f,	1.0f, 1.0f	//Top Right
		};
		break;
	case 2:
		verts = new float[16]
		{
			//positions		//textures
			-1.0f, 0.5f,	0.0f, 0.0f,	//Bottom Left
			-1.0f, 1.0f,	0.0f, 1.0f,	//Top Left
			-0.5f, 0.5f,	1.0f, 0.0f,	//Bottom Right
			-0.5f, 1.0f,	1.0f, 1.0f	//Top Right
		};
		break;
	case 3:
	default:
		verts = new float[16]
		{
			//positions		//textures
			0.5f, 0.5f,		0.0f, 0.0f,	//Bottom Left
			0.5f, 1.0f,		0.0f, 1.0f,	//Top Left
			1.0f, 0.5f,		1.0f, 0.0f,	//Bottom Right
			1.0f, 1.0f,		1.0f, 1.0f	//Top Right
		};
		break;
	case 4:	//Left of Screen
		verts = new float[16]
		{
			//positions		//textures
			-1.0f, -1.0f,	0.0f, 0.0f,	//Bottom Left
			-1.0f,  1.0f,	0.0f, 1.0f,	//Top Left
			 0.0f, -1.0f,	1.0f, 0.0f,	//Bottom Right
			 0.0f,  1.0f,	1.0f, 1.0f	//Top Right
		};
		break;
	case 5:	//Right of Screen
		verts = new float[16]
		{
			//positions		//textures
			0.0f, -1.0f,	0.0f, 0.0f,	//Bottom Left
			0.0f,  1.0f,	0.0f, 1.0f,	//Top Left
			1.0f, -1.0f,	1.0f, 0.0f,	//Bottom Right
			1.0f,  1.0f,	1.0f, 1.0f	//Top Right
		};
		break;
	case 6:	//Bottom of Screen
		verts = new float[16]
		{
			//positions		//textures
			-0.5f, -1.0f,	0.0f, 0.0f,	//Bottom Left
			-0.5f, -0.5f,	0.0f, 1.0f,	//Top Left
			 0.5f, -1.0f,	1.0f, 0.0f,	//Bottom Right
			 0.5f, -0.5f,	1.0f, 1.0f	//Top Right
		};
		break;
	case 7:	//Entire Screen
		verts = new float[16]
		{
			//positions		//textures
			-1.0f, -1.0f,	0.0f, 0.0f,	//Bottom Left
			-1.0f,  1.0f,	0.0f, 1.0f,	//Top Left
			 1.0f, -1.0f,	1.0f, 0.0f,	//Bottom Right
			 1.0f,  1.0f,	1.0f, 1.0f	//Top Right
		};
		break;
	}


	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), verts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	GLintptr stride = 4 * sizeof(float);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, NULL);

	glEnableVertexAttribArray(1);
	GLintptr offset = 2 * sizeof(float);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid *)offset);
	return vao;
}

void drawObject(GLuint shaderID, EulerCamera cam, SingleMesh &mesh, glm::mat4 model, bool cone, LightStruct lightLocation)
{
	glUseProgram(shaderID);
	glBindVertexArray(mesh.VAO[0]);
	//Light Setup
	glUniform3fv(glGetUniformLocation(shaderID, "Ls"), 1, &lightLocation.Ls[0]);
	glUniform3fv(glGetUniformLocation(shaderID, "Ld"), 1, &lightLocation.Ld[0]);
	glUniform3fv(glGetUniformLocation(shaderID, "La"), 1, &lightLocation.La[0]);

	glUniform3fv(glGetUniformLocation(shaderID, "Ks"), 1, &lightLocation.Ks[0]);
	glUniform3fv(glGetUniformLocation(shaderID, "Kd"), 1, &lightLocation.Kd[0]);
	glUniform3fv(glGetUniformLocation(shaderID, "Ka"), 1, &lightLocation.Ka[0]);

	glUniform3fv(glGetUniformLocation(shaderID, "light"), 1, &lightLocation.lightLocation[0]);
	glUniform3fv(glGetUniformLocation(shaderID, "camPos"), 1, &cam.getPosition()[0]);
	glUniform1f(glGetUniformLocation(shaderID, "specular_exponent"), lightLocation.specular_exponent);

	if (cone)
	{
		lightLocation.coneDirection = lightLocation.lightLocation + glm::vec3(0.0f, -1.0f, 0.0f);
		glUniform3fv(glGetUniformLocation(shaderID, "coneDirection"), 1, &lightLocation.coneDirection[0]);
		glUniform1f(glGetUniformLocation(shaderID, "coneAngle"), lightLocation.coneAngle);
	}
	//Matrix Setup
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, GL_FALSE, &cam.getView()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "proj"), 1, GL_FALSE, &cam.getProj()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, &model[0][0]);


	if (mesh.tex)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh.tex);
		glUniform1i(glGetUniformLocation(shaderID, "texture_primary"), 0);
	}

	if (mesh.norm)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mesh.norm);
		glUniform1i(glGetUniformLocation(shaderID, "normal_map"), 1);
	}
	glDrawArrays(mesh.mode, 0, mesh.mesh_vertex_count);
}

void drawCubeMap(GLuint shaderID, EulerCamera cam, SingleMesh &mesh, LightStruct lightLocation)
{
	glDepthMask(GL_FALSE);
	glUseProgram(shaderID);
	glBindVertexArray(mesh.VAO[0]);
	glUniform3fv(glGetUniformLocation(shaderID, "Ld"), 1, &lightLocation.Ld[0]);
	glUniform3fv(glGetUniformLocation(shaderID, "La"), 1, &lightLocation.La[0]);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shaderID, "cube_texture"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mesh.tex);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "proj"), 1, GL_FALSE, &cam.getProj()[0][0]);
	glm::mat4 view = cam.getView();
	view[3][0] = 0;
	view[3][1] = 0;
	view[3][2] = 0;
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, GL_FALSE, &view[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, mesh.mesh_vertex_count);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
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
#endif