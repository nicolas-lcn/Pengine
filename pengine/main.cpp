// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/compatibility.hpp>

using namespace glm;

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include "common/controls.hpp"
#include <common/quaternion_utils.hpp>

// in order to use stb_image.h and import a png texture image
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#include "include/Plane.h"
#include "include/GLTexture.h"
#include "include/Texture.h"
#include "include/SceneObject.h"
#include "include/MeshObject.h"
#include "include/Sphere.h"
#include "include/ShaderController.h"


void key (GLFWwindow *window, int key, int scancode, int action, int mods );
glm::vec3 computeNetForce(glm::vec3 &normal);
glm::vec3 computeImpulse(glm::vec3 & normal, glm::vec3 & velocity);

void render(Entity *node);
void deleteBuffersNode(Entity *node);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
bool cameraRotates = false;
float cameraSpeed;
bool speedUp = false;
bool slowDown = false;
// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;


bool heightmap_activated = true;

// plane data
float plane_len =  10.0;
float plane_larg = 3.0;
int plane_dim = 40;
Plane *plane = new Plane(plane_larg, plane_len, plane_dim, plane_dim);

// sphere data
//Sphere* sphere = new Sphere();
MeshObject* sphere = new MeshObject();
double initial_speed = 2.2;
bool isSliding = false;
glm::vec3 slideForce;

MeshObject* obstacle = new MeshObject();
MeshObject* slope = new MeshObject();

// height map and textures
Texture *height_map = new Texture();
GLTexture *snow_texture = new GLTexture();

GLuint programID;
ShaderController* shaderController = new ShaderController();
/*******************************************************************************/


