#include "geometry_utils.h"
#include "../pengine/include/BoxCollider.h"
#include "../pengine/include/Sphere.h"
#include <cmath>

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
		result.min = fminf(result.min, projection);
		result.max = fmaxf(result.max, projection);

	}
	return result;
}
Interval getInterval(std::vector<glm::vec3> &triangle, const glm::vec3 &axis)
{
	Interval result;
	result.min = result.max = glm::dot(axis, triangle[0]);
	for (int i = 1; i < 3; ++i)
	{
		float projection = glm::dot(axis, triangle[i]);
		result.min = fminf(result.min, projection);
		result.max = fmaxf(result.max, projection);
	}
	return result;

}

bool overlapOnAxis(BoxCollider* collider, std::vector<glm::vec3> &triangle, glm::vec3 axis)
{
	Interval a = getInterval(collider, axis);
	Interval b = getInterval(triangle, axis);
	return ((b.min<=a.max) && (a.min<=b.max));
}

glm::vec3 mean(std::vector<glm::vec3> &vertices)
{
	float sumX, sumY, sumZ;
	sumX = sumY = sumZ = 0.0f;
	for (int i = 0; i < vertices.size(); ++i)
	{
		glm::vec3 v = vertices[i];
		sumX += v.x;
		sumY += v.y;
		sumZ += v.z;
	}
	return glm::vec3(sumX/(float)vertices.size(), sumY/(float)vertices.size(), sumZ/(float)vertices.size());
}
