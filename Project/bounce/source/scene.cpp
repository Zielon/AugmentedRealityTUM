#include <bullet/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <bullet/BulletCollision/BroadphaseCollision/btAxisSweep3.h>
#include <bullet/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include "../headers/scene_object.h"
#include "../headers/scene.h"
#include "../headers/ball.h"
#include "../headers/grid.h"

void Scene::addObject(SceneObject *element) {
    if (element != nullptr) {
        dynamicsWorld->addRigidBody((btRigidBody*) element);
        objects.push_back(element);
    }
}

void Scene::simulateObjects() {
    float dtime = time;
    time = (float) glfwGetTime();
    dtime = time - dtime;
    dynamicsWorld->stepSimulation(dtime, 10);
}

Scene::Scene() {
    btDefaultCollisionConfiguration *collisionCfg = new btDefaultCollisionConfiguration();
    btAxisSweep3 *axisSweep = new btAxisSweep3(btVector3(-100, -100, -100), btVector3(100, 100, 100), 128);

    dynamicsWorld = new btDiscreteDynamicsWorld(new btCollisionDispatcher(collisionCfg), axisSweep,
                                                new btSequentialImpulseConstraintSolver, collisionCfg);

    dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

void Scene::defaultSetting() {
    addObject(Ball::getDefault(btVector3(-10, 30, 15), btVector3(5, 5, 5), 10, true));
    addObject(Ball::getDefault(btVector3(0, -20, 0), btVector3(20, 20, 20), 40, false));
}

void Scene::drawObjects() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto object : objects) object->draw();
}
