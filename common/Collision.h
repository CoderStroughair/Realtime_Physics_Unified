#pragma once
#include "Utilities.h"

glm::vec3 getClosestPointTriangle(glm::vec3 triangle[], glm::vec3 p0);
glm::vec3 getClosestPointLine(glm::vec3 line[], glm::vec3 p0);
glm::vec3 getClosestPointPlane(glm::vec3 normal, glm::vec3 p1, glm::vec3 p0);
float getDistance(glm::vec3 v0, glm::vec3 v1);
float getClosestTriangleTriangle(const glm::vec3 first[], const glm::vec3 second[], glm::vec3 &point1, glm::vec3 &point2, glm::vec3 simplex[]);
int getIndexOfFurthestPoint(const glm::vec3 vertices[], const int &count, const glm::vec3 &d);
glm::vec3 getMinkowskiSumPoint(const glm::vec3 first[], const int numFirst, const glm::vec3 second[], const int numSecond, const glm::vec3 direction);

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

/*
Implements the GJK Collision Algorithm
*/
float getClosestTriangleTriangle(const glm::vec3 first[], const glm::vec3 second[], glm::vec3 &point1, glm::vec3 &point2, glm::vec3 simplex[])
{
	glm::vec3 directionValues[] =
	{
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.966f, 0.259f, 0.0f),
		glm::vec3(0.866f, 0.5f, 0.0f),
		glm::vec3(0.707f, 0.707f, 0.0f),
		glm::vec3(0.5f, 0.866f, 0.0f),
		glm::vec3(0.259f, 0.966f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
	};
	for (int i = 0; i < 7; i++)
	{
		//Initialise a Simplex set Q
		//Calculate C
		glm::vec3 d = directionValues[i];
		glm::vec3 C = getMinkowskiSumPoint(first, 3, second, 3, d);
		if (C == glm::vec3())
		{
			return 0;
		}
		//Calculate B
		glm::vec3 CO = -C;
		glm::vec3 B = getMinkowskiSumPoint(first, 3, second, 3, CO);
		if (B == glm::vec3())
		{
			return 0;
		}
		if (glm::dot(CO, B) < 0)
		{
			continue;
		}
		//Calculate A
		glm::vec3 CB = B - C;
		//Normally done by vector triple product (CB x CO x CB), but can be expanded out to make fewer calculations (a x (b x c) = b(a · c) - c(a · b))
		d = (CO * glm::dot(CB, CB)) - (CB * glm::dot(CB, CO));
		glm::vec3 A = getMinkowskiSumPoint(first, 3, second, 3, d);

		glm::vec3 triangle[3] = { A, B, C };
		simplex[0] = triangle[0];
		simplex[1] = triangle[1];
		simplex[2] = triangle[2];

		if (A == glm::vec3())
		{
			return 0;
		}
		glm::vec3 closest = getClosestPointTriangle(triangle, glm::vec3(0.0, 0.0, 0.0));

		if (closest == glm::vec3(0.0, 0.0, 0.0))
		{
			point1 = glm::vec3();
			point2 = glm::vec3();
			return 0;
		}
	}
	cout << "no collision detected\n";
	return -1;
/*	std::cout << closest[0] << closest[1] << closest[2] << endl;
	if (closest == A)
	{
		point1 = first[A_indexes[0]];
		point2 = second[A_indexes[1]];
		return glm::length(A);
	}
	if (closest == B)
	{
		point1 = first[B_indexes[0]];
		point2 = second[B_indexes[1]];
		return glm::length(B);
	}
	if (closest == C)
	{
		point1 = first[C_indexes[0]];
		point2 = second[C_indexes[1]];
		return glm::length(C);
	}
	else
	{
		//<A, B>
		if (glm::dot(glm::cross(glm::cross(C - B, A - B), A - B), -B) >= 0 && glm::dot(-A, B - A) >= 0 && glm::dot(-B, A - B) >= 0)
		{
			point1 = (first[A_indexes[0]] + first[B_indexes[0]]) / 2.0f;
			point2 = (first[A_indexes[1]] + first[B_indexes[1]]) / 2.0f;
			return glm::length(point1 - point2);
		}
		// <B, C>
		else if (glm::dot(glm::cross(glm::cross(A - C, B - C), B - C), -C) >= 0 && glm::dot(-B, C - B) >= 0 && glm::dot(-C, B - C) >= 0)
		{
			point1 = (first[B_indexes[0]] + first[C_indexes[0]]) / 2.0f;
			point2 = (first[B_indexes[1]] + first[C_indexes[1]]) / 2.0f;
			return glm::length(point1 - point2);
		}
		// <C, A>
		else if (glm::dot(glm::cross(glm::cross(B - A, C - A), C - A), -A) >= 0 && glm::dot(-C, A - C) >= 0 && glm::dot(-A, C - A) >= 0)
		{
			point1 = (first[C_indexes[0]] + first[A_indexes[0]]) / 2.0f;
			point2 = (first[C_indexes[1]] + first[A_indexes[1]]) / 2.0f;
			return glm::length(point1 - point2);
		}
	}
	cout << "Error - Collision Function" << endl;
	return -1;

	//Compute P of minimum Norm in Q's convex hull

	//if P is the origin, return

	//Else, reduce Q to Q' by removing all points that don't contribute to the calculation of P

	//V = Support Mapping of (Minkowski Difference (a,b)(-P)) = Support Mapping of(A(-P)) - Support Mapping of (B(P))

	//If V isn't any closer to origin than P, stop and return no intersection.

	//Else, add V to Q and go back to step 2.

	return 0;
	*/
}

int getIndexOfFurthestPoint(const glm::vec3 vertices[], const int &count, const glm::vec3 &direction)
{
	int index = 0;
	float maxProduct = glm::dot(direction, vertices[index]);
	for (int i = 1; i < count; i++) {
		float product = glm::dot(direction, vertices[i]); // may be negative
		if (product > maxProduct) {
			maxProduct = product;
			index = i;
		}
	}
	return index;
}

glm::vec3 getMinkowskiSumPoint(const glm::vec3 first[], const int numFirst, const glm::vec3 second[], const int numSecond, const glm::vec3 direction)
{
	int i = getIndexOfFurthestPoint(first, numFirst, direction);
	int j = getIndexOfFurthestPoint(second, numSecond, -direction);
	return first[i] - second[j];
}



