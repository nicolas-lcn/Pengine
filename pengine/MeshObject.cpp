//
// Created by Adèle Imparato on 10/03/2023.
//

#include "include/MeshObject.h"
#include "common/objloader.hpp"
#include "common/vboindexer.hpp"
#include "common/tangentspace.hpp"

#include <cstdio>
void MeshObject::create(const std::string & filename)
{
	if(filename.find(std::string(".off")) != std::string::npos)
		loadOFF(filename, indexed_vertices, indices, triangles );
	else if(filename.find(std::string(".obj")) != std::string::npos)
	{
		loadOBJ(filename.c_str(), indexed_vertices, coord_texture, normals);
		indexVBO_slow(indexed_vertices, coord_texture, normals, indices, indexed_vertices, coord_texture, normals);
		for (int i = 0; i < indices.size(); i+=3)
		{
			std::vector<unsigned short>tri(3);
			tri[0] = indices[i]; tri[1] = indices[i+1]; tri[2] = indices[i+1];
			triangles.push_back((tri));
		}
		// std::vector<glm::vec3> bitangents = std::vector<glm::vec3>();
		// computeTangentBasis(indexed_vertices, coord_texture, normals, tangents, bitangents);
	}

	initBoxCollider();
	accelerate();
}


