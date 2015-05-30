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
// Event functions:
//****************************************************************************************************

void Core::OnRender() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        if (ShowBackground || CurrentState == DESIGN_STATE) {
            BackgroundRectangle->Draw(*BackgroundTexture);
        }
        if (CurrentState == DESIGN_STATE) {
            RenderWiredTriangles();
            RenderPoints();
        } else if (CurrentState == SELECTION_STATE) {
            RenderTrianglesWithBorder();
        } else if (CurrentState == ANIMATION_STATE) {
            RenderTriangles();
        }
    glPopMatrix();
    glutSwapBuffers();
}

//----------------------------------------------------------------------------------------------------

void Core::OnResize(int width, int height) {
    WindowWidth = width, WindowHeight = height;
    glViewport(0, 0, WindowWidth, WindowHeight);
    InitializeProjection();
    glutPostRedisplay();
}

//----------------------------------------------------------------------------------------------------

void Core::OnMouseDesignState(int button, int state, int x, int y) {
    if (state == GLUT_UP) {
        if (button == GLUT_LEFT_BUTTON) {
            DesignAddPoint(x, y);
        } else if (button == GLUT_RIGHT_BUTTON) {
            DesignRemovePoints();
        }
        glutPostRedisplay();
    }
}

//----------------------------------------------------------------------------------------------------

void Core::OnMouseSelectionState(int button, int state, int x, int y) {
    if (state == GLUT_UP) {
        Vector2D point = WindowToProjection(x, y);
        if (button == GLUT_LEFT_BUTTON) {
            SelectTriangle(point);
        } else if (button == GLUT_MIDDLE_BUTTON) {
            MoveSelectedTriangle(point, true);
        } else if (button == GLUT_RIGHT_BUTTON) {
            MoveSelectedTriangle(point, false);
        }
        glutPostRedisplay();
    }
}

//----------------------------------------------------------------------------------------------------

void Core::OnMouseAnimationState(int button, int state, int x, int y) {
}

//----------------------------------------------------------------------------------------------------

void Core::OnKeyboard(unsigned char ascii, int mouseX, int mouseY) {
    bool repaint = true;
    switch (ascii) {
    case 27:
        glutLeaveMainLoop();
        break;
    case 'd':
    case 'D':
        GotoDesignState();
        break;
    case 's':
    case 'S':
        GotoSelectionState();
        break;
    case 'a':
    case 'A':
        GotoAnimationState();
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

void Core::OnKeyboardDesignState(unsigned char ascii, int mouseX, int mouseY) {
    switch (ascii) {
    case 'h':
    case 'H':
        std::cout << "[HELP] Design mode keys" << std::endl;
        std::cout << " d = Design mode" << std::endl;
        std::cout << " s = Selection mode" << std::endl;
        std::cout << " a = Animation mode" << std::endl;
        std::cout << " c = Remove last selected points" << std::endl;
        std::cout << " u = Remove last selected point" << std::endl;
        std::cout << " k = Remove current triangles" << std::endl;
        std::cout << " r = Reset application" << std::endl;
        break;
    case 'd':
    case 'D':
        std::cout << "[DESIGN] You're already in the design state" << std::endl;
        break;
    case 'c':
    case 'C':
        std::cout << "[DESIGN] Removing last selected points" << std::endl;
        LastPoints.clear();
        break;
    case 'u':
    case 'U':
        std::cout << "[DESIGN] Removing last selected point" << std::endl;
        LastPoints.pop_back();
        break;
    case 'k':
    case 'K':
        std::cout << "[DESIGN] Removing current triangles" << std::endl;
        CurrentTriangles.clear();
        break;
    case '1':
        DesignAddPoint(mouseX, mouseY);
        break;
    case '2':
        break;
    case '3':
        DesignRemovePoints();
        break;
    default:
        OnKeyboard(ascii, mouseX, mouseY);
        return;
    }
    glutPostRedisplay();
}

//----------------------------------------------------------------------------------------------------

void Core::OnKeyboardSelectionState(unsigned char ascii, int mouseX, int mouseY) {
    switch (ascii) {
    case 'h':
    case 'H':
        std::cout << "[HELP] Selection mode keys" << std::endl;
        std::cout << " d = Design mode" << std::endl;
        std::cout << " s = Selection mode" << std::endl;
        std::cout << " a = Animation mode" << std::endl;
        std::cout << " b = Show/hide background" << std::endl;
        std::cout << " r = Reset application" << std::endl;
        break;
    case 's':
    case 'S':
        std::cout << "[SELECTION] You're already in the selection state" << std::endl;
        break;
    case 'e':
    case 'E':
        RemoveSelectTriangle();
        break;
    case 'b':
    case 'B':
        if (ShowBackground) {
            std::cout << "[SELECTION] Hide background" << std::endl;
        } else {
            std::cout << "[SELECTION] Show background" << std::endl;
        }
        ShowBackground = !ShowBackground;
        break;
    case '1':
        SelectTriangle(WindowToProjection(mouseX, mouseY));
        break;
    case '2':
        MoveSelectedTriangle(WindowToProjection(mouseX, mouseY), true);
        break;
    case '3':
        MoveSelectedTriangle(WindowToProjection(mouseX, mouseY), false);
        break;
    default:
        OnKeyboard(ascii, mouseX, mouseY);
        return;
    }
    glutPostRedisplay();
}

//----------------------------------------------------------------------------------------------------

void Core::OnKeyboardAnimationState(unsigned char ascii, int mouseX, int mouseY) {
    switch (ascii) {
    case 'h':
    case 'H':
        std::cout << "[HELP] Animation mode keys" << std::endl;
        std::cout << " d = Design mode" << std::endl;
        std::cout << " s = Selection mode" << std::endl;
        std::cout << " a = Animation mode" << std::endl;
        std::cout << " m = Start/Stop animation" << std::endl;
        std::cout << " p = Step-by-step animation" << std::endl;
        std::cout << " b = Show/hide background" << std::endl;
        std::cout << " r = Reset application" << std::endl;
        break;
    case 'a':
    case 'A':
        std::cout << "[ANIMATION] You're already in the animation state" << std::endl;
        break;
    case 'm':
    case 'M':
        if (IsAnimationRunning) {
            std::cout << "[ANIMATION] Stop animation" << std::endl;
            IsAnimationRunning = false;
        } else {
            std::cout << "[ANIMATION] Start animation" << std::endl;
            AnimationStart();
        }
        break;
    case 'p':
    case 'P':
        std::cout << "[ANIMATION] Animation step" << std::endl;
        AnimationStep();
        break;
    case 'b':
    case 'B':
        if (ShowBackground) {
            std::cout << "[ANIMATION] Hide background" << std::endl;
        } else {
            std::cout << "[ANIMATION] Show background" << std::endl;
        }
        ShowBackground = !ShowBackground;
        break;
    default:
        OnKeyboard(ascii, mouseX, mouseY);
        return;
    }
    glutPostRedisplay();
}

//----------------------------------------------------------------------------------------------------

void Core::OnSpecial(int keyCode, int mouseX, int mouseY) {
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

//----------------------------------------------------------------------------------------------------

void Core::OnTimer(int value) {
    if (IsAnimationRunning) {
        AnimationStep();
        glutPostRedisplay();
        glutTimerFunc(STEP_TIME, OnTimer, 0);
    }
}
