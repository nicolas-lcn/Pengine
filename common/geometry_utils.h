#ifndef GEOMETRY_UTILS_H
#define GEOMETRY_UTILS_H

#include <vector>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

class BoxCollider;


typedef struct Interval{
	float min, max;
}Interval;

Interval getInterval(BoxCollider* collider, const glm::vec3 &axis);
Interval getInterval(std::vector<glm::vec3> &triangle, const glm::vec3 &axis);

bool overlapOnAxis(BoxCollider* collider, std::vector<glm::vec3> &triangle, glm::vec3 axis);
glm::vec3 mean(std::vector<glm::vec3> &vertices);


#endif
