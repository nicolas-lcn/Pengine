#include "include/ShaderController.h"
#include "include/GLTexture.h"

void ShaderController::sendMatrices(GLuint programID, glm::mat4 model, glm::mat4 projection, glm::mat4 view)
{
	glUniformMatrix4fv(glGetUniformLocation(programID, "model_matrix"), 1, GL_FALSE, glm::value_ptr(model)); // location, count, transpose, value
    glUniformMatrix4fv(glGetUniformLocation(programID, "view_matrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(programID, "proj_matrix"), 1, GL_FALSE, glm::value_ptr(projection));
}
void ShaderController::sendTextures(GLuint programID, GLTexture *grass_texture, GLTexture *rock_texture, GLTexture *snowrocks_texture, GLTexture *snow_texture)
{
	grass_texture->sendTextureToShader(programID, "texture_grass", 0);
    rock_texture->sendTextureToShader(programID, "texture_rock", 1);
    snowrocks_texture->sendTextureToShader(programID, "texture_snowrocks", 2);
    snow_texture->sendTextureToShader(programID, "texture_snow", 3);
}
