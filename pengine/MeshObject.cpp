//
// Created by Adèle Imparato on 10/03/2023.
//

#include "include/MeshObject.h"
#include "common/objloader.hpp"

void MeshObject::create(const std::string & filename)
{
	loadOFF(filename, indexed_vertices, indices, triangles );
}

void MeshObject::setRigidBody(RigidBody* _rb){this->rb = _rb;}
void MeshObject::setBoxCollider(BoxCollider* _collider){this->collider = _collider;}

RigidBody* MeshObject::getRigidBody(){return this->rb;}
BoxCollider* MeshObject::getBoxCollider(){return this->collider;}
