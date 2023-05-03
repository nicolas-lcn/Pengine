#include "include/BoxCollider.h"
#include <vector>
#include <cstdio>


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
}

BoxCollider::BoxCollider(glm::vec3 bbmin, glm::vec3 bbmax):m_a(bbmin), m_b(bbmax){}

BoxCollider::~BoxCollider(){}

void BoxCollider::applyMatrix(glm::mat4 model)
{
	glm::vec4 model_a = model * glm::vec4(m_a,1);
	glm::vec4 model_b = model * glm::vec4(m_b,1);

	m_a = glm::vec3(model_a[0], model_a[1], model_a[2]); 
	m_b = glm::vec3(model_b[0], model_b[1], model_b[2]); 
}

glm::vec3 BoxCollider::getA(){return m_a;}
glm::vec3 BoxCollider::getB(){return m_b;}

void BoxCollider::setA(glm::vec3 bbmin){ m_a = bbmin;}
void BoxCollider::setB(glm::vec3 bbmax){ m_b = bbmax;}

bool BoxCollider::collides(BoxCollider* other)
{
	return  (m_a.x <= other->getB().x && m_b.x >= other->getA().x) &&
			(m_a.y <= other->getB().y && m_b.y >= other->getA().y) &&
			(m_a.z <= other->getB().z && m_b.z >= other->getA().z);
}