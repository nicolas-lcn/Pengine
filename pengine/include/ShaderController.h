#ifndef SHADERCONTROLLER_H
#define SHADERCONTROLLER_H

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include glm utils
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GLTexture; 

class ShaderController
{
public:
	void sendMatrices(GLuint programID, glm::mat4 model, glm::mat4 projection, glm::mat4 view);
	void sendTextures(GLuint programID, GLTexture *grass_texture, GLTexture *rock_texture, GLTexture *snowrocks_texture, GLTexture *snow_texture);
};
#endif //SHADERCONTROLLER_H