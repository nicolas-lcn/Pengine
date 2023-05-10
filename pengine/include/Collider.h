#ifndef COLLIDER_H
#define COLLIDER_H

// Include glm utils
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>


class BoxCollider;
class Sphere;
class Plane;

class Collider
{
public:
	virtual bool collides(BoxCollider* other, glm::vec3 &intersection, glm::vec3 &normal, float & depth) = 0;
	virtual bool collides(Plane* plane, glm::vec3 &normal, float  & depth) = 0;
	virtual bool collides(std::vector<glm::vec3> &triangle) = 0;

};

#endif