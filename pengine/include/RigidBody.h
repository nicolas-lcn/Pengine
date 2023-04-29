#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#define EARTH_G 9.81

// Include glm utils
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class RigidBody
{
public:
	RigidBody(float _mass=1.0);
	~RigidBody();
	
	void computeForces(float deltaTime);

	void applyForce(glm::vec3 force);

	glm::vec3 getForcesDirection();

	void setSpeed(glm::vec3 &_speed);
	void setAcceleration(glm::vec3 &_accel);

	glm::vec3 getSpeed();
	glm::vec3 getAccel();
	glm::vec3 computeRebound(glm::vec3 normal);

private:
	glm::vec3 speed;
	glm::vec3 accel;
	std::vector<glm::vec3> others;

	float mass;
};
#endif