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

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/freeglut.h>
#include <memory>

#include "gmath.h"
#include "gmesh.h"
#include "gobject.h"
#include "generator.h"

//****************************************************************************************************
// References:
//****************************************************************************************************

// http://www.matrix44.net/cms/notes/opengl-3d-graphics/basic-3d-math-matrices

//****************************************************************************************************
// Types:
//****************************************************************************************************

class RodMesh {
private:
    SimpleMesh3D data_;
    static const int SLICES = 18; //36;
public:
    RodMesh(const std::string & path) {
        Outline3D outline;
        outline.LoadFromFile(path);
        data_ = Generator3D::SimpleOutlineByRotation(outline, SLICES, true);
        data_.SetColor(1.0f, 0.0f, 0.0f);
    }
    inline SimpleMesh3D & Data() { return data_; }
    inline void Draw() { data_.Draw(); }
};

class BoxMesh {
private:
    SimpleMesh3D data_;
public:
    BoxMesh() {
        data_ = Generator3D::SimpleBox(120.0f);
        data_.SetColor(1.0f, 0.0f, 0.0f);
    }
    inline SimpleMesh3D & Data() { return data_; }
    inline void Draw() { data_.Draw(); }
};

//****************************************************************************************************
// Data:
//****************************************************************************************************

const GLsizei WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
const GLdouble NEAR_PLANE = -1000.0, FAR_PLANE = 1000.0;
const GLfloat ROTATE_SPEED = 10.0f;
const int NUMBER_OF_RODS = 6;

GLsizei  WindowWidth  = WINDOW_WIDTH;
GLsizei  WindowHeight = WINDOW_HEIGHT;
GLdouble CurrentScale = 1.0;

GLfloat CurrentRotateX = 0.0f;
GLfloat CurrentRotateY = 0.0f;
GLfloat CurrentRotateZ = 0.0f;

std::shared_ptr<IMesh3D> RodModel;
std::shared_ptr<IMesh3D> BoxModel;

std::shared_ptr<Object3D> BoxObject;
std::shared_ptr<Object3D> RodObject[NUMBER_OF_RODS];
std::shared_ptr<ObjectContainer3D> SceneObject;

//****************************************************************************************************
// Render functions:
//****************************************************************************************************

void InitializeResources() {
    BoxMesh box;
    RodMesh rod("staff.outline");
    BoxModel.reset(new SimpleMesh3D(box.Data()));
    RodModel.reset(new SimpleMesh3D(rod.Data()));

    SceneObject = std::make_shared<ObjectContainer3D>();
    BoxObject.reset(new MeshObject3D(BoxModel));
    SceneObject->AddChild(BoxObject);

    for (int i = 0; i < NUMBER_OF_RODS; ++i) {
        RodObject[i].reset(new MeshObject3D(RodModel));
        SceneObject->AddChild(RodObject[i]);
    }

    RodObject[1]->Transform().AddRotate(90.0f, 0.0f, 0.0f, 1.0f);
    RodObject[2]->Transform().AddRotate(180.0f, 0.0f, 0.0f, 1.0f);
    RodObject[3]->Transform().AddRotate(270.0f, 0.0f, 0.0f, 1.0f);
    RodObject[4]->Transform().AddRotate(90.0f, 1.0f, 0.0f, 0.0f);
    RodObject[5]->Transform().AddRotate(270.0f, 1.0f, 0.0f, 0.0f);

    const GLfloat DISTANCE = 60.0f;
    for (int i = 0; i < NUMBER_OF_RODS; ++i) {
        RodObject[i]->Transform().AddTranslate(0.0f, DISTANCE, 0.0f);
    }
}

//----------------------------------------------------------------------------------------------------

void InitializeCamera() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 400.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}

//----------------------------------------------------------------------------------------------------

void InitializeProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLdouble x = (WindowWidth * 0.5) / CurrentScale;
    GLdouble y = (WindowHeight * 0.5) / CurrentScale;
    glOrtho(-x, x, -y, y, NEAR_PLANE, FAR_PLANE);
}

//----------------------------------------------------------------------------------------------------

void InitializeScene() {
    CurrentRotateX = 0.0f;
    CurrentRotateY = 0.0f;
    CurrentRotateZ = 0.0f;
}

