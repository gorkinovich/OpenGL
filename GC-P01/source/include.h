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

#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cmath>
#include <vector>
#include <iostream>
#include <functional>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/freeglut.h>

#include "gsystem.h"

const GLsizei WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
const GLfloat CAMERA_INC = 10.0f;
const GLdouble NEAR_PLANE = 0.5, FAR_PLANE = 1000.0, FOVY = 45.0;

const GLfloat CAMERA_EYE_XI = 100.0f, CAMERA_EYE_YI = 100.0f, CAMERA_EYE_ZI = 100.0f;
const GLfloat CAMERA_EYE_XX = 100.0f, CAMERA_EYE_YX =   0.0f, CAMERA_EYE_ZX =   0.0f;
const GLfloat CAMERA_EYE_XY =   0.0f, CAMERA_EYE_YY = 100.0f, CAMERA_EYE_ZY =   0.0f;
const GLfloat CAMERA_EYE_XZ =   0.0f, CAMERA_EYE_YZ =   0.0f, CAMERA_EYE_ZZ = 100.0f;
const GLfloat CAMERA_LOOK_X =   0.0f, CAMERA_LOOK_Y =   0.0f, CAMERA_LOOK_Z =   0.0f;
const GLfloat CAMERA_UP_XI  =   0.0f, CAMERA_UP_YI  =   1.0f, CAMERA_UP_ZI  =   0.0f;
const GLfloat CAMERA_UP_XY  =   0.0f, CAMERA_UP_YY  =   0.0f, CAMERA_UP_ZY  =  -1.0f;

const GLfloat DIFFUSE_L0[]    = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat SPECULAR_L0[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat POSITION_L0[]   = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat DIFFUSE_L1[]    = { 0.3f, 0.3f, 0.3f, 1.0f };
const GLfloat SPECULAR_L1[]   = { 0.3f, 0.3f, 0.3f, 1.0f };
const GLfloat AMBIENT_COLOR[] = { 0.1f, 0.1f, 0.1f, 1.0f };

#endif
