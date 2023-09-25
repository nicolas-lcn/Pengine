#include "include/ShaderController.h"
#include "include/GLTexture.h"

glm::vec3 m_lightPos = {0.0, 4.0, 0.0};
glm::vec3 m_lightDiffuse = {1.0, 1.0, 1.0};
glm::vec3 m_lightSpecular = {1.0, 1.0, 1.0};
glm::vec3 m_ambient = {1.0, 1.0, 1.0};
glm::vec3 m_diffuse = {1.0, 1.0, 1.0};
glm::vec3 m_specular = {1.0, 1.0, 1.0};
float m_shininess = 16.0f;

void ShaderController::sendMatrices(GLuint programID, glm::mat4 model, glm::mat4 projection, glm::mat4 view)
{
	glUniformMatrix4fv(glGetUniformLocation(programID, "model_matrix"), 1, GL_FALSE, glm::value_ptr(model)); // location, count, transpose, value
    glUniformMatrix4fv(glGetUniformLocation(programID, "view_matrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(programID, "proj_matrix"), 1, GL_FALSE, glm::value_ptr(projection));

    GLint lightpos = glGetUniformLocation(programID, "light.pos");
    glUniform3fv(lightpos, 1, glm::value_ptr(m_lightPos));
    GLint lightDiffuse = glGetUniformLocation(programID, "light.diffuse");
    glUniform3fv(lightDiffuse, 1, glm::value_ptr(m_lightDiffuse));
    GLint lightSpecular = glGetUniformLocation(programID,"light.specular");
    glUniform3fv(lightSpecular, 1, glm::value_ptr(m_lightSpecular));
    GLint ambient = glGetUniformLocation(programID,"material.ambient");
    glUniform3fv(ambient, 1, glm::value_ptr(m_ambient));
    GLint diffuse = glGetUniformLocation(programID,"material.diffuse");
    glUniform3fv(diffuse, 1, glm::value_ptr(m_diffuse));
    GLint specular = glGetUniformLocation(programID,"material.specular");
    glUniform3fv(specular, 1, glm::value_ptr(m_specular));
    GLint shininess = glGetUniformLocation(programID,"material.shininess");
    glUniform1f(shininess, m_shininess);
}
void ShaderController::sendTextures(GLuint programID, GLTexture *snow_texture, GLTexture *mountain, GLTexture *landscape)
{
    snow_texture->sendTextureToShader(programID, "texture_snow", 3);
    mountain->sendTextureToShader(programID, "texture_mountain", 2);
    landscape->sendTextureToShader(programID, "texture_landscape",4);
}
