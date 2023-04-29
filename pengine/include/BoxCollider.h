#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

// Include glm utils
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>



class BoxCollider
{
private:

	glm::vec3 center;
	glm::vec3 m_a, m_b;

public:

	BoxCollider(){};
	BoxCollider(glm::vec3 bbmin, glm::vec3 bbmax):m_a(bbmin), m_b(bbmax){}

	~BoxCollider(){}

	glm::vec3 getA(){return m_a;}
	glm::vec3 getB(){return m_b;}

};


#endif