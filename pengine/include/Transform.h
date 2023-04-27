//
// Created by Adèle Imparato on 14/03/2023.
//

#ifndef TP1_CODE_TRANSFORM_H
#define TP1_CODE_TRANSFORM_H

#define PI 3.14159265

class Transform {

public:

    // general method to update scene graph
    void updateGraph(SceneGraph root, GLuint programID, Camera *camera,
                     GLTexture *grass_texture, GLTexture *rock_texture, GLTexture *snowrocks_texture, GLTexture *sun_texture){
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
            grass_texture->sendTextureToShader(programID, "texture_grass", 0);
            rock_texture->sendTextureToShader(programID, "texture_rock", 1);
            snowrocks_texture->sendTextureToShader(programID, "texture_snowrocks", 2);
        }else{
            sun_texture->sendTextureToShader(programID, "texture_sun", 3);
        }

        obj->loadBuffers();
        obj->draw(programID);

        for(SceneGraph *child : root.getChildren()){
            SceneObject *child_obj = child->getData();
            updateGraph(*child, programID, camera, grass_texture, rock_texture, snowrocks_texture, sun_texture);
        }
    }

};


#endif //TP1_CODE_TRANSFORM_H
