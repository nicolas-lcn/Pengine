//
// Created by Adèle Imparato on 13/02/2023.
//

#ifndef PLANE_H
#define PLANE_H

#include <vector>
#include <iostream>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SceneObject.h"

class Plane : public SceneObject {

private:

public:

    double width;
    double height;

    int w;
    int h;

    glm::vec3 center = glm::vec3(0.0,0.0,0.0); // default value

    glm::vec3 bottom_right, bottom_left, top_right, top_left;

    Plane();
    Plane( double wi , double he , int nw, int nh);

    void setDimension(int nw, int nh);

    void addRelief();

    void generatePlane();


    void addHeightMap(unsigned char *HM_data, int height_HM, int width_HM);
    double getHeightFromCoords(unsigned char *HM_data, int height_HM, int width_HM, glm::vec3 coords);
    glm::vec3 getNormalFromCoords(glm::vec3 coords);
    float getSlopeAngleFromCoords(unsigned char *HM_data, int height_HM, int width_HM, glm::vec3 coords);
    void updateNormals();
};


#endif //PLANE_H
