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

#include "core.hpp"

//****************************************************************************************************
// Render functions:
//****************************************************************************************************

void Core::InitializeResources() {
    // Load the background texture:
    BackgroundTexture = std::make_shared<Texture>();
    if (!BackgroundTexture->LoadBMP("starbuck.bmp")) {
        std::cerr << "[ERROR] Texture not loaded!" << std::endl;
        std::exit(0);
    }
    // Load the background rectangle:
    GLfloat w = (GLfloat)BackgroundTexture->Width();
    GLfloat h = (GLfloat)BackgroundTexture->Height();
    BackgroundRectangle = std::make_shared<GraphicRectangle>(0.0f, 0.0f, w, h);
    // Finish the settings:
    srand((int)time(NULL));
    SelectedTriangle = nullptr;
}

//----------------------------------------------------------------------------------------------------

void Core::InitializeRender() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_CULL_FACE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);

    glShadeModel(GL_SMOOTH);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    glViewport(0, 0, WindowWidth, WindowHeight);
    InitializeProjection();
    InitializeCamera();
    InitializeScene();
}

//----------------------------------------------------------------------------------------------------

void Core::InitializeProjection() {
    projectionLeft = 0;
    projectionRight = WindowWidth;
    projectionTop = WindowHeight;
    projectionBottom = 0;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(projectionLeft, projectionRight, projectionBottom, projectionTop, NEAR_PLANE, FAR_PLANE);
}

//----------------------------------------------------------------------------------------------------

void Core::InitializeCamera() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}

//----------------------------------------------------------------------------------------------------

void Core::InitializeScene() {
    GotoDesignState();
    LastPoints.clear();
    CurrentTriangles.clear();
    SelectedTriangle = nullptr;
    ShowBackground = true;
}

//----------------------------------------------------------------------------------------------------

void Core::ResetConfiguration() {
    InitializeProjection();
    InitializeCamera();
    InitializeScene();
}

//----------------------------------------------------------------------------------------------------

void Core::RenderPoints() {
    glColor3f(0.0f, 1.0f, 0.0f);
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    std::for_each(std::begin(LastPoints), std::end(LastPoints),
        [] (const Vector2D & p) { glVertex2f(p.X(), p.Y()); });
    glEnd();
    glPointSize(1.0f);
}

//----------------------------------------------------------------------------------------------------

void Core::RenderTriangles() {
    std::for_each(
        std::begin(CurrentTriangles), std::end(CurrentTriangles),
        [&] (std::shared_ptr<GraphicTriangle> & victim) {
            victim->Draw(*BackgroundTexture);
        }
    );
}

//----------------------------------------------------------------------------------------------------

void Core::RenderTrianglesWithBorder() {
    std::for_each(
        std::begin(CurrentTriangles), std::end(CurrentTriangles),
        [&] (std::shared_ptr<GraphicTriangle> & victim) {
            victim->DrawWithBorder(*BackgroundTexture);
        }
    );
}

//----------------------------------------------------------------------------------------------------

void Core::RenderWiredTriangles() {
    std::for_each(
        std::begin(CurrentTriangles), std::end(CurrentTriangles),
        [] (std::shared_ptr<GraphicTriangle> & victim) {
            victim->WireDraw();
        }
    );
}
