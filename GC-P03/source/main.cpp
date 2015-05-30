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
// Data:
//****************************************************************************************************

const GLsizei Core::WINDOW_WIDTH = 800, Core::WINDOW_HEIGHT = 600;
const GLdouble Core::NEAR_PLANE = -10.0, Core::FAR_PLANE = 10.0;

GLsizei Core::WindowWidth  = Core::WINDOW_WIDTH;
GLsizei Core::WindowHeight = Core::WINDOW_HEIGHT;

GLfloat Core::projectionLeft = 0;
GLfloat Core::projectionRight = Core::WINDOW_WIDTH;
GLfloat Core::projectionTop = Core::WINDOW_HEIGHT;
GLfloat Core::projectionBottom = 0;

bool Core::ShowBackground = true;
bool Core::IsAnimationRunning = false;
int Core::CurrentState = 0;
std::vector<Vector2D> Core::LastPoints;
std::shared_ptr<Texture> Core::BackgroundTexture;
std::shared_ptr<GraphicRectangle> Core::BackgroundRectangle;
std::vector<std::shared_ptr<GraphicTriangle>> Core::CurrentTriangles;
GraphicTriangle * Core::SelectedTriangle = nullptr;

//****************************************************************************************************
// Functions:
//****************************************************************************************************

int Core::GetRandom(int max) {
    return rand() % max;
}

//----------------------------------------------------------------------------------------------------

Vector2D Core::GetRandomVector(int max) {
    Vector2D victim((GLfloat)GetRandom(max), 0.0f);
    return victim.Rotate((float)GetRandom(360));
}

//----------------------------------------------------------------------------------------------------

Vector2D Core::WindowToProjection(GLfloat x, GLfloat y) {
    return Vector2D(
        x - projectionLeft,
        ((WindowHeight - 1) - y) - projectionBottom
    );
}

//----------------------------------------------------------------------------------------------------

void Core::GotoDesignState() {
    std::cout << "[DESIGN] Mode activated" << std::endl;
    IsAnimationRunning = false;
    CurrentState = DESIGN_STATE;
    glutMouseFunc(OnMouseDesignState);
    glutKeyboardFunc(OnKeyboardDesignState);
}

//----------------------------------------------------------------------------------------------------

void Core::GotoSelectionState() {
    if (CurrentTriangles.empty()) {
        std::cout << "[ERROR] Put at least one triangle to enter in selection mode..." << std::endl;
    } else {
        std::cout << "[SELECTION] Mode activated" << std::endl;
        IsAnimationRunning = false;
        CurrentState = SELECTION_STATE;
        glutMouseFunc(OnMouseSelectionState);
        glutKeyboardFunc(OnKeyboardSelectionState);
    }
}

//----------------------------------------------------------------------------------------------------

void Core::GotoAnimationState() {
    if (CurrentTriangles.empty()) {
        std::cout << "[ERROR] Put at least one triangle to enter in animation mode..." << std::endl;
    } else {
        std::cout << "[ANIMATION] Mode activated" << std::endl;
        CurrentState = ANIMATION_STATE;
        glutMouseFunc(OnMouseAnimationState);
        glutKeyboardFunc(OnKeyboardAnimationState);
        AnimationStart();
    }
}

//----------------------------------------------------------------------------------------------------

void Core::MakeTriangle() {
    auto toTexCoords = [&] (const Vector2D & p) {
        return Vector2D(p.X() / (float)BackgroundTexture->Width(),
            p.Y() / (float)BackgroundTexture->Height());
    };
    auto addTriangle = [&] (const Vector2D & p0, const Vector2D & p1, const Vector2D & p2) {
        auto victim = std::make_shared<GraphicTriangle>(p0, p1, p2,
            toTexCoords(p0), toTexCoords(p1), toTexCoords(p2));
        victim->SetColor(1.0f, 0.0f, 0.0f);
        victim->SetSpeed(GetRandomVector(MAX_PIXELS_PER_STEP));
        CurrentTriangles.push_back(victim);
    };
    while (LastPoints.size() >= 3) {
        Vector2D p0 = LastPoints[0], p1 = LastPoints[1], p2 = LastPoints[2];
        LastPoints.erase(LastPoints.begin(), LastPoints.begin() + 3);
        Vector2D v1 = p1 - p0, v2 = p2 - p0;
        Vector2D n = v1.LeftPerpendicular();
        if (n.Dot(v2) > 0.0f) {
            addTriangle(p0, p1, p2);
        } else {
            addTriangle(p0, p2, p1);
        }
    }
}

//----------------------------------------------------------------------------------------------------

void Core::DesignAddPoint(int x, int y) {
    Vector2D point = WindowToProjection(x, y);
    std::cout << "[DESIGN] Point(" << LastPoints.size() << ") = "
                << point.X() << "," << point.Y() << std::endl;
    LastPoints.push_back(point);
    MakeTriangle();
}

