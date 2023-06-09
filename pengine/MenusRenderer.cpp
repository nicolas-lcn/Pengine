#include "include/MenusRenderer.hpp"


#include "../common/texture.hpp"
#include "../common/objloader.hpp"
#include "../common/shader.hpp"
#include "../common/text2D.hpp"

#include <string.h>
#include <float.h>

bool hasText = false;
void MenusRenderer::initMenu(int type)
{

	// Initialize Menu parameters (textures, campos)
	switch (type)
	{
		case 0 : initStart(); break;
		case 1 : initPause(); break;
		case 2 : initEnd(); break;
	}
	// Initialize VBO
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &uvBuffer);

	// Initialize Shader
	shaderID = LoadShaders( "../pengine/shaders/MenuVertexShader.glsl", "../pengine/shaders/MenuFragmentShader.glsl" );

	// Initialize uniforms' IDs
	textureUniform = glGetUniformLocation( shaderID, "texture" );
	modelUniform = glGetUniformLocation(shaderID, "model");
	projectionUniform = glGetUniformLocation(shaderID, "projection");
	viewUniform = glGetUniformLocation(shaderID, "view");

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	MenuProjection = glm::perspective(glm::radians(fov), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	MenuView       = glm::lookAt(
								campos,           // Camera is here
								campos+direction, // and looks here : at the same campos, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );
	MenuModel = glm::mat4(1.0f);

	std::vector<glm::vec3> normals;
	loadOBJ("data_off/plane-menu.obj", m_vertices, m_uvs, normals);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), &m_vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_uvs.size() * sizeof(glm::vec2), &m_uvs[0], GL_STATIC_DRAW);
}

void MenusRenderer::cleanMenu()
{
	// Delete buffers
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &uvBuffer);

	// Delete texture
	glDeleteTextures(1, &texture);

	// Delete shader
	glDeleteProgram(shaderID);
}

void MenusRenderer::render()
{
	// Bind shader
	glUseProgram(shaderID);

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Set our "texture" sampler to use Texture Unit 0
	glUniform1i(textureUniform, 0);
	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(MenuModel));
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(MenuProjection));
	glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(MenuView));

	// 1rst attribute buffer : m_vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() );

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	if(hasText)
	{
		float minutes = race_time/60.0f;
		float seconds = race_time - minutes*60.0f;
		std::string timestring =  "00:" + std::to_string((int)minutes) + ":" + std::to_string(int(seconds));
		std::string obstaclesstring = std::to_string(obstacles_encountered);
		// printText2D(timestring.c_str(), 0.0, 0.0, 0.2);
		// printText2D(obstaclesstring.c_str(), 0.0, 1.0, 0.2);

	}
}

void MenusRenderer::initStart()
{
	campos = glm::vec3(0.0, 0.0, 5.0);
	texture = loadBMP_custom("./textures/pengine-menu.bmp");
	hasText = false;
}
void MenusRenderer::initPause()
{
	campos = glm::vec3(0.0, 0.0, 5.0);
	texture = loadBMP_custom("./textures/PENGINE-pause.bmp");
	hasText = false;
}
void MenusRenderer::initEnd()
{
	campos = glm::vec3(0.0, 0.0, 5.0);
	texture = loadBMP_custom("./textures/PENGINE-END.bmp");
	initText2D("./textures/colortext.bmp");
	hasText = true;

}

void MenusRenderer::setStats(float & race_time, int & obstacles_encountered)
{
	this->race_time = race_time;
	this->obstacles_encountered = obstacles_encountered;
}
