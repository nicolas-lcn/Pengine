#include "geometry_utils.h"
#include "../pengine/include/BoxCollider.h"

Interval getInterval(BoxCollider* collider, const glm::vec3 &axis)
{
	glm::vec3 i = collider->getA(); glm::vec3 a = collider->getB();
	std::vector<glm::vec3> vertices = {
		glm::vec3(i.x, a.y, a.z),
		glm::vec3(i.x, a.y, i.z),
		glm::vec3(i.x, i.y, a.z),
		glm::vec3(i.x, i.y, i.z),
		glm::vec3(a.x, a.y, a.z),
		glm::vec3(a.x, a.y, i.z),
		glm::vec3(a.x, i.y, a.z),
		glm::vec3(a.x, i.y, i.z)
	};

	Interval result;
	result.min = result.max = glm::dot(axis, vertices[0]);
	for (int i = 1; i < 8; ++i)
	{
		float projection = glm::dot(axis, vertices[i]);
		result.min = (projection < result.min) ? projection : result.min;
		result.max = (projection > result.max) ? projection : result.max;

	}
	return result;
}
Interval getInterval(std::vector<glm::vec3> triangle, const glm::vec3 &axis)
{
	Interval result;
	result.min = result.max = glm::dot(axis, triangle[0]);
	for (int i = 1; i < 3; ++i)
	{
		float projection = glm::dot(axis, triangle[i]);
		result.min = (projection <= result.min) ? projection : result.min;
		result.max = (projection >= result.max) ? projection : result.max;
	}
	return result;

}

bool overlapOnAxis(BoxCollider* collider, std::vector<glm::vec3> triangle, glm::vec3 axis, float & depth, glm::vec3 &normal)
{
	Interval a = getInterval(collider, axis);
	Interval b = getInterval(triangle, axis);
	if((b.min<=a.max) && (a.min<=b.max))
	{
		depth = std::min(b.max - a.min, a.max - b.min);
		normal = axis;
		return true;
	}
	else return false;
}