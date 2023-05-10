//
// Created by Adèle Imparato on 10/03/2023.
//

#ifndef MESHOBJECT_H
#define MESHOBJECT_H

#include "Entity.h"


class MeshObject : public Entity {

public:
	MeshObject(){};
	~MeshObject(){};
    void create(const std::string & filename);
};


#endif //MESHOBJECT_H
