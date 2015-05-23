/*****************************************************************************************************
 Copyright (c) 2015 Gorka Su�rez Garc�a

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

#include "include.h"
#include "data.h"
#include "render.h"
#include "events.h"

//****************************************************************************************************
// Main Entry
//****************************************************************************************************

int main (int argc, char ** argv) {
    // Initialize the window:
    glutInit(&argc, argv);
    glutInitWindowPosition(-1, -1);
    glutInitWindowSize(WindowWidth, WindowHeight);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    int window = glutCreateWindow("Gr�ficos por Computador: Pr�ctica 1");

    // Setting the events:
    glutDisplayFunc(OnRender);
    glutReshapeFunc(OnResize);
    glutKeyboardFunc(OnKeyboard);
    glutSpecialFunc(OnSpecial);

    // Initialize OpenGL and the scene:
    InitializeGL();
    InitializeScene();

    // Run the main loop:
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutMainLoop();

    // Finish the execution:
    UseTimer = false;
    EarthTexture.Release();
    MoonTexture.Release();
    glutDestroyWindow(window);
    return 0;
}
