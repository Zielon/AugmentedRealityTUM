#ifndef PROJECT_BALL_H
#define PROJECT_BALL_H

#include <vector>
#include <LinearMath/btDefaultMotionState.h>
#include "drawing.h"
#include "scene_object.h"

class Ball : SceneObject {
public:
    Ball(btScalar mass, btMotionState *motionState, btCollisionShape *collisionShape, const btVector3 &localInertia);

    void draw() override;

    void setPosition(double x, double y) override;

    static SceneObject *getDefault(btVector3 origin, btVector3 shape, int size, bool gravity);

private:
    btTransform transform;
    btScalar matrix[16];
    btMotionState *motionState;
    int ballSize;
};

#endif //PROJECT_BALL_H
