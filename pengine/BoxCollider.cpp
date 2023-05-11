#include "../common/geometry_utils.h" 
#include "include/BoxCollider.h"
#include "include/Plane.h"
#include "include/MeshObject.h"
#include <vector>
#include <cstdio>
#include <algorithm>
#include <cfloat>
#include <list>


BoxCollider::BoxCollider(){};


BoxCollider::BoxCollider(std::vector<glm::vec3> vertices)
{
	float min_x,min_y,min_z;
	float max_x, max_y, max_z;
	min_x = max_x = vertices[0][0]; 
	min_y = max_y = vertices[0][1];
	min_z = max_z = vertices[0][2];
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		if(vertices[i][0] <= min_x) min_x = vertices[i][0];
		else if(vertices[i][0] >= max_x) max_x = vertices[i][0];

		if(vertices[i][1] <= min_y) min_y = vertices[i][1];
		else if(vertices[i][1] >= max_y) max_y = vertices[i][1];

		if(vertices[i][2] <= min_z) min_z = vertices[i][2];
		else if(vertices[i][2] >= max_z) max_z = vertices[i][2];
	}
	m_a = glm::vec3(min_x-0.001f, min_y-0.001f, min_z-0.001f);
	m_b = glm::vec3(max_x+0.001f, max_y+0.001f, max_z+0.001f);
	m_center = (m_a + m_b) * 0.5f;
	m_size = (m_b - m_a) * 0.5f;

}

BoxCollider::BoxCollider(glm::vec3 bbmin, glm::vec3 bbmax):m_a(bbmin), m_b(bbmax)
{
	m_center = (bbmin + bbmax) * 0.5f;
	m_size = (bbmax - bbmin) * 0.5f;
}

BoxCollider::~BoxCollider(){}

void BoxCollider::reset(std::vector<glm::vec3> vertices)
{
	float min_x,min_y,min_z;
	float max_x, max_y, max_z;
	min_x = max_x = vertices[0][0]; 
	min_y = max_y = vertices[0][1];
	min_z = max_z = vertices[0][2];
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		if(vertices[i][0] <= min_x) min_x = vertices[i][0];
		else if(vertices[i][0] >= max_x) max_x = vertices[i][0];

		if(vertices[i][1] <= min_y) min_y = vertices[i][1];
		else if(vertices[i][1] >= max_y) max_y = vertices[i][1];

		if(vertices[i][2] <= min_z) min_z = vertices[i][2];
		else if(vertices[i][2] >= max_z) max_z = vertices[i][2];
	}
	m_a = glm::vec3(min_x-0.001f, min_y-0.001f, min_z-0.001f);
	m_b = glm::vec3(max_x+0.001f, max_y+0.001f, max_z+0.001f);
	m_center = (m_a + m_b) * 0.5f;
	m_size = (m_b - m_a) * 0.5f;
}

void BoxCollider::applyMatrix(glm::mat4 model)
{
	glm::vec4 model_a = model * glm::vec4(m_a,1);
	glm::vec4 model_b = model * glm::vec4(m_b,1);

	m_a = glm::vec3(model_a[0], model_a[1], model_a[2]); 
	m_b = glm::vec3(model_b[0], model_b[1], model_b[2]); 
}

glm::vec3 BoxCollider::getA(){return m_a;}
glm::vec3 BoxCollider::getB(){return m_b;}
glm::vec3 BoxCollider::getCenter(){return m_center;}
glm::vec3 BoxCollider::getSize(){return m_size;}

void BoxCollider::setA(glm::vec3 bbmin){ 
	m_a = bbmin;
	m_center = (m_a + m_b) * 0.5f;
	m_size = (m_b - m_a) * 0.5f;
}
void BoxCollider::setB(glm::vec3 bbmax){ 
	m_b = bbmax;
	m_center = (m_a + m_b) * 0.5f;
	m_size = (m_b - m_a) * 0.5f;
}

