//
// Created by Adèle Imparato on 14/02/2023.
//

#ifndef TP1_CODE_GLTEXTURE_H
#define TP1_CODE_GLTEXTURE_H

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
#include "Texture.h"


class GLTexture : public Texture {

private:
    unsigned int texture;
    //GLuint buffer_coord_txt;

public:

    void generateTexture();

    void loadTexture(char * path);

    void defineParameters();

    void sendTextureToShader(GLuint programID, const GLchar *texture_shader_name, int indexActiveTexture);

};


#endif //TP1_CODE_GLTEXTURE_H
