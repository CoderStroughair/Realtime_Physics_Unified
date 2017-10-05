#include <common/RigidBody.h>
#include <common/Forces.h>

class RigidBodySystem {
public:
	vector<RigidBody> bodies;
	int numBodies;
	Drag d;
	Gravity g;

	RigidBodySystem() {};

	RigidBodySystem(int _numBodies, SingleMesh &m, SingleMesh &sphere);

	void applyForces(float delta);

	void checkSphericalCollisions();

	void checkBodyCollisions(RigidBody& a, RigidBody& b);

	bool checkInterval(float a1, float a2, float b1, float b2);

	void checkPlaneCollisions(glm::vec3 point, glm::vec3 normal, float delta);
};