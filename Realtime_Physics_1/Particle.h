#pragma once

#include <GLM.h>
#include <common/Defines.h>
#include <vector>
#include <common/SingleMeshLoader.h>

class Particle;
class Force;

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

	Particle(SingleMesh& me, glm::vec3 p, int m);
	void addForce(glm::vec3 f);
	void resolveForce(float delta);
	void reset();
	void evolve();
};

class Force 
{
public:
	virtual void applyForce(Particle& p) = 0;
};

class Drag : public Force 
{
public:
	void applyForce(Particle& p);
};

class Gravity : public Force 
{
public:
	void applyForce(Particle& p);
};


class ParticleSystem 
{
public:
	std::vector<Particle> particles;
	int numParticles;
	int numForces;
	Drag d;
	Gravity g;

	void init(int p, SingleMesh& m);
	void applyForces(float delta);
	void checkCollisions(glm::vec3 point, glm::vec3 normal, float delta);
};
