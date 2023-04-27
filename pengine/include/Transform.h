//
// Created by Adèle Imparato on 14/03/2023.
//

#ifndef TRANSFORM_H
#define TRANSFORM_H


// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
#define PI 3.14159265

class SceneGraph;
class Camera;
class GLTexture;

class Transform {

public:

    // general method to update scene graph
    void updateGraph(SceneGraph root, GLuint programID, Camera *camera,
                     GLTexture *grass_texture, GLTexture *rock_texture, GLTexture *snowrocks_texture, GLTexture *sun_texture);

};


#endif //TRANSFORM_H
