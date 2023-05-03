//
// Created by Adèle Imparato on 09/03/2023.
//

#include "include/SceneObject.h"

SceneObject::SceneObject() {}

void SceneObject::setIsTerrain(int isTerrain){
    this->isTerrain = isTerrain;
}

void SceneObject::setColor(glm::vec4 color){
    this->color = color;
}


void SceneObject::draw(GLuint programID) const {
    if( triangles.size() == 0 ) return;

    glUniform1i(glGetUniformLocation(programID, "isTerrain"), isTerrain);
    glUniform4f(glGetUniformLocation(programID, "mesh_color"), color[0], color[1], color[2], color[3]);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
            0,                  // attribute
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
    );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

    if(isTerrain == 0){
        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
    }else{
        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    }

    glEnableClientState(GL_VERTEX_ARRAY) ;
    glEnableClientState (GL_NORMAL_ARRAY);
    glNormalPointer (GL_FLOAT, 3*sizeof (float), (GLvoid*)(normals.data()));
    glVertexPointer (3, GL_FLOAT, 3*sizeof (float) , (GLvoid*)(indexed_vertices.data()));
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(2);
}

void SceneObject::generateBuffers(){
    glGenBuffers(1, &vertexbuffer);
    glGenBuffers(1, &elementbuffer);
    glGenBuffers(1, &buffer_coord_txt);
}

void SceneObject::loadBuffers(){
    // Load data (vertices, meshes, etc.) into VBO's
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

    // Generate a buffer for the indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

    // fill buffer for texture coordinates
    glBindBuffer(GL_ARRAY_BUFFER, buffer_coord_txt);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2, (void *) 0);
    glBufferData(GL_ARRAY_BUFFER, coord_texture.size() * sizeof(glm::vec2), &coord_texture[0], GL_STATIC_DRAW);
}

void SceneObject::deleteBuffers(){
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &elementbuffer);
    glDeleteBuffers(1, &buffer_coord_txt);
}

void SceneObject::clearVectors(){
    indexed_vertices.clear();
    indices.clear();
    triangles.clear();
    normals.clear();
    coord_texture.clear();
}

void SceneObject::setRigidBody(RigidBody* _rb){this->rb = _rb;}
void SceneObject::setBoxCollider(BoxCollider* _collider){this->collider = _collider;}

RigidBody* SceneObject::getRigidBody(){return this->rb;}
BoxCollider* SceneObject::getBoxCollider(){return this->collider;}

void SceneObject::initBoxCollider()
{
    this->collider = new BoxCollider(this->indexed_vertices);
}