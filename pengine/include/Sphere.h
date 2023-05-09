//
// Created by Adèle Imparato on 16/03/2023.
//

#ifndef SPHERE_H
#define SPHERE_H

#include "MeshObject.h"

#define GRAVITY 8

class Sphere : public MeshObject {
public:
    glm::vec3 m_center;
    float m_radius;

    // default resolution
    unsigned int resolution = 20;

    // resolutions depending on distance from camera
    const unsigned int back_resolution = 10;
    const unsigned int middle_resolution = 20;
    const unsigned int front_resolution = 40;

    std::vector<std::vector<unsigned short> > indices_Res= std::vector<std::vector<unsigned short> >(3);
    std::vector<std::vector<std::vector<unsigned short> > > triangles_Res =  std::vector<std::vector<std::vector<unsigned short> > >(3);
    std::vector<std::vector<glm::vec3> > indexed_vertices_Res = std::vector<std::vector<glm::vec3> >(3);
    std::vector<std::vector<glm::vec2> > coord_texture_Res = std::vector<std::vector<glm::vec2> >(3);
    std::vector<std::vector<glm::vec3> > normals_Res = std::vector<std::vector<glm::vec3> >(3);

    Sphere();

    Sphere(glm::vec3 c, float r);

    void setResolution(unsigned int new_resolution);

    void build_arrays_for_resolutions();

    void build_arrays();

    void switchResolution(int resolution_index);

    glm::vec3 SphericalCoordinatesToEuclidean( float theta , float phi );

    void update(float deltaTime) override;
};


#endif //SPHERE_H
