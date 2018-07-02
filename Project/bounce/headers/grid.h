#ifndef PROJECT_GRID_H
#define PROJECT_GRID_H

#include <bullet/BulletCollision/CollisionShapes/btCollisionShape.h>
#include <bullet/LinearMath/btVector3.h>
#include <bullet/LinearMath/btMotionState.h>

#include "scene_object.h"
#include "drawing.h"

class Grid : SceneObject {
public:
    Grid(btScalar mass, btMotionState *motionState, btCollisionShape *collisionShape, const btVector3 &localInertia);

    void draw() override;

    void setPosition(double x, double y) override;

    void update();

    Type getType() override;

    void setRotation(float euler, float x, float y, float z);

    void setRotationV(std::vector<cv::Vec3d> rot);

    void setOriginV(btVector3 vec);

    static SceneObject *getDefault(btVector3 origin);

private:
    static float gridSize;
    static float gridThickness;
    btVector3 origin;
    float gridYaw = .0f;
    float gridPitch = .0f;
    float gridRoll = .0f;
};

#endif //PROJECT_GRID_H
