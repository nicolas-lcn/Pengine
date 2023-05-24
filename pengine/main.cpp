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
#include <glm/gtx/vector_angle.hpp>

using namespace glm;

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include "common/controls.hpp"
#include <common/quaternion_utils.hpp>

// in order to use stb_image.h and import a png texture image
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#include "include/GLTexture.h"
#include "include/Texture.h"
#include "include/SceneObject.h"
#include "include/MeshObject.h"
#include "include/Sphere.h"
#include "include/ShaderController.h"
#include "include/MenusRenderer.hpp"


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

// penguin object
MeshObject* penguin = new MeshObject();
double initial_speed = 2.2;
bool isSliding = false;
glm::vec3 slideForce;

MeshObject* obstacle = new MeshObject();
MeshObject* slope = new MeshObject();
MeshObject* mountain = new MeshObject();
MeshObject* finishLine = new MeshObject();
MeshObject* background = new MeshObject();
MeshObject* barrier_right = new MeshObject();
MeshObject* barrier_left = new MeshObject();

glm::vec3 initial_penguin_position = glm::vec3(3.2, 2.4, -1.8);

// height map and textures
Texture *height_map = new Texture();
GLTexture *snow_texture = new GLTexture();
GLTexture *mountain_texture = new GLTexture();
GLTexture *landscape_texture = new GLTexture();

GLuint programID;
ShaderController* shaderController = new ShaderController();

