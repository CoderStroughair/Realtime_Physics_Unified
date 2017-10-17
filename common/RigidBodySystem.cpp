#include <common/RigidBodySystem.h>
#include <list>

RigidBodySystem::RigidBodySystem(int _numBodies, const char* mesh) {

	for (int i = 0; i < _numBodies; i++)
	{
		SingleMesh m;
		m.init(mesh, BRICK_TEXTURE, NULL);
		m.mode = GL_QUADS;
		RigidBody body({ 0.0f, 0.0f, 0.0f }, 100.0f, { 1.0, 1.0, 1.0 }, m);
		bodies.push_back(body);
	}
};

void RigidBodySystem::applyForces(float delta)
{
	for (int i = 0; i < bodies.size(); i++)
	{
		bodies[i].force = { 0.0, 0.0, 0.0 };
		g.applyForce(bodies[i]);
		d.applyForce(bodies[i]);
		bodies[i].resolveForce(delta);
	}
}

bool RigidBodySystem::isSphereColliding(RigidBody& a, RigidBody& b)
{
	float coRest = 0.6;
	GLfloat distance = glm::length(a.position - b.position);
	GLfloat radii = a.radius + b.radius;
	if (distance < radii)	//first, are their spheres overlapping?
	{
		return true;
	}
	return false;
}

bool RigidBodySystem::isBoxColliding(RigidBody& a, RigidBody& b)
{
	a.createBoundingBox();
	b.createBoundingBox();
	if (checkInterval(a.boundingBox[0], a.boundingBox[3], b.boundingBox[0], b.boundingBox[3]))
	{
		if (checkInterval(a.boundingBox[1], a.boundingBox[4], b.boundingBox[1], b.boundingBox[4]))
		{
			if (checkInterval(a.boundingBox[2], a.boundingBox[5], b.boundingBox[2], b.boundingBox[5]))
			{
				return true;
			}
		}
	}
	return false;
}

void RigidBodySystem::checkCollisions(const bool& box)
{
	for (int i = 0; i < bodies.size(); i++)
	{
		bodies[i].colour = BLUE;
	}
	if (box)
	{
		for (int i = 0; i < bodies.size(); i++)
		{
			for (int j = i + 1; j < bodies.size(); j++)
			{
				if (isBoxColliding(bodies[i], bodies[j]))
				{
					bodies[i].colour = RED;
					bodies[j].colour = RED;
				}					
			}
		}
	}
	else
	{
		for (int i = 0; i < bodies.size(); i++)
		{
			for (int j = i + 1; j < bodies.size(); j++)
			{
				if (isSphereColliding(bodies[i], bodies[j]))
				{
					bodies[i].colour = RED;
					bodies[j].colour = RED;
				}
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
	for (int i = 0; i < bodies.size(); i++)
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