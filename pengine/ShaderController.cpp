#include "include/ShaderController.h"
#include "include/GLTexture.h"

void ShaderController::sendMatrices(GLuint programID, glm::mat4 model, glm::mat4 projection, glm::mat4 view)
{
	glUniformMatrix4fv(glGetUniformLocation(programID, "model_matrix"), 1, GL_FALSE, glm::value_ptr(model)); // location, count, transpose, value
    glUniformMatrix4fv(glGetUniformLocation(programID, "view_matrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(programID, "proj_matrix"), 1, GL_FALSE, glm::value_ptr(projection));
}
void ShaderController::sendTextures(GLuint programID, GLTexture *snow_texture, GLTexture *mountain, GLTexture *landscape)
{
    snow_texture->sendTextureToShader(programID, "texture_snow", 3);
    mountain->sendTextureToShader(programID, "texture_mountain", 2);
    landscape->sendTextureToShader(programID, "texture_landscape",4);
}
