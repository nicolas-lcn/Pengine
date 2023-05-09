#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include "Collider.h"



class BoxCollider : public Collider
{
private:

	glm::vec3 m_center;
	glm::vec3 m_a, m_b;
	glm::vec3 m_size;

public:

	BoxCollider();
	BoxCollider(std::vector<glm::vec3> vertices);
	BoxCollider(glm::vec3 bbmin, glm::vec3 bbmax);

	~BoxCollider();

	void applyMatrix(glm::mat4 model);

	glm::vec3 getA();
	glm::vec3 getB();
	glm::vec3 getCenter();
	glm::vec3 getSize();

	void setA(glm::vec3 bbmin);
	void setB(glm::vec3 bbmax);

	bool collides(BoxCollider* other, glm::vec3 &intersection) override;
	bool collides(Plane* plane, glm::vec3 &normal, float & depth) override;
	bool collides(std::vector<glm::vec3> triangle, glm::vec3 &normal, float & depth) override;


	void reset(std::vector<glm::vec3> vertices);

};


#endif