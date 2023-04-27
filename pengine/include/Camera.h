//
// Created by Adèle Imparato on 14/02/2023.
//

#ifndef CAMERA_H
#define CAMERA_H

#define PI 3.14159265

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// TODO rename class
class Camera {

private:

public:

    // MVP
    glm::mat4 View_Matrix;
    glm::mat4 Projection_Matrix;
    glm::mat4 Model_Matrix;

    float angle_in_degrees = 1.;

    float increment = 1.0;

    void MVP(bool cameraRotates, bool speedUp, bool slowDown);

    void scaleModelMat(glm::vec3 scaling_factor);

    void rotateModelMat(glm::vec3 axis, float increment);

    void translateModelMat(glm::vec3 translation_vector);

    void sendMVPtoShader(GLuint programID);

};


#endif //CAMERA_H
