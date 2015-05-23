/*****************************************************************************************************
 Copyright (c) 2015 Gorka Suárez García

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 DEALINGS IN THE SOFTWARE.
*****************************************************************************************************/

#include "gentity.h"

//====================================================================================================
// class Camera:
//====================================================================================================

void Camera::moveN(GLfloat value) {
    Vector3 dir = look_ - eye_;
    dir.Normalize();
    dir = dir * value;
    eye_ = eye_ + dir;
    look_ = look_ + dir;
}

void Camera::moveU(GLfloat value) {
    Vector3 dir1 = look_ - eye_;
    Vector3 dir2 = up_;
    dir1.Normalize();
    dir2.Normalize();
    Vector3 dir = dir2.Cross(dir1);
    dir.Normalize();
    dir = dir * value;
    eye_ = eye_ + dir;
    look_ = look_ + dir;
}

void Camera::moveV(GLfloat value) {
    Vector3 dir = up_;
    dir.Normalize();
    dir = dir * value;
    eye_ = eye_ + dir;
    look_ = look_ + dir;
}

void Camera::rotate(GLfloat value) {
    Vector3 view = look_ - eye_;
    value = DegToRad(value);
    Quaternion temp(
        up_.X() * std::sin(value * 0.5f),
        up_.Y() * std::sin(value * 0.5f),
        up_.Z() * std::sin(value * 0.5f),
        std::cos(value * 0.5f)
    );
    Quaternion qview(view.X(), view.Y(), view.Z(), 0.0f);
    Quaternion result = (temp * qview) * temp.Conjugate();
    view.Set(result);
    look_ = view + eye_;
}

void Camera::SetEye(GLfloat x, GLfloat y, GLfloat z) {
    eye_.Set(x, y, z);
}

void Camera::SetLook(GLfloat x, GLfloat y, GLfloat z) {
    look_.Set(x, y, z);
}

void Camera::SetUp(GLfloat x, GLfloat y, GLfloat z) {
    up_.Set(x, y, z);
}

void Camera::Apply() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye_.X(), eye_.Y(), eye_.Z(),
                look_.X(), look_.Y(), look_.Z(),
                up_.X(), up_.Y(), up_.Z());
    glLightfv(GL_LIGHT1, GL_POSITION, Point3(eye_).Get());
}

//====================================================================================================
// class CircleObject:
//====================================================================================================

CircleObject::CircleObject(GLdouble radius, GLint slices, GLint loops, GLfloat xrotation) {
    object_ = gluNewQuadric();
    radius_ = radius;
    slices_ = slices;
    loops_ = loops;
    xrotation_ = xrotation;
    Initialize();
}

CircleObject::~CircleObject() {
    gluDeleteQuadric(object_);
}

void CircleObject::Initialize() {
    gluQuadricDrawStyle(object_, GLU_LINE);
    gluQuadricOrientation(object_, GLU_OUTSIDE);
    gluQuadricNormals(object_, GLU_SMOOTH);
}

void CircleObject::Draw() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glRotatef(xrotation_, 1.0f, 0.0f, 0.0f);
        drawChildrens();
        material_.Apply();
        glLineWidth(2.0f);
        glDisable(GL_LIGHTING);
        gluDisk(object_, radius_, radius_, slices_, loops_);
        glEnable(GL_LIGHTING);
    glPopMatrix();
}

//====================================================================================================
// class SphereObject:
//====================================================================================================

SphereObject::SphereObject(GLdouble radius, GLint slices, GLint stacks, GLdouble distance) {
    object_ = gluNewQuadric();
    radius_ = radius;
    slices_ = slices;
    stacks_ = stacks;
    distance_ = distance;
    Initialize();
}

SphereObject::~SphereObject() {
    gluDeleteQuadric(object_);
}

void SphereObject::Initialize() {
    rotation_ = 0.0;
    orbitRotation_ = 0.0;
    gluQuadricDrawStyle(object_, GLU_FILL);
    gluQuadricOrientation(object_, GLU_OUTSIDE);
    gluQuadricNormals(object_, GLU_SMOOTH);
    gluQuadricTexture(object_, GL_TRUE);
}

void SphereObject::Draw() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glRotated(orbitRotation_, 0.0, 1.0, 0.0);
        glTranslated(distance_, 0.0, 0.0);
        glRotated(-orbitRotation_, 0.0, 1.0, 0.0);
        drawChildrens();
        glRotated(rotation_, 0.0, 1.0, 0.0);
        glRotated(-90.0, 1.0, 0.0, 0.0);
        material_.Apply();
        gluSphere(object_, radius_, slices_, stacks_);
    glPopMatrix();
}

void SphereObject::SetRotation(GLdouble value) {
    rotation_ = std::fmod(value, 360.0);
}

void SphereObject::SetOrbitRotation(GLdouble value) {
    orbitRotation_ = std::fmod(value, 360.0);
}

void SphereObject::AddRotation(GLdouble value) {
    SetRotation(rotation_ + value);
}

void SphereObject::AddOrbitRotation(GLdouble value) {
    SetOrbitRotation(orbitRotation_ + value);
}

//====================================================================================================
// class SatelliteObject:
//====================================================================================================

