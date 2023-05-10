#include "../common/geometry_utils.h" 
#include "include/BoxCollider.h"
#include "include/Plane.h"
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

bool BoxCollider::collides(BoxCollider* other, glm::vec3 &intersection)
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
            if(this->collides(t, normal, depth)) {
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
					if(this->collides(t, normal, depth)) return true;
				}
			}
			if(iterator->children != 0)
			{
				for (int i = 8-1; i >= 0; --i)
				{
					glm::vec3 intersection; //unused;
					if(this->collides(&(iterator->children[i].bounds), intersection))
						toProcess.push_front(&iterator->children[i]);
				}
			}
		}
	}
	return false;
}

bool BoxCollider::collides(std::vector<glm::vec3> triangle, glm::vec3 &normal, float & depth)
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
	depth = FLT_MAX;
	normal = glm::vec3(0.0f);
	for (int i = 0; i < test.size(); ++i)
	{
		float axisDepth;
		glm::vec3 currentNormal;
		if(!overlapOnAxis(this, triangle, test[i], axisDepth, currentNormal)) return false;
		else
		{
			if((axisDepth < depth) && axisDepth > 0)
			{
				depth = axisDepth;
				normal = currentNormal;
			}
		}
	}

	depth /= glm::length(normal);
	normal = glm::normalize(normal);
	glm::vec3 centerTriangle = glm::vec3((triangle[0].x + triangle[0].y + triangle[0].z)/3.0f,
		(triangle[1].x + triangle[1].y + triangle[1].z)/3.0f,
		(triangle[2].x + triangle[2].y + triangle[2].z)/3.0f
		);
	if(glm::dot(this->m_center, centerTriangle) < 0) normal = -normal;
	printf("normal : %f, %f, %f\n", normal.x, normal.y, normal.z);
	return true;

}
