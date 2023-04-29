#include "include/RigidBody.h"

RigidBody::RigidBody(float _mass):mass(_mass){};
RigidBody::~RigidBody(){};

void RigidBody::computeForces(float deltaTime)
{
	glm::vec3 F = glm::vec3(0.0, 0.0, 0.0);
	for (int i = 0; i < others.size(); ++i)
	{
		F += others[i];
	}
	glm::vec3 sumForces = glm::vec3(0.0, -9.81, 0.0) + F;
	float invMass = mass > 0.0f ? (1.0/mass) : 0.0f;
	glm::vec3 a = invMass * sumForces;
	setAcceleration(a);
	glm::vec3 a_t = deltaTime * a;
	glm::vec3 v_t = speed + a_t;
	setSpeed(v_t);
	others = std::vector<glm::vec3>();
}

void RigidBody::applyForce(glm::vec3 force)
{
	others.push_back(force);
}

glm::vec3 RigidBody::getForcesDirection()
{
	glm::vec3 F = glm::vec3(0.0, 0.0, 0.0);
	for (int i = 0; i < others.size(); ++i)
	{
		F += others[i];
	}
	F += glm::vec3(0.0, -0.1, 0.0);
	return glm::normalize(F);
}

void RigidBody::setSpeed(glm::vec3 &_speed){speed = _speed;}
void RigidBody::setAcceleration(glm::vec3 &_accel){accel=_accel;}

glm::vec3 RigidBody::getSpeed(){return speed;}
glm::vec3 RigidBody::getAccel(){return accel;}

glm::vec3 RigidBody::computeRebound(glm::vec3 normal)
{
	return glm::reflect(speed, normal);
	
}