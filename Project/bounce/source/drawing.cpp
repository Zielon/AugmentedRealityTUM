#include <cmath>
#include <opencv2/core/types.hpp>

#include "../headers/drawing.h"

/* PI */
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

using namespace cv;
using namespace std;

void Drawer::drawSphere(double r, int lats, int longs) {
    int i, j;
    for (i = 0; i <= lats; i++) {
        double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
        double z0 = r * sin(lat0);
        double zr0 = r * cos(lat0);

        double lat1 = M_PI * (-0.5 + (double) i / lats);
        double z1 = r * sin(lat1);
        double zr1 = r * cos(lat1);

        glBegin(GL_QUAD_STRIP);
        glColor3f(1.0f, 1.0f, 1.0f);
        for (j = 0; j <= longs; j++) {
            double lng = 2 * M_PI * (double) (j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);

            glNormal3f(x * zr0, y * zr0, z0);
            glVertex3f(x * zr0, y * zr0, z0);
            glNormal3f(x * zr1, y * zr1, z1);
            glVertex3f(x * zr1, y * zr1, z1);
        }
        glEnd();
    }
}

void Drawer::drawCone(GLdouble base, GLdouble height, GLint slices, GLint stacks) {
    // draw the upper part of the cone
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, height);
    for (int angle = 0; angle < 360; angle++) {
        glVertex3f(sin((double) angle) * base, cos((double) angle) * base, 0.f);
    }
    glEnd();

    // draw the base of the cone
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, 0);
    for (int angle = 0; angle < 360; angle++) {
        // normal is just pointing down
        glNormal3f(0, -1, 0);
        glVertex3f(sin((double) angle) * base, cos((double) angle) * base, 0.f);
    }
    glEnd();
    /*IF GLU IS INCLUDED
    GLUquadricObj* quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);

    gluCylinder(quadric, base, (GLdouble)0.0, height, slices, stacks);
    const GLdouble innerradius = 0.0;
    gluDisk(quadric, innerradius, base, slices, stacks);

    gluDeleteQuadric(quadric);
    */
}

void Drawer::drawSnowman() {

    glScalef(0.25, 0.25, 0.25);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawSphere(0.5, 40, 40);

    glTranslatef(0.0f, 0.60f, 0.0f);
    drawSphere(0.40, 40, 40);

    glTranslatef(0.0f, 0.50f, 0.0f);
    drawSphere(0.30, 40, 40);

    glPushMatrix();

    glColor3f(1.0f, 0.0f, 0.0f);

    glTranslatef(0.2f, 0.1f, 0.15f);
    drawSphere(0.05, 40, 40);

    glPopMatrix();
    glPushMatrix();

    glTranslatef(-0.2f, 0.1f, 0.15f);
    drawSphere(0.05, 40, 40);

    glPopMatrix();

    glTranslatef(0.0f, 0.0f, 0.1f);
    drawCone(0.05, 0.5, 10, 10);
}

void Drawer::drawGrid(int size, float thickness, vector<Point3f> points) {

    glScalef(size, thickness, size);

    glBegin(GL_POLYGON);

    glColor3f(0.0, 0.0, 1.0);

    for (auto point : points)
        glVertex3f(point.x, point.y, point.z);

    glEnd();
}

vector<Point3f> Drawer::getGridPoints() {
    return {
            Point3f(0.5, 0.5, 0.5),
            Point3f(0.5, 0.5, -0.5),
            Point3f(-0.5, 0.5, -0.5),
            Point3f(-0.5, 0.5, 0.5)
    };
}
