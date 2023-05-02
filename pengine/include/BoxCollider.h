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

	BoxCollider(std::vector<glm::vec3> vertices);
	BoxCollider(glm::vec3 bbmin, glm::vec3 bbmax);

	~BoxCollider();

	void applyMatrix(glm::mat4 model);

	glm::vec3 getA();
	glm::vec3 getB();

	void setA(glm::vec3 bbmin);
	void setB(glm::vec3 bbmax);

	bool collides(BoxCollider* other);

};


#endif