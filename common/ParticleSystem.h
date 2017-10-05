#pragma once

#include <common/Particle.h>
#include <common/Forces.h>

class ParticleSystem
{
public:
	std::vector<Particle> particles;
	int numParticles;
	Drag d;
	Gravity g;

	void init(int p, SingleMesh& m);
	void applyForces(const float& delta);
	void checkCollisions(glm::vec3 point, glm::vec3 normal, float delta);
};