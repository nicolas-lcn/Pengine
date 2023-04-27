//
// Created by Adèle Imparato on 13/02/2023.
//

#ifndef TUTORIALS_PLANE_H
#define TUTORIALS_PLANE_H

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

    Plane() {}
    Plane( double wi , double he , int nw, int nh) {
        width = wi;
        height = he;
        h = nw;
        w = nh;

        bottom_left = glm::vec3(center[0]-width/2, center[1], center[2]+height/2);
        bottom_right = glm::vec3(center[0]+width/2, center[1], center[2]+height/2);
        top_right = glm::vec3(center[0]+width/2, center[1], center[2]-height/2);
        top_left = glm::vec3(center[0]-width/2, center[1], center[2]-height/2);
    }

    void setDimension(int nw, int nh){
        h = nw;
        w = nh;
    }

    void addRelief(){
        for(int i = 0; i < indexed_vertices.size(); i++){

            double f = (double)rand() / RAND_MAX;
            double rand_d = -0.2 + f * (0.2 - (-0.2));

            indexed_vertices[i][1] += rand_d;
        }
    }

    void generatePlane(){

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


    void addHeightMap(unsigned char *HM_data, int height_HM, int width_HM){
        double max = 1.0; // maximum height
        double min = 0.0; // minimum height
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

            double ratio = (double)dat/(double)range_ndg_HM;
            //std::cout << "ratio :" << ratio << std::endl;
            double difference = max*ratio;
            //std::cout << "difference :" << difference << std::endl;

            indexed_vertices[i][1] = max - difference;
        }
    }

    double getHeightFromCoords(unsigned char *HM_data, int height_HM, int width_HM, glm::vec3 coords){

        // TODO doesn't work for bottom_right and top_right corners!

        double dist_from_zero_x = center[0] - (width/2.0);
        double ratio_x = (coords[0] - dist_from_zero_x)/width;

        int row_HM = floor(ratio_x*width_HM);

        double dist_from_zero_z = center[2] - (height/2.0);
        double ratio_z = (coords[2] - dist_from_zero_z)/height;

        int col_HM = floor(ratio_z*height_HM);

        int dat = (int) HM_data[row_HM*width_HM + col_HM];

        int range_ndg_HM = 256;
        double max = 1.0; // maximum height
        double ratio = (double)dat/(double)range_ndg_HM;
        double difference = max*ratio;

        return max - difference;
    }


};


#endif //TUTORIALS_PLANE_H
