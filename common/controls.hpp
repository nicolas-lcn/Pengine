#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include "../pengine/include/CameraObject.h"

void computeMatricesFromInputs();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
void setCamPosition(glm::vec3 new_pos);
void setVerticalAngle(float new_angle);
glm::vec3 getCamPosition();

void initCameraObject(glm::vec3 t_pos, glm::vec3 t_forward, glm::vec3 t_up, float _spring, float _hdist, float _vdist);
void updateCamera(float deltaTime);
CameraObject* getCamera();

#endif