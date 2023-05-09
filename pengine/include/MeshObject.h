//
// Created by Adèle Imparato on 10/03/2023.
//

#ifndef MESHOBJECT_H
#define MESHOBJECT_H

#include "SceneObject.h"


class MeshObject : public SceneObject {

public:
	MeshObject(){};
	~MeshObject(){};
    void create(const std::string & filename);
};


#endif //MESHOBJECT_H
