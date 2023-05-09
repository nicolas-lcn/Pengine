//
// Created by Adèle Imparato on 14/02/2023.
//

#include "include/GLTexture.h"
#include "include/stb_image.h"


void GLTexture::generateTexture()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void GLTexture::loadTexture(char * path)
{
    data = stbi_load(path, &width , &height , &nrChannels , 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void GLTexture::defineParameters()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void GLTexture::sendTextureToShader(GLuint programID, const GLchar *texture_shader_name, int indexActiveTexture)
{
    //glBindBuffer(GL_ARRAY_BUFFER, buffer_coord_txt);
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2, (void *) 0);
    glActiveTexture(GL_TEXTURE0 + indexActiveTexture);
    glBindTexture(GL_TEXTURE_2D , texture);
    GLuint location = glGetUniformLocation(programID, texture_shader_name);
    glUniform1i(location, indexActiveTexture);
    glEnableVertexAttribArray(2);
}