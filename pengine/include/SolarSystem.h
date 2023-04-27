//
// Created by Adèle Imparato on 15/03/2023.
//

#ifndef SOLARSYSTEM_H
#define SOLARSYSTEM_H

#include <vector>
#include <iostream>
#include "MeshObject.h"
#include "SceneGraph.h"
#include "SceneObject.h"

#define PI 3.14159265

class SolarSystem {

public:

    SolarSystem();

    void createSolarSystem(SceneGraph *root);

};


#endif //SOLARSYSTEM_H
