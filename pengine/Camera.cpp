//
// Created by Adèle Imparato on 14/02/2023.
//

#include "include/Camera.h"
#include "common/controls.hpp"

void Camera::MVP(bool cameraRotates, bool speedUp, bool slowDown){

	View_Matrix = getViewMatrix();
	Model_Matrix = glm::mat4(); // identity matrix (model will be at the origin) then change
	Projection_Matrix = getProjectionMatrix();

	if(cameraRotates){
		if(speedUp){
			increment += 1.0;
		}else if(slowDown){
			increment /= 1.3;
		}
		Model_Matrix = glm::rotate(Model_Matrix,glm::radians(angle_in_degrees+=increment), glm::vec3(0.0,1.0,0.0));
	}
}

void Camera::scaleModelMat(glm::vec3 scaling_factor){
	Model_Matrix = glm::scale(Model_Matrix, scaling_factor);
}

void Camera::rotateModelMat(glm::vec3 axis, float increment){
    Model_Matrix = glm::rotate(Model_Matrix,glm::radians(angle_in_degrees+=increment), axis);
}

void Camera::translateModelMat(glm::vec3 translation_vector){
    Model_Matrix = glm::translate(Model_Matrix, translation_vector);
}

void Camera::sendMVPtoShader(GLuint programID){
    // Send our transformation to the currently bound shader,
    // in the "Model View Projection" to the shader uniforms
    glUniformMatrix4fv(glGetUniformLocation(programID, "model_matrix"), 1, GL_FALSE, &Model_Matrix[0][0]); // location, count, transpose, value
    glUniformMatrix4fv(glGetUniformLocation(programID, "view_matrix"), 1, GL_FALSE, &View_Matrix[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programID, "proj_matrix"), 1, GL_FALSE, &Projection_Matrix[0][0]);
}