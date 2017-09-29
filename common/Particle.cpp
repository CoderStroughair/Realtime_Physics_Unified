#include "Particle.h"
#include <glm\gtc\constants.hpp>

Particle::Particle(SingleMesh& me, glm::vec3 p, float m)
{
	scale = rand() % 10 / 10.0f;
	health = 100;
	force = glm::vec3(0.0, 0.0, 0.0);
	velocity = glm::vec3(rand() % 30 - 15, 50, rand() % 30 - 15);
	mesh = me;
	position = p;
	initialPosition = p;
	mass = m;
	mass /= 100.0f;
	colour = RED;
}
void Particle::addForce(glm::vec3 f)
{
	force += f;
}
void Particle::resolveForce(float delta)
{
	glm::vec3 ac = force*mass;
	velocity += ac*delta;
	position += velocity*delta;
}
void Particle::reset()
{
	force = glm::vec3(0.0, 0.0, 0.0);
	position = initialPosition;
	velocity = glm::vec3(rand() % 30 - 15, 50, rand() % 30 - 15);
	health = rand() % 50 + 50;
	scale = rand() % 1000 / 1000.0f;
	colour = RED;
}
void Particle::evolve()
{
	if (health > 80)
		colour = RED;
	else if (health > 60)
		colour = PURPLE;
	else if (health > 40)
		colour = GREEN;
	else if (health > 20)
		colour = YELLOW;
	else
		colour = GREY;
}

void Drag::applyForce(Particle& p)
{
	float constants = -0.5f * 1.225f * 0.09f * glm::pi<float>() * (p.scale) * (p.scale);
	glm::vec3 velocity = glm::vec3(p.velocity.x * p.velocity.x, p.velocity.y * p.velocity.y, p.velocity.z * p.velocity.z);
	p.addForce(velocity*constants);
};

void Gravity::applyForce(Particle& p)
{
	float force_gravity = -9.81f*p.mass;
	p.addForce(glm::vec3(0.0, force_gravity, 0.0));
};


void ParticleSystem::init(int p, SingleMesh& m)
{
	numParticles = p;
	numForces = 0;
	int length = p / 20;
	for (int i = -length / 2; i < length / 2; i++)
	{
		for (int j = -10; j < 10; j++)
		{
			//Particle particle(m, vec3(rand() % 60 - 30, 20.0f, rand() % 60 - 30), rand() % 400+100);
			Particle particle(m, glm::vec3(0.0f, 20.0f, 0.0f), rand() % 400 + 100.0f);
			if (particle.mass <= 0)
				particle.mass = 1;
			particles.push_back(particle);
		}
	}
	numForces = 2;
}
void ParticleSystem::applyForces(float delta)
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