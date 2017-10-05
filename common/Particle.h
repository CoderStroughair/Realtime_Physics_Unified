#pragma once

#include <GLM.h>
#include <vector>
#include <common/SingleMeshLoader.h>

class Particle
{
	glm::vec3 initialPosition;
public:
	SingleMesh mesh;

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 force;
	float mass;
	int health;
	glm::vec3 colour;
	float scale;

	Particle(SingleMesh& me, const glm::vec3 &p, const float &m);
	void addForce(const glm::vec3 &f);
	void resolveForce(const float &delta);
	void reset();
	void evolve();
};
