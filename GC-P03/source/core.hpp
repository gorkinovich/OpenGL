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

#ifndef __CORE_H__
#define __CORE_H__

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/freeglut.h>
#include <iostream>
#include <memory>
#include <ctime>

#include "gmath.hpp"
#include "gtexture.hpp"
#include "gobject.hpp"

//----------------------------------------------------------------------------------------------------
// Core
//----------------------------------------------------------------------------------------------------

class Core {
public:
    //------------------------------------------------------------------------------------------------
    // Constants:
    //------------------------------------------------------------------------------------------------

    static const int DESIGN_STATE    = 0;
    static const int SELECTION_STATE = 1;
    static const int ANIMATION_STATE = 2;

    static const int MAX_PIXELS_PER_STEP = 32;

    static const unsigned int STEP_TIME = 40; // 25 fps

    static const GLsizei WINDOW_WIDTH, WINDOW_HEIGHT;
    static const GLdouble NEAR_PLANE, FAR_PLANE;

    //------------------------------------------------------------------------------------------------
    // Data:
    //------------------------------------------------------------------------------------------------

    static GLsizei WindowWidth, WindowHeight;
    static GLfloat projectionLeft, projectionRight;
    static GLfloat projectionTop, projectionBottom;

    static bool ShowBackground;
    static bool IsAnimationRunning;
    static int CurrentState;
    static std::vector<Vector2D> LastPoints;
    static std::shared_ptr<Texture> BackgroundTexture;
    static std::shared_ptr<GraphicRectangle> BackgroundRectangle;
    static std::vector<std::shared_ptr<GraphicTriangle>> CurrentTriangles;
    static GraphicTriangle * SelectedTriangle;

    //------------------------------------------------------------------------------------------------
    // Functions:
    //------------------------------------------------------------------------------------------------

    static int GetRandom(int max);
    static Vector2D GetRandomVector(int max);
    static Vector2D WindowToProjection(GLfloat x, GLfloat y);

    static void InitializeResources();
    static void InitializeRender();
    static void InitializeProjection();
    static void InitializeCamera();
    static void InitializeScene();
    static void ResetConfiguration();
    static void RenderPoints();
    static void RenderTriangles();
    static void RenderTrianglesWithBorder();
    static void RenderWiredTriangles();

    static void OnRender();
    static void OnResize(int width, int height);
    static void OnMouseDesignState(int button, int state, int x, int y);
    static void OnMouseSelectionState(int button, int state, int x, int y);
    static void OnMouseAnimationState(int button, int state, int x, int y);
    static void OnKeyboard(unsigned char ascii, int mouseX, int mouseY);
    static void OnKeyboardDesignState(unsigned char ascii, int mouseX, int mouseY);
    static void OnKeyboardSelectionState(unsigned char ascii, int mouseX, int mouseY);
    static void OnKeyboardAnimationState(unsigned char ascii, int mouseX, int mouseY);
    static void OnSpecial(int keyCode, int mouseX, int mouseY);
    static void OnTimer(int value);

    static void GotoDesignState();
    static void GotoSelectionState();
    static void GotoAnimationState();
    static void MakeTriangle();
    static void DesignAddPoint(int x, int y);
    static void DesignRemovePoints();
    static void RemoveSelectTriangle();
    static void MoveSelectedTriangle(const Vector2D & point, bool updateTexCoords = false);
    static void SelectTriangle(const Vector2D & point);
    static void UnselectTriangle();
    static void AnimationStep();
    static void AnimationStart();

    static void Start(int argc, char ** argv);
};

#endif
