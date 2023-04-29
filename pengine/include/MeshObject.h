//
// Created by Adèle Imparato on 10/03/2023.
//

#ifndef MESHOBJECT_H
#define MESHOBJECT_H

#include "SceneObject.h"
#include "RigidBody.h"
#include "BoxCollider.h"

class MeshObject : public SceneObject {

protected:
	RigidBody* rb;
	BoxCollider* collider;

public:
    void create(const std::string & filename);
    RigidBody* getRigidBody();
    BoxCollider* getBoxCollider();
    void setRigidBody(RigidBody* _rb);
    void setBoxCollider(BoxCollider *_collider);


};


#endif //MESHOBJECT_H