bool BoxCollider::collides(BoxCollider* other, glm::vec3 &intersection, glm::vec3 &normal, float & depth)
{
	if ((m_a.x <= other->getB().x && m_b.x >= other->getA().x) &&
			(m_a.y <= other->getB().y && m_b.y >= other->getA().y) &&
			(m_a.z <= other->getB().z && m_b.z >= other->getA().z))
	{
		glm::vec3 intersectionMin(
			std::max(m_a.x, other->getA().x),
			std::max(m_a.y, other->getA().y),
			std::max(m_a.z, other->getA().z)
			);
		glm::vec3 intersectionMax(
			std::min(m_b.x, other->getB().x),
			std::min(m_b.y, other->getB().y),
			std::min(m_b.z, other->getB().z)
			);
		intersection = (intersectionMin + intersectionMax) * 0.5f;

		glm::vec3 penetration = intersectionMax - intersectionMin;
		float minPenetration = FLT_MAX;
		int minAxis = -1;
		if(penetration.x < minPenetration)
		{
			minPenetration = penetration.x;
			minAxis = 0; //X
		}
		if(penetration.y < minPenetration)
		{
			minPenetration = penetration.y;
			minAxis = 1; //Y
		}
		if(penetration.z < minPenetration)
		{
			minPenetration = penetration.z;
			minAxis = 2; //Z
		}
		normal = glm::vec3(0.0f);
		normal[minAxis] = (m_a[minAxis] < other->getA()[minAxis])? -1.0f : 1.0f;
		depth = minPenetration;

		return true;
	}
	else return false;
}

bool BoxCollider::collides(Plane* plane, glm::vec3 &normal, float  & depth)
{
	if(plane->accelerator == 0)
	{
		for (int i = 0; i < plane->triangles.size(); ++i)
		{
			std::vector<glm::vec3> t = {
                    plane->indexed_vertices[plane->triangles[i][0]],
                    plane->indexed_vertices[plane->triangles[i][1]],
                    plane->indexed_vertices[plane->triangles[i][2]]
                };
            for (int i = 0; i < 3; ++i)
            {
            	t[i] = glm::vec3(plane->transform.getModelMatrix() * glm::vec4(t[i], 1.0f));
            }
            if(this->collides(t)) {
            	normal = glm::normalize(glm::cross(t[1] - t[0], t[2] - t[0]));
            	return true;
            	
            }


		}
	}
	else
	{
		std::list<BVHNode*> toProcess;
		toProcess.push_front(plane->accelerator);
		while(!toProcess.empty())
		{
			BVHNode* iterator = *(toProcess.begin());
			toProcess.erase(toProcess.begin());

			if(iterator->numTriangles >=0)
			{
				for (int i = 0; i < iterator->numTriangles; ++i)
				{
					std::vector<glm::vec3> t = {
                    plane->indexed_vertices[plane->triangles[iterator->triangles[i]][0]],
                    plane->indexed_vertices[plane->triangles[iterator->triangles[i]][1]],
                    plane->indexed_vertices[plane->triangles[iterator->triangles[i]][2]]
                	};
					if(this->collides(t)) return true;
				}
			}
			if(iterator->children != 0)
			{
				for (int i = 8-1; i >= 0; --i)
				{
					glm::vec3 intersection; //unused;
					glm::vec3 normal; //unused
					float depth; //unused
					if(this->collides(&(iterator->children[i].bounds), intersection, normal, depth))
						toProcess.push_front(&iterator->children[i]);
				}
			}
		}
	}
	return false;
}

bool BoxCollider::collides(std::vector<glm::vec3> &triangle)
{
	//triangle edges
	glm::vec3 f0 = triangle[1] - triangle[0];
	glm::vec3 f1 = triangle[2] - triangle[1];
	glm::vec3 f2 = triangle[0] - triangle[2];

	//normals of collider
	glm::vec3 u0(1.0f, 0.0f, 0.0f); // Positive x-axis
	glm::vec3 u1(0.0f, 1.0f, 0.0f); // Positive y-axis
	glm::vec3 u2(0.0f, 0.0f, 1.0f); // Positive z-axis

	std::vector<glm::vec3> test {
		u0, u1, u2,
		glm::cross(f0, f1),
		glm::cross(u0, f0), glm::cross(u0, f1), glm::cross(u0, f2),
		glm::cross(u1, f0), glm::cross(u1, f1), glm::cross(u1, f2),
		glm::cross(u2, f0), glm::cross(u2, f1), glm::cross(u2, f2)
	};
	for (int i = 0; i < 13; ++i)
	{
		if(!overlapOnAxis(this, triangle, test[i]))
		{
			return false;
		}
	}
	return true;

}

bool BoxCollider::collides(MeshObject* mesh, glm::vec3&normal, float & depth)
{
	for (int i = 0; i < mesh->indices.size(); i+=3)
	{
		std::vector<glm::vec3> t = {
                mesh->indexed_vertices[mesh->indices[i]],
                mesh->indexed_vertices[mesh->indices[i+1]],
                mesh->indexed_vertices[mesh->indices[i+2]]
            };
        for (int i = 0; i < 3; ++i)
        {
        	t[i] = glm::vec3(mesh->transform.getModelMatrix() * glm::vec4(t[i], 1.0f));
        }
        if(this->collides(t)) {
        	normal = glm::normalize(glm::cross(t[1] - t[0], t[2] - t[0]));
        	return true;
        	
        }


	}
	return false;
}
