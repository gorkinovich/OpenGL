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

#ifndef __GENTITY_H__
#define __GENTITY_H__

#include "gsystem.h"
#include <gl/GLU.h>

//----------------------------------------------------------------------------------------------------
// Camera
//----------------------------------------------------------------------------------------------------

class Camera {
private:
    Vector3 eye_, look_, up_;
    void moveN (GLfloat value);
    void moveU (GLfloat value);
    void moveV (GLfloat value);
    void rotate (GLfloat value);
public:
    Camera() {}
    void SetEye(GLfloat x, GLfloat y, GLfloat z);
    void SetLook(GLfloat x, GLfloat y, GLfloat z);
    void SetUp(GLfloat x, GLfloat y, GLfloat z);
    void Apply();
    inline void MoveForward(GLfloat value) { moveN( value); }
    inline void MoveBackward(GLfloat value) { moveN(-value); }
    inline void MoveLeft(GLfloat value) { moveU( value); }
    inline void MoveRight(GLfloat value) { moveU(-value); }
    inline void MoveUp(GLfloat value) { moveV( value); }
    inline void MoveDown(GLfloat value) { moveV(-value); }
    inline void RotateLeft(GLfloat value) { rotate( value); }
    inline void RotateRight(GLfloat value) { rotate(-value); }
};

//----------------------------------------------------------------------------------------------------
// CircleObject
//----------------------------------------------------------------------------------------------------

class CircleObject : public NodeObject {
protected:
    GLUquadricObj * object_;
    GLdouble radius_;
    GLint slices_;
    GLint loops_;
    GLfloat xrotation_;
public:
    CircleObject(GLdouble radius, GLint slices, GLint loops, GLfloat xrotation = 0.0f);
    virtual ~CircleObject();
    virtual void Initialize();
    virtual void Draw();
};

//----------------------------------------------------------------------------------------------------
// SphereObject
//----------------------------------------------------------------------------------------------------

class SphereObject : public NodeObject {
private:
    GLUquadricObj * object_;
    GLdouble radius_;
    GLint slices_;
    GLint stacks_;
    GLdouble distance_;
    GLdouble rotation_;
    GLdouble orbitRotation_;
public:
    SphereObject(GLdouble radius, GLint slices, GLint stacks, GLdouble distance);
    virtual ~SphereObject();
    virtual void Initialize();
    virtual void Draw();
    void SetRotation(GLdouble value);
    void SetOrbitRotation(GLdouble value);
    void AddRotation(GLdouble value);
    void AddOrbitRotation(GLdouble value);
};

//----------------------------------------------------------------------------------------------------
// SatelliteObject
//----------------------------------------------------------------------------------------------------

class SatelliteObject : public NodeObject {
private:
    GLdouble distance_;
    GLdouble rotation_;
    GLdouble orbitRotation_;
    void drawSide();
public:
    SatelliteObject(GLdouble distance);
    virtual ~SatelliteObject();
    virtual void Initialize();
    virtual void Draw();
    void SetRotation(GLdouble value);
    void SetOrbitRotation(GLdouble value);
    void AddRotation(GLdouble value);
    void AddOrbitRotation(GLdouble value);
};

//----------------------------------------------------------------------------------------------------
// AxesObject
//----------------------------------------------------------------------------------------------------

class AxesObject : public NodeObject {
private:
    static const GLfloat AXIS_LEN;
public:
    AxesObject ();
    virtual ~AxesObject ();
    virtual void Initialize ();
    virtual void Draw ();
};

#endif
