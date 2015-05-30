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

#ifndef __GOBJECT_H__
#define __GOBJECT_H__

#include "gtexture.hpp"
#include "gmath.hpp"
#include <algorithm>
#include <vector>

//----------------------------------------------------------------------------------------------------
// PointInfo
//----------------------------------------------------------------------------------------------------

struct PointInfo {
    Vector2D texCoord;
    Vector2D vertex;
    PointInfo() : texCoord(), vertex() {}
    PointInfo(const Vector2D & tcd, const Vector2D & vtx) : texCoord(tcd), vertex(vtx) {}
};

//----------------------------------------------------------------------------------------------------
// GraphicFigure
//----------------------------------------------------------------------------------------------------

class GraphicFigure {
public:
    typedef std::vector<PointInfo> VectorOfPoints;

protected:
    Vector2D center_;
    VectorOfPoints points_;
    GLfloat red_, green_, blue_;

private:
    inline void draw(GLenum mode) {
        glBegin(mode);
        std::for_each(
            std::begin(points_), std::end(points_),
            [] (const PointInfo & p) {
                glTexCoord2f(p.texCoord.X(), p.texCoord.Y());
                glVertex2f(p.vertex.X(), p.vertex.Y());
            }
        );
        glEnd();
    }

    inline void drawBorder() {
        // Render the figure:
        glLineWidth(2.0f);
        draw(GL_LINE_LOOP);
        glLineWidth(1.0f);
    }

    inline void drawCenter() {
        // Render the center:
        glPointSize(2.0f);
        glBegin(GL_POINTS);
            glVertex2f(center_.X(), center_.Y());
        glEnd();
        glPointSize(1.0f);
    }

public:
    GraphicFigure() : center_(), points_(), red_(1.0f), green_(1.0f), blue_(1.0f) {}
    virtual ~GraphicFigure() {}

    inline const Vector2D & GetCenter() const { return center_; }
    inline const VectorOfPoints & GetPoints() const { return points_; }
    inline GLfloat GetRed() const { return red_; }
    inline GLfloat GetGreen() const { return green_; }
    inline GLfloat GetBlue() const { return blue_; }

    void SetColor(GLfloat r, GLfloat g, GLfloat b) {
        red_ = r, green_ = g, blue_ = b;
    }

    void SetTextCoord(unsigned int index, const Vector2D & value) {
        if (index < points_.size()) {
            points_[index].texCoord = value;
        }
    }

    void SetPosition(const Vector2D & center) {
        Move(center - center_);
    }

    void Move(const Vector2D & offset) {
        center_ = center_ + offset;
        std::for_each(std::begin(points_), std::end(points_),
            [&] (PointInfo & p) { p.vertex = p.vertex + offset; });
    }

    void Draw() {
        glColor3f(red_, green_, blue_);
        draw(GL_POLYGON);
    }

    void Draw(const Texture & texture) {
        glColor3f(1.0f, 1.0f, 1.0f);
        texture.Bind();
        draw(GL_POLYGON);
        texture.Unbind();
    }

    void DrawWithBorder(const Texture & texture) {
        Draw(texture);
        glColor3f(red_, green_, blue_);
        drawBorder();
    }

    void WireDraw() {
        glColor3f(red_, green_, blue_);
        drawBorder();
        drawCenter();
    }

    virtual bool Inside(GLfloat x, GLfloat y) = 0;
    virtual bool Inside(const Vector2D & point) = 0;
};

//----------------------------------------------------------------------------------------------------
// GraphicRectangle
//----------------------------------------------------------------------------------------------------

class GraphicRectangle : public GraphicFigure {
protected:
    void initialize(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
        points_.push_back(PointInfo(Vector2D(0.0f, 0.0f), Vector2D(x1, y1))); // Bottom-left
        points_.push_back(PointInfo(Vector2D(1.0f, 0.0f), Vector2D(x2, y1))); // Bottom-right
        points_.push_back(PointInfo(Vector2D(1.0f, 1.0f), Vector2D(x2, y2))); // Up-right
        points_.push_back(PointInfo(Vector2D(0.0f, 1.0f), Vector2D(x1, y2))); // Up-left

        center_.X((x1 + x2) / 2.0f);
        center_.Y((y1 + y2) / 2.0f);
    }

public:
    GraphicRectangle(GLfloat left, GLfloat bottom, GLfloat right, GLfloat top) {
        initialize(left, bottom, right, top);
    }

    GraphicRectangle(const Vector2D & lb, const Vector2D & rt) {
        initialize(lb.X(), lb.Y(), rt.X(), rt.Y());
    }

    virtual ~GraphicRectangle() {}

    inline GLfloat GetLeft()   const { return points_[0].vertex.X(); }
    inline GLfloat GetBottom() const { return points_[0].vertex.Y(); }
    inline GLfloat GetRight()  const { return points_[2].vertex.X(); }
    inline GLfloat GetTop()    const { return points_[2].vertex.Y(); }

    virtual bool Inside(GLfloat x, GLfloat y) {
        return GetLeft()   <= x && x <= GetRight() &&
               GetBottom() <= y && y <= GetTop();
    }

    virtual bool Inside(const Vector2D & point) {
        return Inside(point.X(), point.Y());
    }
};

//----------------------------------------------------------------------------------------------------
// GraphicTriangle
//----------------------------------------------------------------------------------------------------

class GraphicTriangle : public GraphicFigure {
protected:
    Vector2D speed_, vab_, vac_, vabp_, vacp_;

public:
    GraphicTriangle(const Vector2D & p1, const Vector2D & p2, const Vector2D & p3,
        const Vector2D & t1, const Vector2D & t2, const Vector2D & t3) {
        points_.push_back(PointInfo(t1, p1));
        points_.push_back(PointInfo(t2, p2));
        points_.push_back(PointInfo(t3, p3));

        const GLfloat OneThird = 1.0f / 3.0f;
        center_ = (p1 * OneThird) + (p2 * OneThird) + (p3 * OneThird);
        vab_ = points_[1].vertex - points_[0].vertex;
        vac_ = points_[2].vertex - points_[0].vertex;
        vabp_ = vab_.LeftPerpendicular();
        vacp_ = vac_.RightPerpendicular();
    }

    inline const Vector2D & GetSpeed() const { return speed_; }
    inline void SetSpeed(const Vector2D & value) { speed_ = value; }

    virtual ~GraphicTriangle() {}

    virtual bool Inside(GLfloat x, GLfloat y) {
        return Inside(Vector2D(x, y));
    }

    virtual bool Inside(const Vector2D & point) {
        auto vap = point - points_[0].vertex;
        GLfloat dab = vap.Dot(vacp_) / vab_.Dot(vacp_);
        GLfloat dac = vap.Dot(vabp_) / vac_.Dot(vabp_);
        return dab >= 0.0f && dac >= 0.0f && (dab + dac) <= 1.0f;
    }
};

#endif
