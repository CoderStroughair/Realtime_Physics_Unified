#pragma once
#include "Utilities.h"

glm::vec3 getClosestPointTriangle(glm::vec3 triangle[], glm::vec3 p0);
glm::vec3 getClosestPointLine(glm::vec3 line[], glm::vec3 p0);
glm::vec3 getClosestPointPlane(glm::vec3 normal, glm::vec3 p1, glm::vec3 p0);
float getDistance(glm::vec3 v0, glm::vec3 v1);
float getClosestTriangleTriangle(const glm::vec3 first[], const glm::vec3 second[], glm::vec3 &point1, glm::vec3 &point2);
int getIndexOfClosestPoint(const glm::vec3 vertices[], const int &count, const glm::vec3 &d);

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

//float getClosestTriangleTriangle(const glm::vec3 first[], const glm::vec3 second[], glm::vec3 &point1, glm::vec3 &point2)
//{
//	float distance = FLT_MAX;
//	for (int i = 0; i < 3; i++)
//	{
//		for (int j = 0; j < 3; j++)
//		{
//			if (glm::length(first[i] - second[j]) < distance)
//			{
//				distance = glm::length(first[i] - second[j]);
//				point1 = first[i];
//				point2 = second[j];
//			}
//		}
//	}
//
//	return distance;
//}

float getClosestTriangleTriangle(const glm::vec3 first[], const glm::vec3 second[], glm::vec3 &point1, glm::vec3 &point2)
{
	//Initialise a Simplex set Q
	//Calculate C
	glm::vec3 d = { 1.0f, 1.0f, 0.0f };
	glm::vec3 C_a = first[getIndexOfClosestPoint(first, 3, d)];
	glm::vec3 C_b = first[getIndexOfClosestPoint(second, 3, -d)];
	glm::vec3 C = C_a - C_b;
	//Calculate B
	d = -d;
	glm::vec3 B_a = first[getIndexOfClosestPoint(first, 3, d)];
	glm::vec3 B_b = first[getIndexOfClosestPoint(second, 3, -d)];
	glm::vec3 B = B_a - B_b;
	//Compute P of minimum Norm in Q's convex hull

	//if P is the origin, return

	//Else, reduce Q to Q' by removing all points that don't contribute to the calculation of P

	//V = Support Mapping of (Minkowski Difference (a,b)(-P)) = Support Mapping of(A(-P)) - Support Mapping of (B(P))

	//If V isn't any closer to origin than P, stop and return no intersection.

	//Else, add V to Q and go back to step 2.

	return 0;
}

int getIndexOfClosestPoint(const glm::vec3 vertices[], const int &count, const glm::vec3 &d)
{
	int index = 0;
	float maxProduct = glm::dot(d, vertices[index]);
	for (int i = 1; i < count; i++) {
		float product = glm::dot(d, vertices[i]); // may be negative
		if (product > maxProduct) {
			maxProduct = product;
			index = i;
		}
	}
	return index;
}



