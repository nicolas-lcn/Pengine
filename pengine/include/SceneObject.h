//
// Created by Adèle Imparato on 09/03/2023.
//

#ifndef TP1_CODE_SCENEOBJECT_H
#define TP1_CODE_SCENEOBJECT_H

#include <iostream>
#include <glm/glm.hpp>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
#include <vector>



class SceneObject {

private:

public:

    std::vector<unsigned short> indices; // Triangles concaténés dans une liste
    std::vector<std::vector<unsigned short> > triangles;
    std::vector<glm::vec3> indexed_vertices;
    std::vector<glm::vec2> coord_texture; // texture
    std::vector<glm::vec3> normals;

    GLuint vertexbuffer, elementbuffer, buffer_coord_txt;

    int isTerrain = 0; // 0 is not a terrain, 1 is a terrain

    glm::vec4 color = glm::vec4(0.0,0.0,0.0,0.0); // default value

    // transformations
    std::vector<glm::vec3> transformations;
    std::vector<int> index_transf; // 0 for scaling, 1 for translation, 2 for rotation

    SceneObject();

    void setIsTerrain(int isTerrain);

    void setColor(glm::vec4 color);

    void draw(GLuint programID) const ;

    void generateBuffers();

    void loadBuffers();

    void deleteBuffers();

    void clearVectors();

};


#endif //TP1_CODE_SCENEOBJECT_H
