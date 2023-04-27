//
// Created by Adèle Imparato on 14/02/2023.
//

#ifndef TP1_CODE_CAMERA_H
#define TP1_CODE_CAMERA_H

#define PI 3.14159265

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

    void MVP(bool cameraRotates, bool speedUp, bool slowDown){
        computeMatricesFromInputs();

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

    void scaleModelMat(glm::vec3 scaling_factor){
        Model_Matrix = glm::scale(Model_Matrix, scaling_factor);
    }

    void rotateModelMat(glm::vec3 axis, float increment){
        Model_Matrix = glm::rotate(Model_Matrix,glm::radians(angle_in_degrees+=increment), axis);
    }

    void translateModelMat(glm::vec3 translation_vector){
        Model_Matrix = glm::translate(Model_Matrix, translation_vector);
    }

    void sendMVPtoShader(GLuint programID){
        // Send our transformation to the currently bound shader,
        // in the "Model View Projection" to the shader uniforms
        glUniformMatrix4fv(glGetUniformLocation(programID, "model_matrix"), 1, GL_FALSE, &Model_Matrix[0][0]); // location, count, transpose, value
        glUniformMatrix4fv(glGetUniformLocation(programID, "view_matrix"), 1, GL_FALSE, &View_Matrix[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(programID, "proj_matrix"), 1, GL_FALSE, &Projection_Matrix[0][0]);
    }

};


#endif //TP1_CODE_CAMERA_H
