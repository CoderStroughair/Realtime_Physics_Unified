#pragma once

#include <common/Particle.h>
#include <common/RigidBody.h>

class Force
{
public:
	virtual void applyForce(Particle& p) = 0;
	virtual void applyForce(RigidBody& b) = 0;
};

class Drag : public Force
{
public:
	void applyForce(Particle& p);

	void applyForce(RigidBody& b);
};

class Gravity : public Force
{
public:
	void applyForce(Particle& p);

	void applyForce(RigidBody& b);
};