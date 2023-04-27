//
// Created by Adèle Imparato on 09/03/2023.
//

#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

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
    SceneGraph();
    SceneGraph(SceneObject *data);

    void setData(SceneObject *data);

    void setLevel(int level);

    int getLevel();

    SceneGraph* addChild(SceneGraph *child);

    void addChildren(std::vector<SceneGraph*> children);
    std::vector<SceneGraph*> getChildren();
    SceneObject* getData();

    void setParent(SceneGraph parent);

    SceneGraph getParent();
};


#endif //SCENEGRAPH_H
