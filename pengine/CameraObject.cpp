#include "include/CameraObject.h"

#include <cstdio>

CameraObject::CameraObject(){};

CameraObject::CameraObject(glm::vec3 t_pos, glm::vec3 t_forward, glm::vec3 t_up, float _spring, float _hdist, float _vdist):
m_t_pos(t_pos), m_t_forward(t_forward), m_t_up(t_up), 
springConstant(_spring), hDist(_hdist), vDist(_vdist)
{
	this->dampConstant = 2.0f * sqrt(springConstant);
	this->actualPosition = t_pos - _hdist * t_forward + _vdist * t_up;
	this->velocity = glm::vec3 (0.0f, 0.0f, 0.0f);
	this->computeViewMatrix();
}
CameraObject::~CameraObject(){};
void CameraObject::computeViewMatrix()
{
	glm::vec3 cameraForward = this->m_t_pos - this->actualPosition;
	cameraForward = glm::normalize(cameraForward);

	glm::vec3 cameraLeft = glm::cross(this->m_t_up, cameraForward);
	cameraLeft = glm::normalize(cameraLeft);

	glm::vec3 cameraUp = glm::cross(cameraForward, cameraLeft);
	cameraUp = glm::normalize(cameraUp);

	this->viewMatrix = glm::lookAt(this->actualPosition, this->m_t_pos, cameraUp);
}
void CameraObject::update(float deltaTime)
{
	glm::vec3 idealPos = this->m_t_pos - this->hDist * this->m_t_forward + this->vDist * this->m_t_up;
	glm::vec3 movement = this->actualPosition - idealPos;
	glm::vec3 springAccel = (-this->springConstant * movement) - (this->dampConstant * this->velocity);
	this->velocity += deltaTime * springAccel;
	this->actualPosition += deltaTime * this->velocity;

	computeViewMatrix();
}

glm::mat4 CameraObject::getViewMatrix(){return this->viewMatrix;};

void CameraObject::updateTarget(glm::vec3 t_pos, glm::vec3 t_forward, glm::vec3 t_up)
{
	this->m_t_pos = t_pos;
	this->m_t_forward = t_forward;
	this->m_t_up = t_up;
}
