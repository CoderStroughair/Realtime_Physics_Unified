#pragma once

#include <GLM.h>
#include <common/Defines.h>
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

	Particle(SingleMesh& me, glm::vec3 p, float m);
	void addForce(glm::vec3 f);
	void resolveForce(float delta);
	void reset();
	void evolve();
};

class RigidBody {
	glm::vec3 initialposition;

public:
	//constants
	float mass;					//m									
	glm::mat4 ibody;					//									
	glm::mat4 ibodyInv;				//
	SingleMesh mesh;

	glm::vec3 position;				//x(t), ie, the center of mass
	glm::mat4 orientationMat;		//R(t)
	glm::vec3 linMomentum;			//P(t) = M*v(t)
	glm::vec3 angMomentum;			//L(t) = I(t)*w(t)


	glm::mat4 iInv;
	glm::vec3 velocity;				//Linear Velocity v(t) = P(t) / m
	glm::vec3 angVelocity;			//w(t)

	glm::vec3 force;
	glm::vec3 torque;				//rho(t) = d/dt L(t) = SUM((pi - x(t))*fi)

								//mat3 inertiaTensor;		//I(t)	- Varies in World Space:	|	Ixx		Ixy		Ixz	|
								//									|	Iyx		Iyy		Iyz	|
								//									|	Izx		Izy		Izz	|
								//Diagonal Terms:	Ixx = M * IntegralOverV (Y^2 + z^2) dV
								//Off-Diagonal:		Ixy = -M * IntegralOverV (x*y) dV
								//Body Space - I(t) = R(t)IbodyR(T)^T

								//Rate of change of Orientation = w(t)*R(t)  = (d/dt)R(t) = |	 0		-wz(t)	 wy(t)	0	|
								//															|	 wz(t)	 0		-wx(t)	0	|	*	R(t)
								//															|	-wy(t)	 wx(t)	 0		0	|
								//															|	 0		 0		 0		0	|

	RigidBody() {};
	RigidBody(glm::vec3 x, glm::vec3 P, glm::vec3 L, float m, glm::vec3 hdw, SingleMesh &_mesh)
	{
		initialposition = x;

		float a = (1 / 12.0f) * m;
		ibody = glm::mat4(
			a*(hdw.x*hdw.x + hdw.y*hdw.y), 0.0f, 0.0f, 0.0f,
			0.0f, a*(hdw.z*hdw.z + hdw.y*hdw.y), 0.0f, 0.0f,
			0.0f, 0.0f, a*(hdw.z*hdw.z + hdw.x*hdw.x), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
		ibodyInv = glm::inverse(ibody);
		mass = m;
		mesh = _mesh;

		position = x;
		orientationMat = glm::mat4();
		linMomentum = P;
		angMomentum = L;

		velocity = linMomentum / mass;
		iInv = orientationMat * ibodyInv * glm::transpose(orientationMat);
		angVelocity = glm::vec3(iInv * glm::vec4(angMomentum, 1.0));

		force = glm::vec3(0.0, 0.0, 0.0);
		torque = glm::vec3(0.0, 0.0, 0.0);
	}

	void addForce(glm::vec3 f, glm::vec3 location)
	{
		force += f;
		torque += glm::cross(location, force);
	}

	void resolveForce(float delta)
	{
		if (force.x)
			cout << "";
		linMomentum *= 0.7;
		angMomentum *= 0.7;
		linMomentum += force*delta;
		angMomentum += torque*delta;

		velocity = linMomentum / mass;
		iInv = orientationMat * ibodyInv * glm::transpose(orientationMat);
		angVelocity = glm::vec3(iInv * glm::vec4(angMomentum, 1.0f));

		glm::mat4 rDot = star(angVelocity*delta)*orientationMat;
		orientationMat[0][0] += rDot[0][0];
		orientationMat[0][1] += rDot[0][1];
		orientationMat[0][2] += rDot[0][2];
		orientationMat[0][3] = 0;

		orientationMat[1][0] += rDot[1][0];
		orientationMat[1][1] += rDot[1][1];
		orientationMat[1][2] += rDot[1][2];
		orientationMat[1][3] = 0;

		orientationMat[2][0] += rDot[2][0];
		orientationMat[2][1] += rDot[2][1];
		orientationMat[2][2] += rDot[2][2];
		orientationMat[2][3] = 0;

		orientationMat[3][0] += rDot[3][0];
		orientationMat[3][1] += rDot[3][1];
		orientationMat[3][2] += rDot[3][2];
		orientationMat[3][3] = 1;

		//Orthonormalisation
		glm::vec3 Cx = glm::vec3(orientationMat[0][0], orientationMat[0][1], orientationMat[0][2]) / glm::length(glm::vec3(orientationMat[0][0], orientationMat[0][1], orientationMat[0][2]));
		glm::vec3 Cz = glm::vec3(orientationMat[2][0], orientationMat[2][1], orientationMat[2][2]);
		glm::vec3 Cy = glm::cross(Cz, Cx);
		Cy = Cy / glm::length(Cy);
		Cz = glm::cross(Cx, Cy);
		Cz = Cz / glm::length(Cz);
		orientationMat[0][0] = Cx.x;
		orientationMat[0][1] = Cx.y;
		orientationMat[0][2] = Cx.z;

		orientationMat[1][0] = Cy.x;
		orientationMat[1][1] = Cy.y;
		orientationMat[1][2] = Cy.z;

		orientationMat[2][0] = Cz.x;
		orientationMat[2][1] = Cz.y;
		orientationMat[2][2] = Cz.z;

		position += velocity * delta;

		if (force.x)
			cout << "";

		mesh.update_mesh(orientationMat, position);
	}

	glm::mat4 star(glm::vec3 a)
	{
		glm::mat4 star = glm::mat4(
			0, -a.z, a.y, 0,
			a.z, 0, -a.x, 0,
			-a.y, a.x, 0, 0,
			0, 0, 0, 0
		);
		return glm::transpose(star);	//converting matrix into column order
	}

	void reset(glm::vec3 l, glm::vec3 a)
	{
		position = initialposition;
		orientationMat = glm::mat4();
		linMomentum = l;
		angMomentum = a;

		velocity = linMomentum / mass;
		iInv = orientationMat * ibodyInv * glm::transpose(orientationMat);
		angVelocity = glm::vec3(iInv * glm::vec4(angMomentum, 1.0));

		force = glm::vec3(0.0, 0.0, 0.0);
		torque = glm::vec3(0.0, 0.0, 0.0);
	}
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