static const GLfloat SATELLITE_VERTEX[] = {
    // GL_TRIANGLES 0..35 (12)
     0.0f,  0.0f,  0.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
     0.0f,  0.0f,  0.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,
     0.0f,  0.0f,  0.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
     0.0f,  0.0f,  0.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,
    // GL_TRIANGLE_STRIP 36..47 (4)
    -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
    // GL_TRIANGLE_STRIP 48..59 (4)
    -1.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.0f, -1.0f, -1.0f,  0.0f,  1.0f, -1.0f,  0.0f
};
static const GLfloat SATELLITE_NORMAL[] = {
    // GL_TRIANGLES 0..35 (12)
     0.0f,  1.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.0f,  1.0f,  1.0f,
     1.0f,  0.0f,  1.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.0f,  1.0f,
     0.0f, -1.0f,  1.0f,  0.0f, -1.0f,  1.0f,  0.0f, -1.0f,  1.0f,
    -1.0f,  0.0f,  1.0f, -1.0f,  0.0f,  1.0f, -1.0f,  0.0f,  1.0f,
    // GL_TRIANGLE_STRIP 36..47 (4)
     0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,
    // GL_TRIANGLE_STRIP 48..59 (4)
     0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f
};
static const GLfloat SATELLITE_COLOR[] = {
    // GL_TRIANGLES 0..35 (12)
    0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f,
    0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f,
    0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f,
    0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f,
    // GL_TRIANGLE_STRIP 36..47 (4)
    0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f,
    // GL_TRIANGLE_STRIP 48..59 (4)
    0.0f, 0.3f, 0.6f, 0.0f, 0.3f, 0.6f, 0.0f, 0.3f, 0.6f, 0.0f, 0.3f, 0.6f
};

SatelliteObject::SatelliteObject(GLdouble distance) {
    distance_ = distance;
    Initialize();
}

SatelliteObject::~SatelliteObject() {
}

void SatelliteObject::Initialize() {
    rotation_ = 0.0;
    orbitRotation_ = 0.0;
    glVertexPointer(3, GL_FLOAT, 0, SATELLITE_VERTEX);
    glNormalPointer(GL_FLOAT, 0, SATELLITE_NORMAL);
    glColorPointer(3, GL_FLOAT, 0, SATELLITE_COLOR);
}

void SatelliteObject::Draw() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glRotated(-90.0, 1.0, 0.0, 0.0);
        glRotated(orbitRotation_, 0.0, 1.0, 0.0);
        glTranslated(distance_, 0.0, 0.0);
        glRotated(-orbitRotation_, 0.0, 1.0, 0.0);
        glRotated(-90.0, 1.0, 0.0, 0.0);
        glRotated(rotation_, 0.0, 1.0, 0.0);
        material_.Apply();
        drawSide();
        glRotated(180.0, 0.0, 1.0, 0.0);
        drawSide();
    glPopMatrix();
}

void SatelliteObject::SetRotation(GLdouble value) {
    rotation_ = std::fmod(value, 360.0);
}

void SatelliteObject::SetOrbitRotation(GLdouble value) {
    orbitRotation_ = std::fmod(value, 360.0);
}

void SatelliteObject::AddRotation(GLdouble value) {
    SetRotation(rotation_ + value);
}

void SatelliteObject::AddOrbitRotation(GLdouble value) {
    SetOrbitRotation(orbitRotation_ + value);
}

void SatelliteObject::drawSide() {
    glDrawArrays(GL_TRIANGLES, 0, 12);
    glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
    /*
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_TRIANGLES);
        glNormal3f( 0.0f,  1.0f,  1.0f);
        glVertex3f( 0.0f,  0.0f,  0.0f);
        glVertex3f( 1.0f,  1.0f, -1.0f);
        glVertex3f(-1.0f,  1.0f, -1.0f);

        glNormal3f( 1.0f,  0.0f,  1.0f);
        glVertex3f( 0.0f,  0.0f,  0.0f);
        glVertex3f( 1.0f, -1.0f, -1.0f);
        glVertex3f( 1.0f,  1.0f, -1.0f);

        glNormal3f( 0.0f, -1.0f,  1.0f);
        glVertex3f( 0.0f,  0.0f,  0.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f( 1.0f, -1.0f, -1.0f);

        glNormal3f(-1.0f,  0.0f,  1.0f);
        glVertex3f( 0.0f,  0.0f,  0.0f);
        glVertex3f(-1.0f,  1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();
    glBegin(GL_TRIANGLE_STRIP);
        glNormal3f( 0.0f,  0.0f, -1.0f);
        glVertex3f(-1.0f,  1.0f, -1.0f);
        glVertex3f( 1.0f,  1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f( 1.0f, -1.0f, -1.0f);
    glEnd();
    glColor3f(0.0f, 0.3f, 0.6f);
    glBegin(GL_TRIANGLE_STRIP);
        glNormal3f( 0.0f,  0.0f, -1.0f);
        glVertex3f(-1.0f,  1.0f,  0.0f);
        glVertex3f( 1.0f,  1.0f,  0.0f);
        glVertex3f(-1.0f, -1.0f,  0.0f);
        glVertex3f( 1.0f, -1.0f,  0.0f);
    glEnd();
    //*/
}

//====================================================================================================
// class SatelliteObject:
//====================================================================================================

const GLfloat AxesObject::AXIS_LEN = 50.0f;

AxesObject::AxesObject() {}

AxesObject::~AxesObject() {}

void AxesObject::Initialize() {}

void AxesObject::Draw() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        material_.Apply();
        glDisable(GL_LIGHTING);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
            // Red X-axis:
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(AXIS_LEN, 0.0f, 0.0f);
            // Green Y-axis:
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, AXIS_LEN, 0.0f);
            // Blue Z-axis:
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, AXIS_LEN);
        glEnd();
        glEnable(GL_LIGHTING);
    glPopMatrix();
}
