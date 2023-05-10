#include "include/Entity.h"

glm::mat4 Transform::getLocalModelMatrix()
{
    const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f),
                glm::radians(m_eulerRot.x),
                glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f),
                glm::radians(m_eulerRot.y),
                glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f),
                glm::radians(m_eulerRot.z),
                glm::vec3(0.0f, 0.0f, 1.0f));

    const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

    return glm::translate(glm::mat4(1.0f), m_pos) *
                rotationMatrix *
                glm::scale(glm::mat4(1.0f), m_scale);
}


void Transform::computeModelMatrix()
{
    m_worldMatrix = getLocalModelMatrix();
    m_hasMoved = false;
}

void Transform::computeModelMatrix(const glm::mat4& parentGlobalModelMatrix)
{
    m_worldMatrix = parentGlobalModelMatrix * getLocalModelMatrix();
    m_hasMoved = false;
}

void Transform::setLocalPosition(const glm::vec3& newPosition)
{
    m_pos = newPosition;
    m_hasMoved = true;
}

const glm::vec3& Transform::getLocalPosition()
{
    return m_pos;
}

const glm::mat4& Transform::getWorldMatrix()
{
    return m_worldMatrix;
}

bool Transform::hasMoved()
{
    return m_hasMoved;
}

void Transform::setScale(glm::vec3 xyz)
{
    m_scale = xyz;
    m_hasMoved = true;
}

void Transform::setEulerRot(glm::vec3 xyz)
{
    m_eulerRot = xyz;
    m_hasMoved = true;
}

glm::vec3 Transform::getEulerRot(){return m_eulerRot;}

Entity::Entity(){}
Entity::~Entity(){}


void Entity::addChild(Entity* child)
{
    children.emplace_back(child);
    children.back()->parent = this;
}

//Update transform if it was changed
void Entity::updateSelfAndChild()
{
    if (transform.hasMoved())
    {
        forceUpdateSelfAndChild();
        return;
    }

    for (auto&& child : children)
    {
        child->updateSelfAndChild();
    }
}

//Force update of transform even if local space don't change
void Entity::forceUpdateSelfAndChild()
{
    if (parent)
        transform.computeModelMatrix(parent->transform.getWorldMatrix());
    else
        transform.computeModelMatrix();

    for (auto&& child : children)
    {
        child->forceUpdateSelfAndChild();
    }
}

glm::vec3 Entity::getPosition()
{
    glm::mat4 worldMatrix = transform.getWorldMatrix();
    glm::vec4 worldPos = glm::vec4(transform.getLocalPosition(), 1) * worldMatrix;
    return glm::vec3(worldPos.x, worldPos.y, worldPos.z);
}

void Entity::setRigidBody(RigidBody* _rb){this->rb = _rb;}
void Entity::setBoxCollider(BoxCollider* _collider){this->collider = _collider;}

RigidBody* Entity::getRigidBody(){return this->rb;}
BoxCollider* Entity::getBoxCollider(){
	glm::mat4 worldMatrix = transform.getWorldMatrix();
	glm::vec4 world_bbmin = glm::vec4(this->collider->getA(),1.0f) * worldMatrix;
	glm::vec4 world_bbmax = glm::vec4(this->collider->getB(),1.0f) * worldMatrix;
	return new BoxCollider( glm::vec3(world_bbmin.x, world_bbmin.y, world_bbmin.z),
							glm::vec3(world_bbmax.x, world_bbmax.y, world_bbmax.z));
}

void Entity::initBoxCollider()
{
    this->collider = new BoxCollider(this->indexed_vertices);
}

void Entity::resetBoxCollider()
{
    this->collider->reset(this->indexed_vertices);
}

void Entity::update(float deltaTime)
{
	rb->computeForces(deltaTime);
    transform.setLocalPosition(transform.getLocalPosition() + deltaTime * this->rb->getVelocity());
}
