#pragma once
#include "Utilities.h"

glm::vec3 getClosestPointTriangle(glm::vec3 triangle[], glm::vec3 p0);
glm::vec3 getClosestPointLine(glm::vec3 line[], glm::vec3 p0);
glm::vec3 getClosestPointPlane(glm::vec3 normal, glm::vec3 p1, glm::vec3 p0);
float getDistance(glm::vec3 v0, glm::vec3 v1);
void getClosestTriangleTriangle(glm::vec3 first[], glm::vec3 second[], glm::vec3 &point1, glm::vec3 &point2);

glm::vec3 getClosestPointTriangle(glm::vec3 triangle[], glm::vec3 p0)
{
	glm::vec3 p1 = triangle[0];
	glm::vec3 p2 = triangle[1];
	glm::vec3 p3 = triangle[2];

	//Check Each Voronoi Region

	//Vertex Regions:

	//p1
	if (glm::dot(p0 - p1, p3 - p1) <= 0 && glm::dot(p0 - p1, p2 - p1) <= 0)
	{
		return p1;
	}
	//p2
	if (glm::dot(p0 - p2, p3 - p2) <= 0 && glm::dot(p0 - p2, p1 - p2) <= 0)
	{
		return p2;
	}
	//p3
	if (glm::dot(p0 - p3, p2 - p3) <= 0 && glm::dot(p0 - p3, p1 - p3) <= 0)
	{
		return p3;
	}

	//Edge Regions:

	//<p1, p2>
	if (glm::dot(glm::cross(glm::cross(p3 - p2, p1 - p2), p1 - p2), p0 - p2) >= 0 && glm::dot(p0 - p1, p2 - p1) >= 0 && glm::dot(p0 - p2, p1 - p2) >= 0)
	{
		glm::vec3 line[] = { p1, p2 };
		return getClosestPointLine(line, p0);
	}
	// <p2, p3>
	if (glm::dot(glm::cross(glm::cross(p1 - p3, p2 - p3), p2 - p3), p0 - p3) >= 0 && glm::dot(p0 - p2, p3 - p2) >= 0 && glm::dot(p0 - p3, p2 - p3) >= 0)
	{
		glm::vec3 line[] = { p2, p3 };
		return getClosestPointLine(line, p0);
	}
	// <p3, p1>
	if (glm::dot(glm::cross(glm::cross(p2 - p1, p3 - p1), p3 - p1), p0 - p1) >= 0 && glm::dot(p0 - p3, p1 - p3) >= 0 && glm::dot(p0 - p1, p3 - p1) >= 0)
	{
		glm::vec3 line[] = { p3, p1 };
		return getClosestPointLine(line, p0);
	}

	//It must be the Face Region
	return getClosestPointPlane(glm::cross(p2, p1), p1, p0);
}
glm::vec3 getClosestPointLine(glm::vec3 line[], glm::vec3 p0)
{
	glm::vec3 u = line[1] - line[0];
	return line[0] + glm::normalize(u)*(glm::dot(p0 - line[0], glm::normalize(u)));
}
glm::vec3 getClosestPointPlane(glm::vec3 normal, glm::vec3 p1, glm::vec3 p0)
{
	normal = glm::normalize(normal);
	return (p0 - normal*glm::dot((p0 - p1), normal));
}
float getDistance(glm::vec3 v0, glm::vec3 v1)
{
	float x_sq = (v0.x - v1.x) * (v0.x - v1.x);
	float y_sq = (v0.y - v1.y) * (v0.y - v1.y);
	float z_sq = (v0.z - v1.z) * (v0.z - v1.z);
	return sqrt(x_sq + y_sq + z_sq);
}

void getClosestTriangleTriangle(glm::vec3 first[], glm::vec3 second[], glm::vec3 &point1, glm::vec3 &point2)
{

}


