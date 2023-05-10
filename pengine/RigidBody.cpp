#include "include/RigidBody.h"

RigidBody::RigidBody(float _mass):mass(_mass){};
RigidBody::~RigidBody(){};

void RigidBody::computeForces(float deltaTime)
{
	glm::vec3 F = glm::vec3(0.0, 0.0, 0.0);
	for (int i = 0; i < forces.size(); ++i)
	{
		F += forces[i];
	}
	float invMass = mass > 0.0f ? (1.0/mass) : 0.0f;
	glm::vec3 a = invMass * F;
	setAcceleration(a);
	glm::vec3 a_t = deltaTime * a;
	glm::vec3 v_t = velocity + a_t;
	setVelocity(v_t);
	forces.clear();
}

void RigidBody::applyForce(glm::vec3 force)
{
	forces.push_back(force);
}

glm::vec3 RigidBody::getForcesDirection()
{
	glm::vec3 F = glm::vec3(0.0, 0.0, 0.0);
	for (int i = 0; i < forces.size(); ++i)
	{
		F += forces[i];
	}
	F += glm::vec3(0.0, -0.1, 0.0);
	return glm::normalize(F);
}

void RigidBody::setVelocity(glm::vec3 &_velocity){velocity = _velocity;}
void RigidBody::setAcceleration(glm::vec3 &_accel){accel=_accel;}

glm::vec3 RigidBody::getVelocity(){return velocity;}
glm::vec3 RigidBody::getAccel(){return accel;}

glm::vec3 RigidBody::computeRebound(glm::vec3 normal)
{
	return glm::reflect(velocity, normal);
	
}