//----------------------------------------------------------------------------------------------------

void Core::DesignRemovePoints() {
    std::cout << "[DESIGN] Removing last selected points" << std::endl;
    LastPoints.clear();
}

//----------------------------------------------------------------------------------------------------

void Core::RemoveSelectTriangle() {
    if (SelectedTriangle) {
        std::cout << "[SELECTION] Removing selected triangle" << std::endl;
        auto victim = std::find_if(
            std::begin(CurrentTriangles), std::end(CurrentTriangles),
            [&] (std::shared_ptr<GraphicTriangle> & victim) {
                return victim.get() == SelectedTriangle;
            }
        );
        if (victim != std::end(CurrentTriangles)) {
            SelectedTriangle = nullptr;
            CurrentTriangles.erase(victim);
        }
    } else {
        std::cout << "[SELECTION] No selected triangle to remove" << std::endl;
    }
}

//----------------------------------------------------------------------------------------------------

void Core::MoveSelectedTriangle(const Vector2D & point, bool updateTexCoords) {
    if (SelectedTriangle) {
        SelectedTriangle->SetPosition(point);
        if (updateTexCoords) {
            auto & points = SelectedTriangle->GetPoints();
            for (int i = 0, len = points.size(); i < len; ++i) {
                auto & p = points[i];
                SelectedTriangle->SetTextCoord(i, Vector2D(
                    p.vertex.X() / (float)BackgroundTexture->Width(),
                    p.vertex.Y() / (float)BackgroundTexture->Height()
                ));
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------

void Core::SelectTriangle(const Vector2D & point) {
    UnselectTriangle();
    std::for_each(
        std::begin(CurrentTriangles), std::end(CurrentTriangles),
        [&] (std::shared_ptr<GraphicTriangle> & victim) {
            if (!SelectedTriangle && victim->Inside(point)) {
                SelectedTriangle = victim.get();
                SelectedTriangle->SetColor(0.0f, 1.0f, 0.0f);
            }
        }
    );
}

//----------------------------------------------------------------------------------------------------

void Core::UnselectTriangle() {
    if (SelectedTriangle) {
        SelectedTriangle->SetColor(1.0f, 0.0f, 0.0f);
        SelectedTriangle = nullptr;
    }
}

//----------------------------------------------------------------------------------------------------

void Core::AnimationStep() {
    std::for_each(
        std::begin(CurrentTriangles), std::end(CurrentTriangles),
        [&] (std::shared_ptr<GraphicTriangle> & victim) {
            auto & speed = victim->GetSpeed();
            victim->Move(speed);
            auto & position = victim->GetCenter();
            if (position.X() < 0.0f) {
                if (position.Y() < 0.0f) {
                    victim->SetSpeed(-speed);
                } else if (position.Y() >= (GLfloat)BackgroundTexture->Height()) {
                    victim->SetSpeed(-speed);
                } else {
                    victim->SetSpeed(speed.Reflection(Vector2D(1.0f, 0.0f)));
                }
            } else if (position.X() >= (GLfloat)BackgroundTexture->Width()) {
                if (position.Y() < 0.0f) {
                    victim->SetSpeed(-speed);
                } else if (position.Y() >= (GLfloat)BackgroundTexture->Height()) {
                    victim->SetSpeed(-speed);
                } else {
                    victim->SetSpeed(speed.Reflection(Vector2D(-1.0f, 0.0f)));
                }
            } else {
                if (position.Y() < 0.0f) {
                    victim->SetSpeed(speed.Reflection(Vector2D(0.0f, 1.0f)));
                } else if (position.Y() >= (GLfloat)BackgroundTexture->Height()) {
                    victim->SetSpeed(speed.Reflection(Vector2D(0.0f, -1.0f)));
                }
            }
        }
    );
}

//----------------------------------------------------------------------------------------------------

void Core::AnimationStart() {
    if (!IsAnimationRunning) {
        IsAnimationRunning = true;
        glutTimerFunc(STEP_TIME, OnTimer, 0);
    }
}

//----------------------------------------------------------------------------------------------------

void Core::Start(int argc, char ** argv) {
    // Initialize the window:
    glutInit(&argc, argv);
    glutInitWindowPosition(-1, -1);
    glutInitWindowSize(WindowWidth, WindowHeight);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    int window = glutCreateWindow("Computer Graphics: Practice 3");
    
    // Setting the events:
    glutDisplayFunc(OnRender);
    glutReshapeFunc(OnResize);
    glutSpecialFunc(OnSpecial);
    
    // Initialize OpenGL and the scene:
    InitializeResources();
    InitializeRender();
    
    // Run the main loop:
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutMainLoop();
    
    // Finish the execution:
    glutDestroyWindow(window);
}

//****************************************************************************************************
// Main Entry:
//****************************************************************************************************

int main(int argc, char ** argv) {
    Core::Start(argc, argv);
    return 0;
}
