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

#include "include.h"
#include "gentity.h"

const GLdouble SUN_RADIUS   = 20.0, EARTH_RADIUS   = 10.0, MOON_RADIUS   =  5.0;
const GLdouble SUN_DISTANCE =  0.0, EARTH_DISTANCE = 60.0, MOON_DISTANCE = 25.0, SATELLITE_DISTANCE = 15.0;

const GLint SPHERE_SLICES = 32, SPHERE_STACKS = 32;
const GLint CIRCLE_SLICES = 32, CIRCLE_LOOPS  = 32;

GLsizei  WindowWidth  = WINDOW_WIDTH;
GLsizei  WindowHeight = WINDOW_HEIGHT;
GLdouble CurrentScale = 1.0;

bool UseOrtho = true;
bool UseOneVP = true;
bool UseTimer = false;
bool UseAxes  = true;

GLint WindowColumns = 4;
GLint WindowRows    = 4;

GLsizei WindowWidth4  = WINDOW_WIDTH  / 4;
GLsizei WindowHeight4 = WINDOW_HEIGHT / 4;

SphereObject SunSphere(SUN_RADIUS, SPHERE_SLICES, SPHERE_STACKS, SUN_DISTANCE);
SphereObject EarthSphere(EARTH_RADIUS, SPHERE_SLICES, SPHERE_STACKS, EARTH_DISTANCE);
SphereObject MoonSphere(MOON_RADIUS, SPHERE_SLICES, SPHERE_STACKS, MOON_DISTANCE);

SatelliteObject HumanSatellite(SATELLITE_DISTANCE);

CircleObject EarthOrbit(EARTH_DISTANCE, CIRCLE_SLICES, CIRCLE_LOOPS, 90.0f);
CircleObject MoonOrbit(MOON_DISTANCE, CIRCLE_SLICES, CIRCLE_LOOPS, 90.0f);
CircleObject SatelliteOrbit(SATELLITE_DISTANCE, CIRCLE_SLICES, CIRCLE_LOOPS);

AxesObject MainAxes;

Texture EarthTexture;
Texture MoonTexture;

Camera MainCamera;
