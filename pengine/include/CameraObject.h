#ifndef CAMERA_OBJECT_H
#define CAMERA_OBJECT_H

// Include glm utils
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class CameraObject
{
private:
	// Horizontal and vertical follow distance
	float hDist, vDist;
	// Spring constant: higher value means stiffer spring
	// A good starting value will vary based on the desired range
	float springConstant;
	// Dampening constant is based on the above
	float dampConstant;

	glm::vec3 velocity, actualPosition;

	// Target position, forward and up vectors
	glm::vec3 m_t_pos, m_t_forward, m_t_up;

	glm::mat4 viewMatrix;

public:

	CameraObject();
	CameraObject(glm::vec3 t_pos, glm::vec3 t_forward, glm::vec3 t_up, float _spring, float _hdist, float _vdist);
	~CameraObject();
	void computeViewMatrix();
	void update(float deltaTime);
	void updateTarget(glm::vec3 t_pos, glm::vec3 t_forward, glm::vec3 t_up);

	glm::mat4 getViewMatrix();

};

#endif