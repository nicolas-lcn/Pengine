//
// Created by Adèle Imparato on 14/03/2023.
//

#include "include/Transform.h"
#include "include/SceneGraph.h"
#include "include/Camera.h"
#include "include/GLTexture.h"
// general method to update scene graph
void Transform::updateGraph(SceneGraph root, GLuint programID, Camera *camera, GLTexture *snow_texture){
    SceneObject *obj = root.getData();

    // transform Model matrix
    for(int i = 0; i < obj->transformations.size(); i++){
        if(obj->index_transf[i]==0){
            camera->scaleModelMat(obj->transformations[i]);
        }else if(obj->index_transf[i]==1){
            //std::cout << obj->transformations[i][0] << "," << obj->transformations[i][1] << "," << obj->transformations[i][2] << std::endl;
            camera->translateModelMat(obj->transformations[i]);
        }else if(obj->index_transf[i]==2){
            camera->rotateModelMat(obj->transformations[i] , 0.07);
        }
    }


    // update MVP matrix
    camera->sendMVPtoShader(programID);

    if(obj->isTerrain==1){ // terrain
        // send textures to shader
        snow_texture->sendTextureToShader(programID, "texture_snow", 0);
    }

    obj->loadBuffers();
    obj->draw(programID);

    for(SceneGraph *child : root.getChildren()){
        SceneObject *child_obj = child->getData();
        updateGraph(*child, programID, camera, snow_texture);
    }
}
