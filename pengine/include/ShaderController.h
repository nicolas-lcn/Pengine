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
	void sendTextures(GLuint programID, GLTexture *snow_texture, GLTexture *mountain, GLTexture *landscape);
};
#endif //SHADERCONTROLLER_H