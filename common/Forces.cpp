#include <common/Forces.h>

void Drag::applyForce(Particle& p)
{
	float constants = -0.5f * 1.225f * 0.09f * glm::pi<float>() * (p.scale) * (p.scale);
	glm::vec3 velocity = glm::vec3(p.velocity.x * p.velocity.x, p.velocity.y * p.velocity.y, p.velocity.z * p.velocity.z);
	p.addForce(velocity*constants);
};

void Drag::applyForce(RigidBody& b)
{
	GLfloat constants = -0.5 * 1.225 * 1.05 * 1;
	glm::vec3 velocity = glm::vec3(b.velocity.x * b.velocity.x, b.velocity.y * b.velocity.y, b.velocity.z * b.velocity.z);
	b.addForce(velocity*constants, glm::vec3(0.0, 0.0, 0.0));
}

void Gravity::applyForce(Particle& p)
{
	float force_gravity = -9.81f*p.mass;
	p.addForce(glm::vec3(0.0, force_gravity, 0.0));
};

void Gravity::applyForce(RigidBody& b)
{
	GLfloat force_gravity = -9.81*b.mass;
	b.addForce(glm::vec3(0.0, force_gravity, 0.0), glm::vec3(0.0, 0.0, 0.0));
}