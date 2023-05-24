#ifndef MENUS_HPP
#define MENUS_HPP


#include <vector>
#include <GL/glew.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class MenusRenderer
{

protected:
	GLuint texture;
	GLuint vertexBuffer;
	GLuint uvBuffer;

	GLuint shaderID;
	GLuint textureUniform;

	GLuint modelUniform;
	GLuint projectionUniform;
	GLuint viewUniform;

	glm::mat4 MenuModel, MenuView, MenuProjection;
	glm::vec3 campos = glm::vec3(0.0, 1.0, 1.0);
	glm::vec3 direction = glm::vec3(0.0, -1.0, -1.0);
	glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);

	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec2> m_uvs;
	float fov = 45.0f;

public:
	void initMenu(int type); // 0 -> Start Menu / 1 -> Pause Menu / 2 -> End Menu
	void render();
	void cleanMenu();

private:
	void initStart();
	void initPause();
	void initEnd();
};

#endif