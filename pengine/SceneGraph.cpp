//
// Created by Adèle Imparato on 09/03/2023.
//

#include "include/SceneGraph.h"
SceneGraph::SceneGraph() {}
SceneGraph::SceneGraph(SceneObject *data) {
    this->data = data;
}

void SceneGraph::setData(SceneObject *data){
    this->data = data;
}

void SceneGraph::setLevel(int level){
    this->level = level;
}

int SceneGraph::getLevel(){
    return this->level;
}

SceneGraph* SceneGraph::addChild(SceneGraph *child) {
    child->setParent(*this);
    child->setLevel(this->getLevel()+1);
    this->children.push_back(child);
    return child;
}

void SceneGraph::addChildren(std::vector<SceneGraph*> children) {
    for(SceneGraph *child :children){
        this->children.push_back(child);
    }
}

std::vector<SceneGraph*> SceneGraph::getChildren() {
    return this->children;
}

SceneObject* SceneGraph::getData() {
    return this->data;
}

void SceneGraph::setParent(SceneGraph parent) {
    this->parent = &parent;
}

SceneGraph SceneGraph::getParent() {
    return *this->parent;
}