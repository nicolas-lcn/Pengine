#include "include/Entity.h"


glm::mat4 Transform::getLocalModelMatrix()
{
    const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), glm::radians(m_eulerRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), glm::radians(m_eulerRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), glm::radians(m_eulerRot.z), glm::vec3(0.0f, 0.0f, 1.0f));

    // Y * X * Z
    const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

    // translation * rotation * scale (also know as TRS matrix)
    return glm::translate(glm::mat4(1.0f), m_pos) * rotationMatrix * glm::scale(glm::mat4(1.0f), m_scale);
}


void Transform::computeModelMatrix()
{
    m_modelMatrix = getLocalModelMatrix();
    m_isDirty = false;
}

void Transform::computeModelMatrix(const glm::mat4& parentGlobalModelMatrix)
{
    m_modelMatrix = parentGlobalModelMatrix * getLocalModelMatrix();
    m_isDirty = false;
}

void Transform::setLocalPosition(const glm::vec3& newPosition)
{
    m_pos = newPosition;
    m_isDirty = true;
}

void Transform::setLocalRotation(const glm::vec3& newRotation)
{
    m_eulerRot = newRotation;
    m_isDirty = true;
}

void Transform::setLocalScale(const glm::vec3& newScale)
{
    m_scale = newScale;
    m_isDirty = true;
}

glm::vec3 Transform::getGlobalPosition() const
{
    glm::vec3 pos(m_modelMatrix[3]);
    return pos;
}

glm::vec3 Transform::getLocalPosition() const
{
    return m_pos;
}

glm::vec3 Transform::getLocalRotation() const
{
    return m_eulerRot;
}

glm::vec3 Transform::getLocalScale() const
{
    return m_scale;
}

glm::mat4 Transform::getModelMatrix() const
{
    return m_modelMatrix;
}

glm::vec3 Transform::getRight() const
{
    return glm::vec3(m_modelMatrix[0]);
}


glm::vec3 Transform::getUp() const
{
    return glm::vec3(m_modelMatrix[1]);
}

glm::vec3 Transform::getBackward() const
{
    return glm::vec3(m_modelMatrix[2]);
}

glm::vec3 Transform::getForward() const
{
    return glm::vec3(-m_modelMatrix[2]);
}

glm::vec3 Transform::getGlobalScale() const
{
    return { glm::length(getRight()), glm::length(getUp()), glm::length(getBackward()) };
}

bool Transform::isDirty() const
{
    return m_isDirty;
}


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
    if (transform.isDirty())
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
        transform.computeModelMatrix(parent->transform.getModelMatrix());
    else
        transform.computeModelMatrix();

    for (auto&& child : children)
    {
        child->forceUpdateSelfAndChild();
    }
}

glm::vec3 Entity::getPosition()
{
    return transform.getGlobalPosition();
}

void Entity::setRigidBody(RigidBody* _rb){this->rb = _rb;}
void Entity::setBoxCollider(BoxCollider* _collider){this->collider = _collider;}

RigidBody* Entity::getRigidBody(){return this->rb;}

BoxCollider* Entity::getBoxCollider(){
	return this->collider;
}

BoxCollider Entity::getGlobalCollider()
{
    glm::vec3 globalA(transform.getModelMatrix() * glm::vec4(this->collider->getA(),1.0f));
    glm::vec3 globalB(transform.getModelMatrix() * glm::vec4(this->collider->getB(),1.0f));
    return BoxCollider(globalA, globalB);

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
