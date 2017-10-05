#include <common/ParticleSystem.h>

void ParticleSystem::init(int p, SingleMesh& m)
{
	numParticles = p;
	int length = p / 20;
	for (int i = -length / 2; i < length / 2; i++)
	{
		for (int j = -10; j < 10; j++)
		{
			Particle particle(m, glm::vec3(0.0f, 20.0f, 0.0f), rand() % 400 + 100.0f);
			if (particle.mass <= 0)
				particle.mass = 1;
			particles.push_back(particle);
		}
	}
}
void ParticleSystem::applyForces(const float &delta)
{
	for (int i = 0; i < numParticles; i++)
	{
		if (particles[i].health <= 0)
			particles[i].reset();
		particles[i].force = glm::vec3(0.0, 0.0, 0.0);
		d.applyForce(particles[i]);
		g.applyForce(particles[i]);

		particles[i].resolveForce(delta);
		particles[i].health--;
	}
}
void ParticleSystem::checkCollisions(glm::vec3 point, glm::vec3 normal, float delta)
{
	float coRest = 0.6f;
	for (int i = 0; i < numParticles; i++)
	{
		if (glm::dot((particles[i].position - point), normal) < 0.00001f && glm::dot(particles[i].velocity, normal) < 0.00001f)
		{
			glm::vec3 deltaX = particles[i].position - normal * 1.4f * glm::dot((particles[i].position - point), normal);
			particles[i].position = deltaX;
			particles[i].velocity = particles[i].velocity - (normal*glm::dot(particles[i].velocity, normal))*(1 + coRest);
		}
	}
}