#ifndef PROJECT_OBJECT_H
#define PROJECT_OBJECT_H

#include <bullet/BulletDynamics/Dynamics/btRigidBody.h>
#include <bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <bullet/LinearMath/btScalar.h>
#include <bullet/LinearMath/btVector3.h>
#include <LinearMath/btDefaultMotionState.h>

#include "drawing.h"

enum Type {
    GRID,
    BALL
};

class SceneObject : public btRigidBody {
public:

    SceneObject(btScalar mass, btMotionState *motionState, btCollisionShape *collisionShape,
                const btVector3 &localInertia = btVector3(0, 0, 0))
            : btRigidBody(mass, motionState, collisionShape, localInertia) {
    }

    virtual void draw() = 0;

    virtual Type getType() = 0;

protected:
    Drawer drawer;
    btTransform transform;
    btScalar matrix[16];
    btQuaternion quaternion;
    btDefaultMotionState *motionState;
};

#endif //PROJECT_OBJECT_H
