//
// Created by Adèle Imparato on 09/03/2023.
//

#ifndef TP1_CODE_SCENEGRAPH_H
#define TP1_CODE_SCENEGRAPH_H

#include <vector>
#include <iostream>
#include "SceneObject.h"


class SceneGraph {
private:
    SceneObject* data;
    std::vector<SceneGraph*> children;
    SceneGraph* parent;
    int level;
public:
    SceneGraph() {}
    SceneGraph(SceneObject *data) {
        this->data = data;
    }

    void setData(SceneObject *data){
        this->data = data;
    }

    void setLevel(int level){
        this->level = level;
    }

    int getLevel(){
        return this->level;
    }

    SceneGraph* addChild(SceneGraph *child) {
        child->setParent(*this);
        child->setLevel(this->getLevel()+1);
        this->children.push_back(child);
        return child;
    }

    void addChildren(std::vector<SceneGraph*> children) {
        for(SceneGraph *child :children){
            this->children.push_back(child);
        }
    }

    std::vector<SceneGraph*> getChildren() {
        return this->children;
    }

    SceneObject* getData() {
        return this->data;
    }

    void setParent(SceneGraph parent) {
        this->parent = &parent;
    }

    SceneGraph getParent() {
        return *this->parent;
    }
};


#endif //TP1_CODE_SCENEGRAPH_H
