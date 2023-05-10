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

    glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

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


void SceneObject::accelerate()
{
    if(this->accelerator != 0) return;
    this->accelerator = new BVHNode();
    this->accelerator->bounds = BoxCollider(this->indexed_vertices);
    this->accelerator->numTriangles = triangles.size();
    this->accelerator->triangles = std::vector<int>(this->accelerator->numTriangles );
    for (int i = 0; i < this->accelerator->numTriangles ; ++i)
    {
        this->accelerator->triangles[i] = i;
    }
    splitBVHNode(this->accelerator, 3);
}

void SceneObject::splitBVHNode(BVHNode* node, int depth)
{
    if(depth-- == 0) return;
    if(node->children == 0)
    {
        if(node->numTriangles > 0)
        {
            node->children = new BVHNode[8];
            glm::vec3 c = node->bounds.getCenter();
            glm::vec3 e = node->bounds.getSize() * 0.5f;

            node->children[0].bounds = BoxCollider(c + glm::vec3(-e.x, +e.y, -e.z), e);
            node->children[1].bounds = BoxCollider(c + glm::vec3(+e.x, +e.y, -e.z), e);
            node->children[2].bounds = BoxCollider(c + glm::vec3(-e.x, +e.y, +e.z), e);
            node->children[3].bounds = BoxCollider(c + glm::vec3(+e.x, +e.y, +e.z), e);
            node->children[4].bounds = BoxCollider(c + glm::vec3(-e.x, -e.y, -e.z), e);
            node->children[5].bounds = BoxCollider(c + glm::vec3(+e.x, -e.y, -e.z), e);
            node->children[6].bounds = BoxCollider(c + glm::vec3(-e.x, -e.y, +e.z), e);
            node->children[7].bounds = BoxCollider(c + glm::vec3(+e.x, -e.y, +e.z), e);
        }
    }
    if(node->children != 0 && node->numTriangles > 0)
    {
        for (int i = 0; i < 8; ++i) //each child
        {
            node->children[i].numTriangles = 0;
            for (int j = 0; j < node->numTriangles; ++j)
            {
                std::vector<glm::vec3> t = {
                    this->indexed_vertices[this->triangles[node->triangles[j]][0]],
                    this->indexed_vertices[this->triangles[node->triangles[j]][1]],
                    this->indexed_vertices[this->triangles[node->triangles[j]][2]]
                };
                glm::vec3 normal; float depth;//unused
                if(node->children[i].bounds.collides(t, normal, depth))
                {
                    node->children[i].numTriangles++;

                }
            }
            if(node->children[i].numTriangles == 0) continue;
            node->children[i].triangles = std::vector<int>(node->children[i].numTriangles);
            int index = 0;
            for (int j = 0; j < node->numTriangles; ++j)
            {
                std::vector<glm::vec3> t = {
                    this->indexed_vertices[this->triangles[node->triangles[j]][0]],
                    this->indexed_vertices[this->triangles[node->triangles[j]][1]],
                    this->indexed_vertices[this->triangles[node->triangles[j]][2]]
                };
                glm::vec3 normal; float depth;//unused
                if(node->children[i].bounds.collides(t, normal, depth))
                {
                    node->children[i].triangles[index++] = node->triangles[j];

                }
            }
            
        }
        node->numTriangles = 0;
        node->triangles.clear();
        for (int i = 0; i < 8; ++i)
        {
            splitBVHNode(&node->children[i], depth);
        }
    }
}

void freeBVHNode(BVHNode *node)
{
    if(node->children != 0)
    {
        for (int i = 0; i < 8; ++i)
        {
            freeBVHNode(&node->children[i]);
        }
        delete[] node->children;
        node->children = 0;
    }
    if(node->numTriangles != 0 || node->triangles.size() > 0)
    {
        node->triangles.clear();
        node->numTriangles = 0;
    }
}