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

#include "render.h"
#include "data.h"

//****************************************************************************************************
// Initialization Functions
//****************************************************************************************************

void InitializeGL () {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, DIFFUSE_L0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, SPECULAR_L0);
    glLightfv(GL_LIGHT0, GL_POSITION, POSITION_L0);

    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, DIFFUSE_L1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, SPECULAR_L1);
    glLightfv(GL_LIGHT1, GL_POSITION, POSITION_L0);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, AMBIENT_COLOR);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glViewport(0, 0, WindowWidth, WindowHeight);
    InitializeProjection();
    SetCameraAtInitial();
}

//----------------------------------------------------------------------------------------------------

void InitializeProjection () {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (UseOrtho) {
        GLdouble x = (WindowWidth * 0.5) / CurrentScale;
        GLdouble y = (WindowHeight * 0.5) / CurrentScale;
        glOrtho(-x, x, -y, y, NEAR_PLANE, FAR_PLANE);
    } else {
        GLdouble aspect = (GLdouble)WindowWidth / (GLdouble)WindowHeight;
        gluPerspective(FOVY / CurrentScale, aspect, NEAR_PLANE, FAR_PLANE);
    }
}

//----------------------------------------------------------------------------------------------------

void InitializeScene () {
    SunSphere.Initialize();
    SunSphere.GetMaterial().SetColor(0.2f, 0.2f, 0.2f);
    SunSphere.GetMaterial().SetEmission(0.9f, 0.9f, 0.3f);

    EarthSphere.Initialize();
    MoonSphere.Initialize();
    HumanSatellite.Initialize();

    EarthOrbit.Initialize();
    EarthOrbit.GetMaterial().SetColor(0.4f, 0.25f, 0.04f);

    MoonOrbit.Initialize();
    MoonOrbit.GetMaterial().SetColor(0.5f, 0.5f, 0.5f);

    SatelliteOrbit.Initialize();
    SatelliteOrbit.GetMaterial().SetColor(0.2f, 0.2f, 0.5f);

    if (EarthTexture.LoadBMP("earth.bmp")) {
        EarthSphere.GetMaterial().SetTexture(EarthTexture.Name());
    }

    if (MoonTexture.LoadBMP("moon.bmp")) {
        MoonSphere.GetMaterial().SetTexture(MoonTexture.Name());
    }

    SunSphere.AddChildren(&EarthOrbit);
    SunSphere.AddChildren(&EarthSphere);

    EarthSphere.AddChildren(&MoonOrbit);
    EarthSphere.AddChildren(&MoonSphere);
    EarthSphere.AddChildren(&SatelliteOrbit);
    EarthSphere.AddChildren(&HumanSatellite);
}

//----------------------------------------------------------------------------------------------------

void ResetConfiguration () {
    CurrentScale = 1.0;
    UseOrtho = true;
    UseOneVP = true;
    UseTimer = false;
    UseAxes  = true;
    UpdateMVPConfiguration();
    glViewport(0, 0, WindowWidth, WindowHeight);
    InitializeProjection();
    SetCameraAtInitial();
    InitializeScene();
}

//----------------------------------------------------------------------------------------------------

void UpdateMVPConfiguration () {
    WindowColumns = 4;
    WindowWidth4  = WindowWidth / WindowColumns;
    WindowHeight4 = WindowWidth4 * WindowHeight / WindowWidth;
    WindowRows    = WindowHeight / WindowHeight4;
}

//****************************************************************************************************
// Render Functions
//****************************************************************************************************

void DrawScene () {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    if (UseAxes) MainAxes.Draw();
    SunSphere.Draw();
}

//----------------------------------------------------------------------------------------------------

void DrawScene4xN () {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLint x, y;
    for (GLint i = 0; i < WindowRows; ++i) {
        for (GLint j = 0; j < WindowColumns; ++j) {
            x = j * WindowWidth4, y = i * WindowHeight4;
            glScissor(x, y, WindowWidth4, WindowHeight4);
            glViewport(x, y, WindowWidth4, WindowHeight4);
            InitializeProjection();
            glMatrixMode(GL_MODELVIEW);
            if (UseAxes) MainAxes.Draw();
            SunSphere.Draw();
        }
    }
    glScissor(0, 0, WindowWidth, WindowHeight);
}

//****************************************************************************************************
// Rotation Functions
//****************************************************************************************************

void OneStepRotation () {
    EarthSphere.AddOrbitRotation(1.0);
    EarthSphere.AddRotation(2.0);
    MoonSphere.AddOrbitRotation(4.0);
    MoonSphere.AddRotation(4.0);
    HumanSatellite.AddOrbitRotation(8.0);
    HumanSatellite.AddRotation(8.0);
}

//****************************************************************************************************
// Camera Functions
//****************************************************************************************************

void SetCameraAtInitial () {
    MainCamera.SetEye(CAMERA_EYE_XI, CAMERA_EYE_YI, CAMERA_EYE_ZI);
    MainCamera.SetLook(CAMERA_LOOK_X, CAMERA_LOOK_Y, CAMERA_LOOK_Z);
    MainCamera.SetUp(CAMERA_UP_XI, CAMERA_UP_YI, CAMERA_UP_ZI);
    MainCamera.Apply();
}

//----------------------------------------------------------------------------------------------------

void SetCameraAtX () {
    MainCamera.SetEye(CAMERA_EYE_XX, CAMERA_EYE_YX, CAMERA_EYE_ZX);
    MainCamera.SetLook(CAMERA_LOOK_X, CAMERA_LOOK_Y, CAMERA_LOOK_Z);
    MainCamera.SetUp(CAMERA_UP_XI, CAMERA_UP_YI, CAMERA_UP_ZI);
    MainCamera.Apply();
}

//----------------------------------------------------------------------------------------------------

void SetCameraAtY () {
    MainCamera.SetEye(CAMERA_EYE_XY, CAMERA_EYE_YY, CAMERA_EYE_ZY);
    MainCamera.SetLook(CAMERA_LOOK_X, CAMERA_LOOK_Y, CAMERA_LOOK_Z);
    MainCamera.SetUp(CAMERA_UP_XY, CAMERA_UP_YY, CAMERA_UP_ZY);
    MainCamera.Apply();
}

//----------------------------------------------------------------------------------------------------

void SetCameraAtZ () {
    MainCamera.SetEye(CAMERA_EYE_XZ, CAMERA_EYE_YZ, CAMERA_EYE_ZZ);
    MainCamera.SetLook(CAMERA_LOOK_X, CAMERA_LOOK_Y, CAMERA_LOOK_Z);
    MainCamera.SetUp(CAMERA_UP_XI, CAMERA_UP_YI, CAMERA_UP_ZI);
    MainCamera.Apply();
}
