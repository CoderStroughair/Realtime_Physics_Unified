#pragma once

#include <GLM.h>
#include <vector>
#include <common/SingleMeshLoader.h>
#include <common/Defines.h>

class RigidBody {
	glm::vec3 initialposition;
	int identifier;

public:
	glm::vec3 colour = BLUE;
	//constants
	float mass;						//m									
	glm::mat4 ibody;				//									
	glm::mat4 ibodyInv;				//
	SingleMesh mesh;

	glm::vec3 position;				//x(t), ie, the center of mass
	glm::mat4 orientationMat;		//R(t)
	glm::vec3 linMomentum;			//P(t) = M*v(t)
	glm::vec3 angMomentum;			//L(t) = I(t)*w(t)
	float radius = 0;

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

	float boundingBox[6];

	RigidBody() {};
	RigidBody(glm::vec3 x, float m, glm::vec3 hdw, SingleMesh _mesh);

	void addForce(glm::vec3 f, glm::vec3 location);

	void resolveForce(float delta);

	glm::mat4 star(glm::vec3 a);

	void reset(glm::vec3 l, glm::vec3 a);

	string updateString();

	void createBoundingBox();

	bool operator==(const RigidBody &b);

	void setPosition(glm::vec3 position);

	void clearMomentum();
};