MenusRenderer* menusRenderer = new MenusRenderer();
bool inMenu = true;
bool inPause = false;
bool inEndGame = false;
bool isClosing = false;
int obstacle_encountered = 0;
float race_time = 0.0f;
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

    menusRenderer->initMenu(0);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders( "./shaders/vertex_shader.glsl", "./shaders/fragment_shader.glsl" );

    // ------------------------------------------------------------------------------------
    // GENERATE TERRAIN
    // ------------------------------------------------------------------------------------
    slope->generateBuffers();
    slope->create("./data_off/slope.obj");
    slope->transform.setLocalPosition(glm::vec3(0.0, 0.0, 0.0));
    //slope->transform.setLocalRotation(glm::vec3(45.0, 90.0, 90.0));
    slope->transform.setLocalScale(glm::vec3(3, 3, 3));
    slope->setColor(glm::vec4(0.0, 1.0, 0.0, 1.0));
    slope->setIsTerrain(1);

    mountain->generateBuffers();

    mountain->create("./data_off/m_noslope.obj");
    mountain->transform.setLocalPosition(glm::vec3(0.0, 0.0, 0.0));
    //mountain->transform.setLocalScale(glm::vec3(3, 3, 3));
    mountain->setColor(glm::vec4(1.0, 0.0, 0.0, 1.0));
    mountain->setIsTerrain(2);
    // ------------------------------------------------------------------------------------

    // -----------------------------------------------------------------------------------
    // OBJECT PENGUIN
    // -----------------------------------------------------------------------------------
    //penguin->setRigidBody(new RigidBody());
    penguin->generateBuffers();
    penguin->create("./data_off/penguin-2500-triangle.obj");
    penguin->setRigidBody(new RigidBody(50.0f));
    // -----------------------------------------------------------------------------------

    // ------------------------------------------------------------------------------------
    // ADD OBSTACLES
    // -----------------------------------------------------------------------------------
    obstacle->generateBuffers();
    obstacle->create("./data_off/sphere.off");
    // -----------------------------------------------------------------------------------


    // ------------------------------------------------------------------------------------
    // ADD FINISH LINE
    // -----------------------------------------------------------------------------------
    finishLine->generateBuffers();
    finishLine->create("./data_off/finishline.obj");
    barrier_left->generateBuffers();
    barrier_left->create("./data_off/barrier_left.obj");
    barrier_right->generateBuffers();
    barrier_right->create("./data_off/barrier_right.obj");
    // ------------------------------------------------------------------------------------

    // ------------------------------------------------------------------------------------
    // ADD BACKGROUND
    // -----------------------------------------------------------------------------------
    background->generateBuffers();
    background->create("./data_off/plane.obj");
    background->setIsBackground(1);
    background->setColor(glm::vec4(0.4, 0.8, 0.95, 0.0));
    background->transform.setLocalPosition(glm::vec3(-6.0, 8.0, -62.0));
    background->transform.setLocalScale(glm::vec3(200.0, 200.0, 0.0));
    background->transform.setLocalRotation(glm::vec3(-20.0, 270.0, 0.0));
    background->updateSelfAndChild();
    // -----------------------------------------------------------------------------------

    // ------------------------------------------------------------------------------------
    // SCENE GRAPH
    // ------------------------------------------------------------------------------------
    slope->addChild(penguin);
    slope->addChild(obstacle);
    slope->addChild(finishLine);
    finishLine->addChild(barrier_right);
    finishLine->addChild(barrier_left);

    // Place Penguin --------------------------------------------------------------------
    penguin->transform.setLocalPosition(initial_penguin_position);
    penguin->transform.setLocalScale(glm::vec3(3.0, 3.0, 3.0));
    penguin->transform.setLocalRotation(glm::vec3(0.0, 0.0, 0.0));

    // Place Finish Line ----------------------------------------------------------------
    finishLine->transform.setLocalPosition(glm::vec3(-2.3, -1.7, 4.9));

    obstacle->transform.setLocalPosition(glm::vec3(0.0, 0.0,0.0));
    obstacle->transform.setLocalScale(glm::vec3(0.1, 0.1, 0.1));
    slope->forceUpdateSelfAndChild();
    // ------------------------------------------------------------------------------------


    // ------------------------------------------------------------------------------------
    // add textures
    // ------------------------------------------------------------------------------------
    snow_texture->generateTexture();
    snow_texture->loadTexture((char*)"textures/snow.png");
    snow_texture->defineParameters();

    mountain_texture->generateTexture();
    mountain_texture->loadTexture((char*)"textures/snowrocks.png");
    mountain_texture->defineParameters();

    landscape_texture->generateTexture();
    landscape_texture->loadTexture((char*)"textures/paysage.jpg");
    landscape_texture->defineParameters();
    // ------------------------------------------------------------------------------------

    // --- Spring Camera

    initCameraObject(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0), 70.0f, 1.2f, 1.0f);

    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);



    // For speed computation
    float lastTime = glfwGetTime();
    race_time = lastTime;
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

        // display Menu
        if(inMenu || inPause || inEndGame)
        {
            menusRenderer->render();
            glfwSwapBuffers(window);
            glfwPollEvents();
            continue;
        }
        // Use our shader
        glUseProgram(programID);
        computeMatricesFromInputs();

        if(!slope->getGlobalCollider().isInside(penguin->getPosition()))
        {
            penguin->transform.setLocalPosition(initial_penguin_position);
            glm::vec3 novelocity(0.0f);
            penguin->getRigidBody()->setVelocity(novelocity);
        }

        glm::vec3 intersection;
        glm::vec3 normal;
        float depth;
        if(penguin->getGlobalCollider().collides(obstacle, normal, depth))
        {
            penguin->setColor(glm::vec4(1.0, 0.0, 0.0, 1.0));
            glm::vec3 out = depth * normal;
            penguin->transform.setLocalPosition(penguin->transform.getLocalPosition() + out);
            glm::vec3 reboundVec = penguin->getRigidBody()->computeRebound(normal);
            reboundVec *= 0.08;
            //glm::vec3 velocity = penguin->getRigidBody()->getVelocity() + reboundVec;
            penguin->getRigidBody()->setVelocity(reboundVec);
        }
        else{
            penguin->setColor(glm::vec4(0.0, 0.0, 0.0, 1.0));
        }

        glm::vec3 barrier_normal;
        if(penguin->getGlobalCollider().collides(barrier_right, barrier_normal, depth))
        {
            glm::vec3 velocity = penguin->getRigidBody()->getVelocity();
            impulseResponse = penguin->getRigidBody()->computeImpulseResponse(barrier_normal, 0.6f, 12400.0f, glm::vec3(0.0f));
            glm::vec3 adjustedVelocity = velocity + 0.9f * impulseResponse;
            penguin->getRigidBody()->setVelocity(adjustedVelocity);

        }
        if(penguin->getGlobalCollider().collides(barrier_left, barrier_normal, depth))
        {
            glm::vec3 velocity = penguin->getRigidBody()->getVelocity();
            impulseResponse = penguin->getRigidBody()->computeImpulseResponse(barrier_normal, 0.6f, 12400.0f, glm::vec3(0.0f));
            glm::vec3 adjustedVelocity = velocity + 0.9f * impulseResponse;
            penguin->getRigidBody()->setVelocity(adjustedVelocity);

        }

        glm::vec3 planeNormal;
        float planeDepth;
        if(penguin->getGlobalCollider().collides(slope, planeNormal, planeDepth))
        {
            glm::vec3 velocity = penguin->getRigidBody()->getVelocity();
            impulseResponse = penguin->getRigidBody()->computeImpulseResponse(planeNormal, 0.0f, 12400.0f, glm::vec3(0.0f), 0.6f, 1.0f);
            glm::vec3 adjustedVelocity = velocity + impulseResponse;
            penguin->getRigidBody()->setVelocity(adjustedVelocity);
            float costheta = glm::dot(planeNormal, glm::vec3(0.0, -1.0, 0.0));
            glm::vec3 normalforce = 9.81f * costheta * glm::vec3(0.0, -1.0, 0.0);
            glm::vec3 tangent = glm::cross(glm::cross(planeNormal,(velocity)), planeNormal);

            penguin->getRigidBody()->applyForce(normalforce);

            if(isSliding)
            {
                penguin->getRigidBody()->applyForce(slideForce);
                isSliding = false;
            }

            // penguin->setColor(glm::vec4(1.0, 0.0, 0.0, 1.0));
            // printf("Collides");

        }else{
            //penguin->setColor(glm::vec4(0.0, 0.0, 0.0, 1.0));
            penguin->getRigidBody()->applyForce(9.81f * glm::vec3(0.0f, -1.0f, 0.0f));
        }

        glm::vec3 mountainNormal;float mountainDepth;
        if(penguin->getGlobalCollider().collides(mountain, mountainNormal, mountainDepth))
        {
            glm::vec3 velocity = penguin->getRigidBody()->getVelocity();
            impulseResponse = penguin->getRigidBody()->computeImpulseResponse(mountainNormal, 0.6f, 12400.0f, glm::vec3(0.0f));
            glm::vec3 adjustedVelocity = velocity + impulseResponse;
            penguin->getRigidBody()->setVelocity(adjustedVelocity);
            if(isSliding)
            {
                penguin->getRigidBody()->applyForce(slideForce);
                isSliding = false;
            }

        }
        BoxCollider finishCollider = finishLine->getGlobalCollider();
        if(penguin->getGlobalCollider().collides(&finishCollider, intersection, normal, depth))
        {
            // Finish game
            deleteBuffersNode(slope);
            deleteBuffersNode(mountain);
            menusRenderer->initMenu(2);
            menusRenderer->setStats(race_time, obstacle_encountered);
            inEndGame = true;
            glfwSwapBuffers(window);
            glfwPollEvents();
            continue;

        }
        //penguin->getRigidBody()->applyForce(glm::vec3(-15.0, 0.0, 0.0));
        float rot = glm::orientedAngle(glm::normalize(penguin->getRigidBody()->getVelocity()), -penguin->transform.getForward(), penguin->transform.getUp());
        penguin->transform.setLocalRotation(penguin->transform.getLocalRotation() + glm::vec3(0.0, glm::degrees(rot) * deltaTime, 0.0));

        // Update Scene
        //printf("%f, %f, %f\n", penguin->getPosition().x, penguin->getPosition().y, penguin->getPosition().z);
        penguin->update(deltaTime);
        getCamera()->updateTarget(penguin->getPosition(), penguin->transform.getForward() , penguin->transform.getUp());
        //getCamera()->updateTarget(penguin->getPosition(), glm::vec3(-1.0,0.0, 0.0) , glm::vec3(0.0, 1.0, 0.0));
        updateCamera(deltaTime);
        slope->updateSelfAndChild();

        // race_time update
        race_time+=deltaTime;
        // Draw Scene Graph
        render(slope);
        render(mountain);
        render(background);
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( !isClosing &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    deleteBuffersNode(slope);
    deleteBuffersNode(mountain);
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

    // DISPLACE SPHERE USING arrow keys
    if ( key == GLFW_KEY_UP ){
        //getCamera()->updateTarget(sphere->m_center, glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0));
        slideForce = 15.0f * penguin->transform.getForward();
        isSliding = true;

    }else if ( key == GLFW_KEY_DOWN ){
        //getCamera()->updateTarget(sphere->m_center, glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0));
        slideForce = -15.0f * penguin->transform.getForward();
        isSliding = true;

    }else if ( key == GLFW_KEY_LEFT ){
        //getCamera()->updateTarget(sphere->m_center, glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0));

        //sphere->forward[0] -= offset;
        // sphere->up[0] -= offset;
        slideForce = 15.0f * penguin->transform.getRight();
        isSliding = true;

    }else if ( key == GLFW_KEY_RIGHT ){
        //getCamera()->updateTarget(sphere->m_center, glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0));
        slideForce = -15.0f * penguin->transform.getRight();
        isSliding = true;

    }else if ( key == GLFW_KEY_SPACE and action == GLFW_PRESS and inPause){
        inPause = false;
        menusRenderer->cleanMenu();
    }
    else if ( key == GLFW_KEY_ESCAPE and action == GLFW_PRESS and (inPause || inMenu || inEndGame)){
        isClosing = true;
    }
    else if ( key == GLFW_KEY_ESCAPE and action == GLFW_PRESS and !(inPause || inMenu || inEndGame)){
        menusRenderer->initMenu(1);
        inPause = true;
    }


    if(key == GLFW_KEY_ENTER and action == GLFW_PRESS and inMenu)
    {
        menusRenderer->cleanMenu();
        inMenu = false;
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
    shaderController->sendTextures(programID, snow_texture, mountain_texture, landscape_texture);

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