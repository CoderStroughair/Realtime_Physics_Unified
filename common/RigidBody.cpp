#include "RigidBody.h"
#include "glm/ext.hpp"

RigidBody::RigidBody(glm::vec3 x, float m, glm::vec3 hdw, SingleMesh _mesh)
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
	linMomentum = {0.0f, 0.0f, 0.0f};
	angMomentum = { 0.0f, 0.0f, 0.0f };

	velocity = linMomentum / mass;
	iInv = orientationMat * ibodyInv * glm::transpose(orientationMat);
	angVelocity = glm::vec3(iInv * glm::vec4(angMomentum, 1.0));

	force = glm::vec3(0.0, 0.0, 0.0);
	torque = glm::vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < mesh.mesh_vertex_count; i += 3)
	{
		glm::vec3 vertex = glm::vec3(mesh.newpoints[i], mesh.newpoints[i + 1], mesh.newpoints[i + 2]);
		if (glm::length(vertex) > radius)
		{
			radius = glm::length(vertex);
		}
	}
	mesh.update_mesh(orientationMat, x);
	createBoundingBox();
}

void RigidBody::addForce(glm::vec3 f, glm::vec3 location)
{
	force += f;
	torque += glm::cross(location, force);
}

void RigidBody::resolveForce(float delta)
{
	//linMomentum *= 0.7;
	//angMomentum *= 0.7;
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
	mesh.update_mesh(orientationMat, position);

	force = { 0.0f, 0.0f, 0.0f };
	torque = { 0.0f, 0.0f, 0.0f };
}

glm::mat4 RigidBody::star(glm::vec3 a)
{
	glm::mat4 star = glm::mat4(
		0, -a.z, a.y, 0,
		a.z, 0, -a.x, 0,
		-a.y, a.x, 0, 0,
		0, 0, 0, 0
	);
	return glm::transpose(star);	//converting matrix into column order
}

void RigidBody::reset(glm::vec3 l, glm::vec3 a)
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
	mesh.update_mesh(orientationMat, position);
}

std::string RigidBody::updateString()
{
	string values = "Force: " + glm::to_string(force) + "\n";
	values += "Torque: " + glm::to_string(torque) + "\n";
	values += "Linear Momentum: " + glm::to_string(linMomentum) + "\n";
	values += "Angular Momentum: " + glm::to_string(angMomentum) + "]\n\n";

	values += "Position: " + glm::to_string(position) + "\n\n";
	values += "Orientation Matrix: \n" + glm::to_string(orientationMat[0]) + "\n" + glm::to_string(orientationMat[1]) + "\n" + glm::to_string(orientationMat[2]) + "\n" + glm::to_string(orientationMat[3]) + "\n\n";

	values += "Vertices: \n";
	vector<glm::vec3> vertices;
	for (int i = 0; i < mesh.newpoints.size() / 3; i++)
	{
		vertices.push_back(glm::vec3());
		vertices[i].x = mesh.newpoints[i * 3];
		vertices[i].y = mesh.newpoints[i * 3 + 1];
		vertices[i].z = mesh.newpoints[i * 3 + 2];
		bool add = true;
		for (int j = 0; j < i; j++)
		{
			if (vertices[i] == vertices[j])
			{
				add = false;
				break;
			}
		}
		if (add)
			values += glm::to_string(vertices[i]) + "\n";
	}
	values += "\n";

	return values;
}

void RigidBody::createBoundingBox()
{
	boundingBox[0] = 999;
	boundingBox[1] = 999;
	boundingBox[2] = 999;
	boundingBox[3] = -999;
	boundingBox[4] = -999;
	boundingBox[5] = -999;

	for (int i = 0; i < mesh.newpoints.size(); i += 3)
	{
		if (mesh.newpoints[i] < boundingBox[0])
			boundingBox[0] = mesh.newpoints[i];
		if (mesh.newpoints[i] > boundingBox[3])
			boundingBox[3] = mesh.newpoints[i];

		if (mesh.newpoints[i + 1] < boundingBox[1])
			boundingBox[1] = mesh.newpoints[i + 1];
		if (mesh.newpoints[i + 1] > boundingBox[4])
			boundingBox[4] = mesh.newpoints[i + 1];

		if (mesh.newpoints[i + 2] < boundingBox[2])
			boundingBox[2] = mesh.newpoints[i];
		if (mesh.newpoints[i + 2] > boundingBox[5])
			boundingBox[5] = mesh.newpoints[i + 2];
	}
}

bool RigidBody::operator==(const RigidBody &b)
{
	return (identifier == b.identifier);
}

void RigidBody::setPosition(glm::vec3 position)
{
	initialposition = position;
	reset(glm::vec3(), glm::vec3());
}

void RigidBody::clearMomentum()
{
	linMomentum = { 0.0f, 0.0f, 0.0f };
	angMomentum = { 0.0f, 0.0f, 0.0f };
}