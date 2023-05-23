#include "include/RigidBody.h"

#include <cstdio>

RigidBody::RigidBody(float _mass):mass(_mass){};
RigidBody::~RigidBody(){};

void RigidBody::computeForces(float deltaTime)
{
	glm::vec3 F = glm::vec3(0.0, 0.0, 0.0);
	for (int i = 0; i < forces.size(); ++i)
	{
		F += forces[i];
	}
	float invMass = this->mass > 0.0f ? (1.0/this->mass) : 0.0f;
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

glm::vec3 RigidBody::computeImpulseResponse(glm::vec3 &normal, float C_r_other, float mass_o, glm::vec3 v_o, float mu_k, float mu_s)
{
	glm::vec3 velocityProjection = normal * (glm::dot(this->velocity, normal)/glm::dot(normal, normal));
    float C_r = (glm::length(velocityProjection) <= 0.15f) ? 0.01f : 0.3f;
    glm::vec3 tangent = glm::cross(glm::cross(normal,(this->velocity - v_o)), normal);
    tangent = glm::normalize(tangent);
    float j =  (-(1+C_r) * glm::dot((this->velocity - v_o), normal))/ ((1/this->mass) + 1.0f/mass_o);
    float theta = glm::acos(glm::dot(glm::vec3(0.0, -1.0, 0.0), normal));
    float F_parallel = 9.81f * this->mass * glm::sin(theta);
    float frictionCoeff = ((mu_s * 9.81f * this->mass * glm::cos(theta)) <= glm::abs(F_parallel))? mu_s : mu_k;
    return (j * (normal + frictionCoeff* tangent))* (1/this->mass);
}
glm::vec3 RigidBody::computeImpulseResponse(glm::vec3 &normal, float mass_o, glm::vec3 v_o)
{
	glm::vec3 velocityProjection = normal * (glm::dot(this->velocity, normal)/glm::dot(normal, normal));
    float C_r = 0.0f;
    glm::vec3 tangent = glm::cross(glm::cross(normal,(this->velocity - v_o)), normal);
    tangent = glm::normalize(tangent);
    float j =  (-(1+C_r) * glm::dot((this->velocity - v_o), normal))/ ((1/this->mass) + 1.0f/mass_o);
    return (j * (normal))* (1/this->mass);
}
