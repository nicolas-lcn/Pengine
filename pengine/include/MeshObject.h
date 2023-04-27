//
// Created by Adèle Imparato on 10/03/2023.
//

#ifndef TP1_CODE_MESHOBJECT_H
#define TP1_CODE_MESHOBJECT_H

#include "SceneObject.h"


class MeshObject : public SceneObject {

private:


public:
    void create(const std::string & filename){
        loadOFF(filename, indexed_vertices, indices, triangles );
    }

};


#endif //TP1_CODE_MESHOBJECT_H
