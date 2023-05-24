//
// Created by Adèle Imparato on 09/03/2023.
//

#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <iostream>
#include <glm/glm.hpp>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
#include <vector>

#include "BoxCollider.h"

typedef struct BVHNode
{
    BoxCollider bounds;
    BVHNode* children;
    int numTriangles;
    std::vector<int> triangles;
    BVHNode() : children(0), numTriangles(0){}

}BVHNode;


class SceneObject {
    
public:

    BVHNode* accelerator;


public:

    std::vector<unsigned short> indices; // Triangles concaténés dans une liste
    std::vector<std::vector<unsigned short> > triangles;
    std::vector<glm::vec3> indexed_vertices;
    std::vector<glm::vec2> coord_texture; // texture
    std::vector<glm::vec3> normals;

    GLuint vertexbuffer, elementbuffer, buffer_coord_txt;

    int isTerrain = 0; // 0 is not a terrain, 1 is a slope, 2 is a moutain
    int isBackground = 0; // 0 is not a background, 1 is a background

    glm::vec4 color = glm::vec4(0.0,0.0,0.0,0.0); // default value

    SceneObject();

    void setIsTerrain(int isTerrain);

    void setIsBackground(int isBackground);

    void setColor(glm::vec4 color);

    void draw(GLuint programID) const ;

    void generateBuffers();

    void loadBuffers();

    void deleteBuffers();

    void clearVectors();

    void accelerate();
    void splitBVHNode(BVHNode* node, int depth);
    void freeBVHNode(BVHNode* node);
};


#endif //SCENEOBJECT_H
