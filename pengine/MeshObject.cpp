//
// Created by Adèle Imparato on 10/03/2023.
//

#include "include/MeshObject.h"
#include "common/objloader.hpp"

void MeshObject::create(const std::string & filename)
{
	loadOFF(filename, indexed_vertices, indices, triangles );
	initBoxCollider();
}


