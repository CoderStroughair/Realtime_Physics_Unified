#include <common/RigidBody.h>
#include <common/Forces.h>

class RigidBodySystem {
public:
	vector<RigidBody> bodies;
	Drag d;
	Gravity g;

	RigidBodySystem() {};

	RigidBodySystem(int _numBodies, const char* mesh);

	void applyForces(float delta);

	bool isBoxColliding(RigidBody& a, RigidBody& b);

	bool isSphereColliding(RigidBody& a, RigidBody& b);

	void checkCollisions(const bool &box);

	bool checkInterval(float a1, float a2, float b1, float b2);

	void checkPlaneCollisions(glm::vec3 point, glm::vec3 normal, float delta);
};