#include <common/RigidBodySystem.h>
#include <list>

RigidBodySystem::RigidBodySystem(int _numBodies, SingleMesh &m, SingleMesh &sphere) {
	numBodies = _numBodies;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			RigidBody body({ i * 2, 15.0f, j * 2 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, 100.0f, { 1.0, 1.0, 1.0 }, m, sphere);
			bodies.push_back(body);
		}
	}
};

void RigidBodySystem::applyForces(float delta)
{
	for (int i = 0; i < numBodies; i++)
	{
		bodies[i].force = { 0.0, 0.0, 0.0 };
		g.applyForce(bodies[i]);
		//d.applyForce(bodies[i]);
		bodies[i].torque = { -1.0, 5.0, 5.0 };
		bodies[i].resolveForce(delta);
	}
}

void RigidBodySystem::checkSphericalCollisions()
{
	for (int i = 0; i < numBodies; i++)
	{
		bodies[i].colour = BLUE;
	}
	float coRest = 0.6;
	for (int i = 0; i < numBodies; i++)
	{
		for (int j = i + 1; j < numBodies; j++)
		{
			if (i == j)
				continue;
			GLfloat distance = glm::length(bodies[i].position - bodies[j].position);
			GLfloat radii = bodies[i].radius + bodies[j].radius;
			if (distance < radii)	//first, are their spheres overlapping?
			{
				bodies[i].colour = RED;
				bodies[j].colour = RED;
				//Next, checking if the boundary boxes overlap. If they do, turn the colour purple instead of red.
				checkBodyCollisions(bodies[i], bodies[j]);
			}
		}
	}
}

void RigidBodySystem::checkBodyCollisions(RigidBody& a, RigidBody& b)
{
	a.createBoundingBox();
	b.createBoundingBox();
	if (checkInterval(a.boundingBox[0], a.boundingBox[3], b.boundingBox[0], b.boundingBox[3]))
	{
		if (checkInterval(a.boundingBox[1], a.boundingBox[4], b.boundingBox[1], b.boundingBox[4]))
		{
			if (checkInterval(a.boundingBox[2], a.boundingBox[5], b.boundingBox[2], b.boundingBox[5]))
			{
				a.colour = PURPLE;
				b.colour = PURPLE;
			}
		}
	}
}
bool RigidBodySystem::checkInterval(float a1, float a2, float b1, float b2)
{
	pair<float, float> a(a1, a2);
	pair<float, float> b(b1, b2);
	list<pair<float, float>> blist;
	blist.push_back(a);
	blist.push_back(b);
	blist.sort([](const pair<float, float> &a, const pair<float, float> &b) {return a.first < b.first; });
	return(blist.front().second > blist.back().first);
}
void RigidBodySystem::checkPlaneCollisions(glm::vec3 point, glm::vec3 normal, float delta)
{
	float coRest = 0.0;
	for (int i = 0; i < numBodies; i++)
	{
		if (glm::dot((bodies[i].position - point), normal) <= bodies[i].radius + 0.00001f && glm::dot(bodies[i].linMomentum, normal) < 0.00001f)
		{
			glm::vec3 deltaX = bodies[i].position - normal * 1.4f * glm::dot((bodies[i].position - point), normal);
			bodies[i].position = deltaX;
			bodies[i].linMomentum -= (normal*glm::dot(bodies[i].linMomentum, normal))*(1 + coRest);
		}
		/*if (dot((bodies[i].position - point), normal) < 0.00001f && dot(bodies[i].linMomentum, normal) < 0.00001f)
		{
		vec3 deltaX = bodies[i].position - normal * 1.4 * dot((bodies[i].position - point), normal);
		bodies[i].position = deltaX;
		bodies[i].linMomentum = bodies[i].linMomentum - (normal*dot(bodies[i].linMomentum, normal))*(1 + coRest);
		}*/
	}
}