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
    //Local 
    glm::vec3 m_pos = { 0.0f, 0.0f, 0.0f };
    glm::vec3 m_eulerRot = { 0.0f, 0.0f, 0.0f }; //In degrees
    glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };

    //Global space information concatenate in matrix
    glm::mat4 m_worldMatrix = glm::mat4(1.0f);

    // This flag is used to know if the transform has been modified
    bool m_hasMoved = true;

protected:
    glm::mat4 getLocalModelMatrix();
public:

    void computeModelMatrix();

    void computeModelMatrix(const glm::mat4& parentGlobalModelMatrix);

    void setLocalPosition(const glm::vec3& newPosition);

    const glm::vec3& getLocalPosition();

    const glm::mat4& getWorldMatrix();

    bool hasMoved();
    void setScale(glm::vec3 xyz);

    void setEulerRot(glm::vec3 xyz);

    glm::vec3 getEulerRot();
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

    //Update transform if it was changed
    void updateSelfAndChild();

    //Force update of transform even if local space don't change
    void forceUpdateSelfAndChild();

    void update(float deltaTime);

    glm::vec3 getPosition();

    RigidBody* getRigidBody();
    /* Returns a new BoxCollider in world space*/
    BoxCollider* getBoxCollider();
    void setRigidBody(RigidBody* _rb);
    void setBoxCollider(BoxCollider *_collider);
    void initBoxCollider();
    void resetBoxCollider();
};

#endif