int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "PENGINE", NULL, NULL);

    // key
    glfwSetKeyCallback(window, key);
    int screen_width, screen_height;
    glfwGetFramebufferSize(window, &screen_width, &screen_height);

    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    //  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Dark blue background
    glClearColor(0.8f, 0.8f, 0.8f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    //glEnable(GL_CULL_FACE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders( "./shaders/vertex_shader.glsl", "./shaders/fragment_shader.glsl" );

    // ------------------------------------------------------------------------------------
    // GENERATE TERRAIN
    // ------------------------------------------------------------------------------------
    // generate plane -> fill arrays of indices, triangles and indexed_vertices
    plane->generatePlane();
    plane->setIsTerrain(1);
    plane->generateBuffers();

    // use height map
    if(heightmap_activated){
        height_map->readPGMTexture((char*)"textures/heightmap_simple1024.pgm");
        plane->addHeightMap(height_map->data, height_map->height, height_map->width);
    }

    plane->addRelief();

    plane->setColor(glm::vec4(0.2, 0.8, 0.05, 0.0));
    plane->transform.setLocalPosition(glm::vec3(0.0, 0.0, 0.0));

    slope->generateBuffers();
    slope->create("./data_off/moutain.obj");
    slope->transform.setLocalPosition(glm::vec3(0.0, 0.0, 0.0));
    //slope->transform.setLocalRotation(glm::vec3(45.0, 90.0, 90.0));
    slope->transform.setLocalScale(glm::vec3(150, 150, 150));
    slope->setColor(glm::vec4(0.0, 1.0, 0.0, 1.0));
    slope->setIsTerrain(1);
    
    // ------------------------------------------------------------------------------------

    // -----------------------------------------------------------------------------------
    // SPHERE OBJECT - PENGUIN
    // -----------------------------------------------------------------------------------
    /*sphere->m_radius =  0.1f;
    //sphere->m_center = glm::vec3(glm::vec3(0.0, 0.0, -0.1));
    //sphere->m_center = center_sphere;
    sphere->build_arrays();
    sphere->build_arrays_for_resolutions();
    sphere->setColor(glm::vec4(0.0,0.0,0.0,0.0));
    sphere->generateBuffers();
    sphere->transform.setLocalPosition(glm::vec3(0.0, 30, 93.0));
    // if(heightmap_activated){
    //     float height = plane->getHeightFromCoords(sphere->transform.getLocalPosition());
    //     sphere->transform.setLocalPosition(glm::vec3(0.0, height,-0.1));

    // }
    sphere->setRigidBody(new RigidBody());*/
    sphere->generateBuffers();
    sphere->create("./data_off/penguin-2500-triangle.obj");
    sphere->setRigidBody(new RigidBody(150.0f));


    // ------------------------------------------------------------------------------------
    // ADD OBSTACLE
    // -----------------------------------------------------------------------------------
    obstacle->generateBuffers();
    obstacle->create("./data_off/cube.off");
    

    //penguin->generateBuffers();
    //penguin->create("./data_off/penguin-simpl-triangle.obj");


    // Plane *plane2 = new Plane(plane_larg, plane_len, plane_dim, plane_dim);
    // plane2->center = glm::vec3(0.0,0.0,-plane_len);
    // plane2->generatePlane();
    // plane2->setIsTerrain(1);
    // plane2->generateBuffers();
    // // use height map
    // if(heightmap_activated){
    //     height_map->readPGMTexture((char*)"textures/heightmap_jeu1024.pgm");
    //     plane2->addHeightMap(height_map->data, height_map->height, height_map->width);
    // }
    // ------------------------------------------------------------------------------------
    // SCENE GRAPH
    // ------------------------------------------------------------------------------------
    
    // plane->addChild(sphere);
    // plane->addChild(obstacle);
    // plane->addChild(slope);
    slope->addChild(sphere);
    //slope->addChild(obstacle);
    sphere->transform.setLocalPosition(glm::vec3(0.15, 1.5, 0.4));
    // sphere->transform.setLocalScale(glm::vec3(0.3, 0.3, 0.3));
    obstacle->transform.setLocalPosition(glm::vec3(0.0, 0.6,-2.0));
    if(heightmap_activated){
        float height = plane->getHeightFromCoords(obstacle->transform.getLocalPosition());
        obstacle->transform.setLocalPosition(glm::vec3(0.0, height + 0.05,-2.0));
    }
    obstacle->transform.setLocalScale(glm::vec3(0.1, 0.1, 0.1));
    slope->forceUpdateSelfAndChild();
    plane->forceUpdateSelfAndChild();
    

    // ------------------------------------------------------------------------------------


    // ------------------------------------------------------------------------------------
    // add textures
    // ------------------------------------------------------------------------------------
    snow_texture->generateTexture();
    snow_texture->loadTexture((char*)"textures/snow.png");
    snow_texture->defineParameters();

    // ------------------------------------------------------------------------------------

    // --- Spring Camera 
    initCameraObject(sphere->getPosition(), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0), 70.0f, 10.0f, 10.0f);

    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);


    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;
    double counter_flying = 0.0;
        
    glm::vec3 impulseResponse;


    do{
        // Measure speed
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);
        computeMatricesFromInputs();

        if(isSliding)
        {
            sphere->getRigidBody()->applyForce(slideForce);
            isSliding = false;
        }
        
        glm::vec3 intersection;
        glm::vec3 normal;
        float depth;
        BoxCollider obstacleCollider = obstacle->getGlobalCollider();
        BoxCollider *obstacle_ptr = &obstacleCollider;
        if(sphere->getGlobalCollider().collides(obstacle_ptr,intersection, normal, depth))
        {
            sphere->setColor(glm::vec4(1.0, 0.0, 0.0, 1.0));
            glm::vec3 out = depth * normal;
            sphere->transform.setLocalPosition(sphere->transform.getLocalPosition() + out);
            glm::vec3 reboundVec = sphere->getRigidBody()->computeRebound(normal);
            reboundVec *= 0.2;
            //glm::vec3 velocity = sphere->getRigidBody()->getVelocity() + reboundVec;
            sphere->getRigidBody()->setVelocity(reboundVec);
        }
        else{
            sphere->setColor(glm::vec4(0.0, 0.0, 0.0, 1.0));
        }

        glm::vec3 planeNormal;
        float planeDepth;
        if(sphere->getGlobalCollider().collides(slope, planeNormal, planeDepth))
        {

            glm::vec3 velocity = sphere->getRigidBody()->getVelocity();
            impulseResponse = sphere->getRigidBody()->computeImpulseResponse(planeNormal, 0.0f, 12400.0f, glm::vec3(0.0f), 0.3f, 0.7f);
            glm::vec3 adjustedVelocity = velocity + impulseResponse;
            sphere->getRigidBody()->setVelocity(adjustedVelocity);
            float costheta = glm::dot(planeNormal, glm::vec3(0.0, 1.0, 0.0));
            glm::vec3 normalforce = 9.81f * costheta * glm::vec3(0.0, 1.0, 0.0);
            sphere->getRigidBody()->applyForce(normalforce);
            //sphere->setColor(glm::vec4(1.0, 0.0, 0.0, 1.0));
            // printf("Collides");


        }
        else
        {
            //sphere->setColor(glm::vec4(0.0, 0.0, 0.0, 1.0));
            sphere->getRigidBody()->applyForce(9.81f * glm::vec3(0.0f, -1.0f, 0.0f));

        }
        

        // Update Scene 
        sphere->update(deltaTime);
        getCamera()->updateTarget(sphere->getPosition(), glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
        updateCamera(deltaTime);
        //plane->updateSelfAndChild();
        slope->updateSelfAndChild();

        // Draw Scene Graph
        render(slope);
        //render(slope);
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    // Entity* entity = plane;
    // while(entity != nullptr)
    // {
    //     entity->deleteBuffers();
    // }
    deleteBuffersNode(plane);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


void key (GLFWwindow *window, int key, int scancode, int action, int mods ) {

    double offset = 0.01;

    if( key == GLFW_KEY_EQUAL and action == GLFW_PRESS ){ // minus on macbook keyboard
        std::cout << "You have pressed the key - : resolution decreases" << std::endl;

        /// DECREASE RESOLUTION

        if(plane_dim > 13){
            plane_dim -= 5;
        }else{
            plane_dim = 8;
        }

    }
    else if( key == GLFW_KEY_SLASH and action == GLFW_PRESS ){ // plus on macbook keyboard
        std::cout << "You have pressed the key + : resolution increases" << std::endl;

        /// INCREASE RESOLUTION

        if(plane_dim < 196){
            plane_dim += 5;
        }else{
            plane_dim = 200;
        }

    }else if( key == GLFW_KEY_W and action == GLFW_PRESS ){ // Z on macbook keyboard
        std::cout << "You have pressed the key Z : rotation speeds up" << std::endl;

        /// accelerates camera
        speedUp = true;

    }else if ( key == GLFW_KEY_Z and action == GLFW_PRESS ) { // W on macbook keyboard
        std::cout << "You have pressed the key W : rotation slows down" << std::endl;

        /// slows down camera
        slowDown = true;

    }

    // DISPLACE SPHERE USING arrow keys
    else if ( key == GLFW_KEY_UP ){

        // if object doesn't go farther than terrain area
        if(sphere->m_center[2] - offset > plane->top_right[2] and sphere->m_center[2] - offset < plane->bottom_right[2]) {
            // sphere->transformations[0][2] -= offset;
            // sphere->m_center[2] -= offset;
            //getCamera()->updateTarget(sphere->m_center, glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0));
            slideForce = glm::vec3(-5.0, 0.0, 0.0);
            isSliding = true;
        }

    }else if ( key == GLFW_KEY_DOWN ){

        // if object doesn't go farther than terrain area
        if(sphere->m_center[2] + offset > plane->top_right[2] and sphere->m_center[2] + offset < plane->bottom_right[2]){
            // sphere->transformations[0][2] += offset;
            // sphere->m_center[2] += offset;
            //getCamera()->updateTarget(sphere->m_center, glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0));
            slideForce = glm::vec3(5.0, 0.0, 0.0);
            isSliding = true;
        }

    }else if ( key == GLFW_KEY_LEFT ){

        // if object doesn't go farther than terrain area
        if(sphere->m_center[0] - offset > plane->top_right[2] and sphere->m_center[0] - offset < plane->bottom_right[2]){
            // sphere->transformations[0][0] -= offset;
            // sphere->m_center[0] -= offset;
            //getCamera()->updateTarget(sphere->m_center, glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0));

            //sphere->forward[0] -= offset;
            // sphere->up[0] -= offset;
            slideForce = glm::vec3(0.0, 0.0, 5.0);
            isSliding = true;
        }

    }else if ( key == GLFW_KEY_RIGHT ){

        // if object doesn't go farther than terrain area
        if(sphere->m_center[0] + offset > plane->top_right[2] and sphere->m_center[0] + offset < plane->bottom_right[2]) {
            // sphere->transformations[0][0] += offset;
            // sphere->m_center[0] += offset;
            //getCamera()->updateTarget(sphere->m_center, glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0));
            slideForce = glm::vec3(0.0, 0.0, -5.0);
            isSliding = true;
        }
    }else if ( key == GLFW_KEY_SPACE and action == GLFW_PRESS ){
        //sphere->isFlying = true;
        std::cout << "fly starts" << std::endl;
        // sphere->velocity = glm::vec3(1.0,1.0,0.0) * glm::vec3(initial_speed,initial_speed,initial_speed);
        glm::vec3 flyForce(0.0, 5.0, 0.0);
        sphere->getRigidBody()->applyForce(flyForce);
    }

    /*if( key == GLFW_KEY_RIGHT or key == GLFW_KEY_LEFT or key == GLFW_KEY_UP or key == GLFW_KEY_DOWN){
        // ----------------------------------------------------------------
        //follow height of terrain according to heightmap
        // sphere->transformations[0][1] -= sphere->m_center[1];
        // double height_sphere = 0.0;
        // if(heightmap_activated){
        //     height_sphere = plane->getHeightFromCoords(height_map->data, height_map->height, height_map->width, sphere->m_center);
        // }
        // double y_offset = 0.0;
        // sphere->transformations[0][1] += height_sphere + sphere->m_radius + y_offset;
        // sphere->m_center[1] = height_sphere + sphere->m_radius + y_offset;
        //----------------------------------------------------------------

        // ----------------------------------------------------------------
        // check if object is too far from camera -> decrease resolution
        glm::vec3 camPos = getCamPosition();
        double distance_from_cam = camPos[2] - sphere->m_center[2];

        unsigned int new_resol;
        int reso;
        if(distance_from_cam > 6.0){ // back
            reso = 0;
            new_resol = sphere->back_resolution;
        }else if(distance_from_cam <= 6.0 and distance_from_cam > 4.0){ // middle
            reso = 1;
            new_resol = sphere->middle_resolution;
        }else{ // front
            reso = 2;
            new_resol = sphere->front_resolution;
        }

        // do it only if new resolution
        if(new_resol != sphere->resolution){
            sphere->setResolution(new_resol);
            sphere->clearVectors();
            sphere->switchResolution(reso);
        }
        // ----------------------------------------------------------------
    }*/

    if( (key == GLFW_KEY_SLASH or key == GLFW_KEY_EQUAL) and action == GLFW_PRESS){

        // EDIT PLANE
        plane->setDimension(plane_dim, plane_dim);
        plane->clearVectors();
        plane->generatePlane();
        plane->addHeightMap(height_map->data, height_map->height, height_map->width);
    }

}

//Calculate force to apply to make the object slide on the slope
glm::vec3 computeNetForce(glm::vec3 &normal)
{
    float theta = glm::acos(glm::dot(glm::vec3(0.0, 1.0, 0.0), normal));
    return glm::vec3(0.0, 0.0, 9.81f * (sin(theta) - cos(theta)));
}

void render(Entity* node)
{

    shaderController->sendMatrices(programID, node->transform.getModelMatrix(), getProjectionMatrix(), getViewMatrix());
    shaderController->sendTextures(programID, snow_texture);

    node->loadBuffers();
    node->draw(programID);
    for (int i = 0; i < node->children.size(); ++i)
    {
        render(node->children[i]);
    }
}

void deleteBuffersNode(Entity *node)
{
    node->deleteBuffers();
    for (int i = 0; i < node->children.size(); ++i)
    {
        deleteBuffersNode(node->children[i]);
    }
}