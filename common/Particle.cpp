#include <common/Particle.h>
#include <common/Defines.h>
#include <glm\gtc\constants.hpp>

Particle::Particle(SingleMesh& me, const glm::vec3 &p, const float &m)
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
void Particle::addForce(const glm::vec3 &f)
{
	force += f;
}
void Particle::resolveForce(const float &delta)
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