//----------------------------------------------------------------------------------------------------

void InitializeRender() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glShadeModel(GL_SMOOTH);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    const GLfloat AMBIENT[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    const GLfloat DIFFUSE[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat POSITION[] = { 1.0f, 1.0f, 1.0f, 0.0f };

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, DIFFUSE);
    glLightfv(GL_LIGHT0, GL_POSITION, POSITION);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, AMBIENT);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glViewport(0, 0, WindowWidth, WindowHeight);
    InitializeProjection();
    InitializeCamera();
    InitializeScene();
}

//----------------------------------------------------------------------------------------------------

void ResetConfiguration() {
    CurrentScale = 1.0;
    InitializeProjection();
    InitializeCamera();
    InitializeScene();
}

//****************************************************************************************************
// Event functions:
//****************************************************************************************************

void OnRender() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glRotatef(CurrentRotateX, 1.0f, 0.0f, 0.0f);
        glRotatef(CurrentRotateY, 0.0f, 1.0f, 0.0f);
        glRotatef(CurrentRotateZ, 0.0f, 0.0f, 1.0f);
        SceneObject->Draw();
    glPopMatrix();
    glutSwapBuffers();
}

//----------------------------------------------------------------------------------------------------

void OnResize(int width, int height) {
    WindowWidth = width, WindowHeight = height;
    glViewport(0, 0, WindowWidth, WindowHeight);
    InitializeProjection();
    glutPostRedisplay();
}

//----------------------------------------------------------------------------------------------------

void OnKeyboard(unsigned char ascii, int mouseX, int mouseY) {
    bool repaint = true;
    switch (ascii) {
    case 27:
        glutLeaveMainLoop();
        break;
    case '+':
        if (CurrentScale < 10.0) {
            CurrentScale += 0.25;
            OnResize(WindowWidth, WindowHeight);
        }
        break;
    case '-':
        if (CurrentScale > 0.3) {
            CurrentScale -= 0.25;
            OnResize(WindowWidth, WindowHeight);
        }
        break;
    case 'x':
    case 'X':
        CurrentRotateX += ROTATE_SPEED;
        CurrentRotateY = 0.0f;
        CurrentRotateZ = 0.0f;
        while (CurrentRotateX >= 360.0f) {
            CurrentRotateX -= 360.0f;
        }
        break;
    case 'y':
    case 'Y':
        CurrentRotateX = 0.0f;
        CurrentRotateY += ROTATE_SPEED;
        CurrentRotateZ = 0.0f;
        while (CurrentRotateY >= 360.0f) {
            CurrentRotateY -= 360.0f;
        }
        break;
    case 'z':
    case 'Z':
        CurrentRotateX = 0.0f;
        CurrentRotateY = 0.0f;
        CurrentRotateZ += ROTATE_SPEED;
        while (CurrentRotateZ >= 360.0f) {
            CurrentRotateZ -= 360.0f;
        }
        break;
    case 'r':
    case 'R':
        ResetConfiguration();
        break;
    default:
        repaint = false;
    }
    if (repaint) {
        glutPostRedisplay();
    }
}

//----------------------------------------------------------------------------------------------------

void OnSpecial(int keyCode, int mouseX, int mouseY) {
    bool repaint = true;
    switch (keyCode) {
    case GLUT_KEY_F5:
        ResetConfiguration();
        break;
    default:
        repaint = false;
    }
    if (repaint) {
        glutPostRedisplay();
    }
}

//****************************************************************************************************
// Main Entry:
//****************************************************************************************************

int main(int argc, char ** argv) {
    // Initialize the window:
    glutInit(&argc, argv);
    glutInitWindowPosition(-1, -1);
    glutInitWindowSize(WindowWidth, WindowHeight);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    int window = glutCreateWindow("Computer Graphics: Practice 2");

    // Setting the events:
    glutDisplayFunc(OnRender);
    glutReshapeFunc(OnResize);
    glutKeyboardFunc(OnKeyboard);
    glutSpecialFunc(OnSpecial);

    // Initialize OpenGL and the scene:
    InitializeResources();
    InitializeRender();

    // Run the main loop:
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutMainLoop();

    // Finish the execution:
    glutDestroyWindow(window);
    return 0;
}
