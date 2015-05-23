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

#include "events.h"
#include "data.h"
#include "render.h"

//****************************************************************************************************
// Event Functions
//****************************************************************************************************

void OnRender () {
    if (UseOneVP) {
        DrawScene();
    } else {
        DrawScene4xN();
    }
    glutSwapBuffers();
}

//----------------------------------------------------------------------------------------------------

void OnResize (int width, int height) {
    WindowWidth = width, WindowHeight = height;
    if (UseOneVP) {
        glViewport(0, 0, WindowWidth, WindowHeight);
        InitializeProjection();
    } else {
        UpdateMVPConfiguration();
    }
    glutPostRedisplay();
}

//----------------------------------------------------------------------------------------------------

void OnKeyboard (unsigned char ascii, int mouseX, int mouseY) {
    bool repaint = true;
    switch (ascii) {
    case 27:
        glutLeaveMainLoop();
        break;
    case '+':
        if(CurrentScale < 10.0) {
            CurrentScale += 0.25;
            OnResize(WindowWidth, WindowHeight);
        }
        break;
    case '-':
        if(CurrentScale > 0.3) {
            CurrentScale -= 0.25;
            OnResize(WindowWidth, WindowHeight);
        }
        break;
    case 's':
        if (UseTimer) {
            repaint = false;
        } else {
            OneStepRotation();
        }
        break;
    case 't':
        UseOneVP = !UseOneVP;
        OnResize(WindowWidth, WindowHeight);
        break;
    case 'o':
        SetCameraAtInitial();
        break;
    case 'x':
        SetCameraAtX();
        break;
    case 'y':
        SetCameraAtY();
        break;
    case 'z':
        SetCameraAtZ();
        break;
    case 'p':
        UseOrtho = !UseOrtho;
        InitializeProjection();
        break;
    case 'i':
        ResetConfiguration();
        break;
    case 'n':
        MainCamera.MoveForward(CAMERA_INC);
        MainCamera.Apply();
        break;
    case 'N':
        MainCamera.MoveBackward(CAMERA_INC);
        MainCamera.Apply();
        break;
    case 'u':
        MainCamera.MoveRight(CAMERA_INC);
        MainCamera.Apply();
        break;
    case 'U':
        MainCamera.MoveLeft(CAMERA_INC);
        MainCamera.Apply();
        break;
    case 'v':
        MainCamera.MoveUp(CAMERA_INC);
        MainCamera.Apply();
        break;
    case 'V':
        MainCamera.MoveDown(CAMERA_INC);
        MainCamera.Apply();
        break;
    case 'g':
        MainCamera.RotateLeft(CAMERA_INC);
        MainCamera.Apply();
        break;
    default:
        repaint = false;
    }
    if (repaint) {
        glutPostRedisplay();
    }
}

//----------------------------------------------------------------------------------------------------

void OnSpecial (int keyCode, int mouseX, int mouseY) {
    bool repaint = true;
    switch (keyCode) {
    case GLUT_KEY_F1:
        SetCameraAtInitial();
        break;
    case GLUT_KEY_F2:
        SetCameraAtX();
        break;
    case GLUT_KEY_F3:
        SetCameraAtY();
        break;
    case GLUT_KEY_F4:
        SetCameraAtZ();
        break;
    case GLUT_KEY_F5:
        ResetConfiguration();
        break;
    case GLUT_KEY_F8:
        UseAxes = !UseAxes;
        break;
    case GLUT_KEY_F9:
        UseTimer = !UseTimer;
        if (UseTimer) {
            glutTimerFunc(40, OnTimer, 0);
        }
        break;
    case GLUT_KEY_UP:
        MainCamera.MoveForward(CAMERA_INC);
        MainCamera.Apply();
        break;
    case GLUT_KEY_DOWN:
        MainCamera.MoveBackward(CAMERA_INC);
        MainCamera.Apply();
        break;
    case GLUT_KEY_LEFT:
        MainCamera.MoveLeft(CAMERA_INC);
        MainCamera.Apply();
        break;
    case GLUT_KEY_RIGHT:
        MainCamera.MoveRight(CAMERA_INC);
        MainCamera.Apply();
        break;
    case GLUT_KEY_PAGE_UP:
        MainCamera.MoveUp(CAMERA_INC);
        MainCamera.Apply();
        break;
    case GLUT_KEY_PAGE_DOWN:
        MainCamera.MoveDown(CAMERA_INC);
        MainCamera.Apply();
        break;
    case GLUT_KEY_DELETE:
    case GLUT_KEY_F12:
        MainCamera.RotateRight(CAMERA_INC);
        MainCamera.Apply();
        break;
    case GLUT_KEY_F11:
        MainCamera.RotateLeft(CAMERA_INC);
        MainCamera.Apply();
        break;
    default:
        repaint = false;
    }
    if (repaint) {
        glutPostRedisplay();
    }
}

//----------------------------------------------------------------------------------------------------

void OnTimer (int value) {
    if (UseTimer) {
        OneStepRotation();
        glutPostRedisplay();
        glutTimerFunc(40, OnTimer, 0);
    }
}
