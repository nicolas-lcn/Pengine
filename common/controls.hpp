#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void computeMatricesFromInputs();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
void setCamPosition(glm::vec3 new_pos);
void setVerticalAngle(float new_angle);
glm::vec3 getCamPosition();

#endif