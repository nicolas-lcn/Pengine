//
// Created by Adèle Imparato on 15/03/2023.
//

#ifndef TP1_CODE_SOLARSYSTEM_H
#define TP1_CODE_SOLARSYSTEM_H

#include <vector>
#include <iostream>
#include "MeshObject.h"
#include "SceneGraph.h"
#include "SceneObject.h"

#define PI 3.14159265

class SolarSystem {

public:

    SolarSystem() {}

    void createSolarSystem(SceneGraph *root){

        // create tree

        // create sun
        MeshObject *sun = new MeshObject();
        std::string filename_sphere("data_off/sphere.off");
        sun->create(filename_sphere);
        sun->setColor(glm::vec4(1.0,0.5,0.0,0.0));
        sun->generateBuffers();

        // add sun transformations
        sun->transformations.push_back(glm::vec3(0.8f,0.8f,0.8f));
        sun->index_transf.push_back(0);

        // create scene graph
        //root = new SceneGraph(*sun);
        root->setData(sun);
        root->setLevel(0);

        // create earth
        MeshObject *earth = new MeshObject();
        earth->create(filename_sphere);
        earth->setColor(glm::vec4(0.0,0.0,1.0,0.0));
        earth->generateBuffers();

        // add earth transformations
        earth->transformations.push_back(glm::vec3(0.5f,0.5f,0.5f)); // earth two times smaller than sun
        earth->index_transf.push_back(0);

        earth->transformations.push_back(glm::vec3(5, 0, 0));
        earth->index_transf.push_back(1);

        // earth turns around sun
        earth->transformations.push_back(glm::vec3(-5, 0, 0));
        earth->index_transf.push_back(1);
        earth->transformations.push_back(glm::vec3(0.0,1.0,0.0));
        earth->index_transf.push_back(2);
        earth->transformations.push_back(glm::vec3(5, 0, 0));
        earth->index_transf.push_back(1);
        earth->transformations.push_back(glm::vec3(0.0,-1.0,0.0));
        earth->index_transf.push_back(2);

        // earth turns around itself
        earth->transformations.push_back(glm::vec3(sin(23.f*PI/180),cos(23.f*PI/180),0));
        earth->index_transf.push_back(2);

        SceneGraph *node_earth = root->addChild(new SceneGraph(earth));

        // create moon
        MeshObject *moon = new MeshObject();
        moon->create(filename_sphere);
        moon->setColor(glm::vec4(0.5,0.5,0.5,0.0));
        moon->generateBuffers();

        // add moon transformations
        moon->transformations.push_back(glm::vec3(0.45f,0.45f,0.45f));
        moon->index_transf.push_back(0);
        moon->transformations.push_back(glm::vec3(5*cos(23.f*PI/180), (-5)*sin(23.f*PI/180), 0));
        moon->index_transf.push_back(1);

        // moon turns around earth
        moon->transformations.push_back(glm::vec3(-5, 0, 0)); // move it to center of earth
        moon->index_transf.push_back(1);
        moon->transformations.push_back(glm::vec3(sin(23.f*PI/180),cos(23.f*PI/180),0)); // same rotation axis as earth
        moon->index_transf.push_back(2);
        moon->transformations.push_back(glm::vec3(5, 0, 0)); // move it back to its place
        moon->index_transf.push_back(1);

        SceneGraph *node_moon = node_earth->addChild(new SceneGraph(moon));
    }

};


#endif //TP1_CODE_SOLARSYSTEM_H
