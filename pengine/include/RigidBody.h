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

	void setVelocity(glm::vec3 &_velocity);
	void setAcceleration(glm::vec3 &_accel);

	glm::vec3 getVelocity();
	glm::vec3 getAccel();
	glm::vec3 computeRebound(glm::vec3 normal);
	glm::vec3 computeImpulseResponse(glm::vec3 &normal, float C_r_other, float mass_o, glm::vec3 v_o, float mu_k, float mu_s);
	glm::vec3 computeImpulseResponse(glm::vec3 &normal, float C_r, float mass_o, glm::vec3 v_o);

	float mass;


private:
	glm::vec3 velocity;
	glm::vec3 accel;
	std::vector<glm::vec3> forces;

	float C_r;

};
#endif