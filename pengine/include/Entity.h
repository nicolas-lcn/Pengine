#ifndef ENTITY_H
#define ENTITY_H

#include <list>
#include <memory>
// Include glm utils
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SceneObject.h"
#include "RigidBody.h"

class Transform
{
protected:
    //Local space information
    glm::vec3 m_pos = { 0.0f, 0.0f, 0.0f };
    glm::vec3 m_eulerRot = { 0.0f, 0.0f, 0.0f }; //In degrees
    glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };

    //Global space information concatenate in matrix
    glm::mat4 m_modelMatrix = glm::mat4(1.0f);

    //Dirty flag
    bool m_isDirty = true;

protected:
    glm::mat4 getLocalModelMatrix();
public:

    void computeModelMatrix();

    void computeModelMatrix(const glm::mat4& parentGlobalModelMatrix);

    void setLocalPosition(const glm::vec3& newPosition);

    void setLocalRotation(const glm::vec3& newRotation);

    void setLocalScale(const glm::vec3& newScale);

    glm::vec3 getGlobalPosition() const;

    glm::vec3 getLocalPosition() const;

    glm::vec3 getLocalRotation() const;

    glm::vec3 getLocalScale() const;

    glm::mat4 getModelMatrix() const;

    glm::vec3 getRight() const;


    glm::vec3 getUp() const;

    glm::vec3 getBackward() const;

    glm::vec3 getForward() const;

    glm::vec3 getGlobalScale() const;

    bool isDirty() const;
};

class Entity : public SceneObject
{
protected:
    RigidBody* rb;
    BoxCollider* collider;
public:
    std::vector<Entity*> children;
    Entity* parent = nullptr;

    Transform transform;

    Entity();
    ~Entity();

    
    void addChild(Entity* child);
    void removeChild(int i);

    //Update transform if it was changed
    void updateSelfAndChild();

    //Force update of transform even if local space don't change
    void forceUpdateSelfAndChild();

    void update(float deltaTime);

    glm::vec3 getPosition();

    RigidBody* getRigidBody();
    /* Returns a new BoxCollider in world space*/
    BoxCollider getGlobalCollider();
    BoxCollider* getBoxCollider();
    void setRigidBody(RigidBody* _rb);
    void setBoxCollider(BoxCollider *_collider);
    void initBoxCollider();
    void resetBoxCollider();
};

#endif