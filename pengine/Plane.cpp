//
// Created by Adèle Imparato on 13/02/2023.
//

#include "include/Plane.h"

#include <cfloat>

Plane::Plane() {}
Plane::Plane( double wi , double he , int nw, int nh) {
    width = wi;
    height = he;
    h = nw;
    w = nh;

    bottom_left = glm::vec3(center[0]-width/2, center[1], center[2]+height/2);
    bottom_right = glm::vec3(center[0]+width/2, center[1], center[2]+height/2);
    top_right = glm::vec3(center[0]+width/2, center[1], center[2]-height/2);
    top_left = glm::vec3(center[0]-width/2, center[1], center[2]-height/2);
}

void Plane::setDimension(int nw, int nh){
    h = nw;
    w = nh;
}

void Plane::addRelief(){
    for(int i = 0; i < indexed_vertices.size(); i++){

        double f = (double)rand() / RAND_MAX;
        double rand_d = -0.02 + f * (0.02 - (-0.02));

        indexed_vertices[i][1] += rand_d;
    }
}

void Plane::generatePlane(){

    glm::vec3 start_corner = glm::vec3(center[0]-width/2.0, center[1], center[2]-height/2.0);

    double step_1 = width/(double)w;
    double step_2 = height/(double)h;

    glm::vec3 current_corner;

    // fill indexed_vertices
    for(int i = 0; i <= h; i++) {
        for (int j = 0; j <= w; j++) {
            current_corner = glm::vec3(start_corner[0] + i*step_1, start_corner[1], start_corner[2] + j*step_2);
            glm::vec3 normal = glm::vec3(0.0, 1.0, 0.0);
            coord_texture.push_back(glm::vec2(current_corner[0]/width, 1.0-current_corner[2]/height));
            indexed_vertices.push_back(current_corner);
            normals.push_back(normal);
        }
    }

    for(int i = 0; i < h; i++){
        for(int j = 0; j < w; j++){
            std::vector<unsigned short> triangle1;
            std::vector<unsigned short> triangle2;
            int c1, c2, c3, c4;

            c1 = i + j*(w+1);
            c2 = (i+1) + j*(w+1);
            c3 = i + (j+1)*(w+1);
            c4 = (i+1) + (j+1)*(w+1);

            indices.push_back(c1);
            indices.push_back(c2);
            indices.push_back(c3);

            indices.push_back(c3);
            indices.push_back(c4);
            indices.push_back(c2);

            triangle1.push_back(c1);
            triangle1.push_back(c2);
            triangle1.push_back(c3);

            triangle2.push_back(c3);
            triangle2.push_back(c4);
            triangle2.push_back(c2);

            triangles.push_back(triangle1);
            triangles.push_back(triangle2);
        }
    }
    
}


void Plane::addHeightMap(unsigned char *HM_data, int height_HM, int width_HM){
    float max = 1.0; // maximum height
    float min = 0.0; // minimum height
    // TODO adapt to any max and min value -> shader

    int height_plane = h + 1;
    int width_plane = w + 1;

    int range_ndg_HM = 256;

    //int count = 0;
    //int line = 0;
    for(int i = 0; i < indexed_vertices.size(); i++){

        /*if(count%10 == 0){
            line++;
            std::cout << "LINE " << line << std::endl;
        }
        count++;*/

        //int dat = (int) HM_data[i]
        int row = floor(i / height_plane);
        float row_plane = (float)row/(float)height_plane;
        int row_HM = floor(height_HM*row_plane);
        int col = i - row*width_plane;
        float col_plane = (float)col/(float)width_plane;
        int col_HM = floor(width_HM*col_plane);
        int dat = (int) HM_data[row_HM*width_HM + col_HM];
        //std::cout << "data HeightMap :" << dat << std::endl;

        float ratio = (float)dat/(float)range_ndg_HM;
        //std::cout << "ratio :" << ratio << std::endl;
        float difference = max*ratio;
        //std::cout << "difference :" << difference << std::endl;

        indexed_vertices[i][1] = max - difference;
    }
    updateNormals();
    //accelerate();
}

double Plane::getHeightFromCoords(unsigned char *HM_data, int height_HM, int width_HM, glm::vec3 coords){

    // TODO doesn't work for bottom_right and top_right corners!

    double dist_from_zero_x = transform.getLocalPosition()[0] - (width/2.0);
    double ratio_x = (coords[0] - dist_from_zero_x)/width;

    int row_HM = floor(ratio_x*width_HM);

    double dist_from_zero_z = transform.getLocalPosition()[2] - (height/2.0);
    double ratio_z = (coords[2] - dist_from_zero_z)/height;

    int col_HM = floor(ratio_z*height_HM);

    int dat = (int) HM_data[row_HM*width_HM + col_HM];

    int range_ndg_HM = 256;
    double max = 1.0; // maximum height
    double ratio = (double)dat/(double)range_ndg_HM;
    double difference = max*ratio;

    return max - difference;
}

float Plane::getHeightFromCoords(glm::vec3 coords)
{
    //glm::vec4 pos4 = glm::inverse(transform.getWorldMatrix()) * glm::vec4(coords,1);
    float minDistance = FLT_MAX;
    //glm::vec3 pos = glm::vec3(pos4.x, pos4.y, pos4.z);
    int closest = -1;
    for (int i = 0; i < indexed_vertices.size(); ++i)
    {
        glm::vec3 planePos = indexed_vertices[i];
        float distance = glm::length(coords - planePos);
        if( distance < minDistance)
        {
            minDistance = distance;
            closest = i;
        }
    }
    return indexed_vertices[closest].y;
}

glm::vec3 Plane::getNormalFromCoords(glm::vec3 coords)
{
    //glm::vec4 pos4 = glm::inverse(transform.getWorldMatrix()) * glm::vec4(coords,1);
    float minDistance = FLT_MAX;
    //glm::vec3 pos = glm::vec3(pos4.x, pos4.y, pos4.z);
    int closest = -1;
    for (int i = 0; i < indexed_vertices.size(); ++i)
    {
        glm::vec3 planePos = indexed_vertices[i];
        float distance = glm::length(coords - planePos);
        if( distance < minDistance)
        {
            minDistance = distance;
            closest = i;
        }
    }
    return glm::normalize(glm::vec3(transform.getModelMatrix() * glm::vec4(normals[closest],1.0f)));
}

void Plane::updateNormals() {
    // Reset normals
    for (int i = 0; i < normals.size(); i++) {
        normals[i] = glm::vec3(0.0, 0.0, 0.0);
    }

    // Compute face normals
    for (int i = 0; i < triangles.size(); i++) {
        const std::vector<unsigned short>& triangle = triangles[i];
        unsigned short index1 = triangle[0];
        unsigned short index2 = triangle[1];
        unsigned short index3 = triangle[2];

        glm::vec3 v1 = indexed_vertices[index1];
        glm::vec3 v2 = indexed_vertices[index2];
        glm::vec3 v3 = indexed_vertices[index3];

        glm::vec3 faceNormal = glm::normalize(glm::cross(v2 - v1, v3 - v1));

        normals[index1] += faceNormal;
        normals[index2] += faceNormal;
        normals[index3] += faceNormal;
    }

    // Normalize vertex normals
    for (int i = 0; i < normals.size(); i++) {
        normals[i] = glm::normalize(normals[i]);
    }
}

