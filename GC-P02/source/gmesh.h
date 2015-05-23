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

#ifndef __GMESH_H__
#define __GMESH_H__

#include "gmath.h"
#include <vector>
#include <algorithm>

//----------------------------------------------------------------------------------------------------
// VertexData3D
//----------------------------------------------------------------------------------------------------

class VertexData3D {
private:
    GLuint vidx_, nidx_;

public:
    VertexData3D() : vidx_(0), nidx_(0) {}
    VertexData3D(GLuint vidx, GLuint nidx) : vidx_(vidx), nidx_(nidx) {}
    VertexData3D(const VertexData3D & v) : vidx_(v.vidx_), nidx_(v.nidx_) {}

    GLuint VertexIndex() const { return vidx_; }
    GLuint NormalIndex() const { return nidx_; }
};

//----------------------------------------------------------------------------------------------------
// Face3D
//----------------------------------------------------------------------------------------------------

class Face3D {
private:
    std::vector<VertexData3D> data_;

public:
    Face3D() : data_() {}
    Face3D(const std::vector<VertexData3D> & data) : data_(data) {}
    Face3D(const Face3D & v) : data_(v.data_) {}

    const std::vector<VertexData3D> & Data() const { return data_; }

    inline const std::vector<VertexData3D>::const_iterator Begin() const { return data_.cbegin(); }
    inline const std::vector<VertexData3D>::const_iterator End() const { return data_.cend(); }
};

//----------------------------------------------------------------------------------------------------
// IMesh3D
//----------------------------------------------------------------------------------------------------

class IMesh3D {
public:
    IMesh3D() {}
    virtual ~IMesh3D() {}
    virtual void Draw() const = 0;
};

//----------------------------------------------------------------------------------------------------
// Mesh3D
//----------------------------------------------------------------------------------------------------

class Mesh3D : public IMesh3D {
private:
    std::vector<Vector3D> vertex_;
    std::vector<Vector3D> normal_;
    std::vector<Face3D> face_;
    GLfloat red_, green_, blue_;

public:
    Mesh3D() : vertex_(), normal_(), face_(), red_(1.0f), green_(1.0f), blue_(1.0f) {}
    Mesh3D(const std::vector<Vector3D> & vertex, const std::vector<Vector3D> & normal,
        const std::vector<Face3D> & face) : vertex_(vertex), normal_(normal), face_(face),
        red_(1.0f), green_(1.0f), blue_(1.0f) {}
    Mesh3D(const Mesh3D & v) : vertex_(v.vertex_), normal_(v.normal_), face_(v.face_),
        red_(v.red_), green_(v.green_), blue_(v.blue_) {}
    virtual ~Mesh3D() {}

    void SetColor(GLfloat r, GLfloat g, GLfloat b) {
        red_ = r, green_ = g, blue_ = b;
    }

    virtual void Draw() const {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLineWidth(1.0f);
        glColor3f(red_, green_, blue_);
        auto nsize = normal_.size();
        auto vsize = vertex_.size();
        std::for_each(std::begin(face_), std::end(face_), [&] (const Face3D & face) {
            glBegin(GL_POLYGON);
            std::for_each(face.Begin(), face.End(), [&] (const VertexData3D & data) {
                auto nidx = data.NormalIndex();
                if (nidx < nsize) {
                    auto & n = normal_[nidx];
                    glNormal3f(n.X(), n.Y(), n.Z());
                }
                auto vidx = data.VertexIndex();
                if (vidx < vsize) {
                    auto & v = vertex_[vidx];
                    glVertex3f(v.X(), v.Y(), v.Z());
                }
            });
            glEnd();
        });
        glPopMatrix();
    }
};

//----------------------------------------------------------------------------------------------------
// FaceWithNormal3D
//----------------------------------------------------------------------------------------------------

class FaceWithNormal3D {
private:
    std::vector<GLuint> vertex_;
    Vector3D normal_;

public:
    FaceWithNormal3D() : vertex_(), normal_() {}
    FaceWithNormal3D(const std::vector<GLuint> & vertex) : vertex_(vertex), normal_() {}
    FaceWithNormal3D(const std::vector<GLuint> & vertex, const Vector3D & normal) :
        vertex_(vertex), normal_(normal) {}
    FaceWithNormal3D(const FaceWithNormal3D & v) : vertex_(v.vertex_), normal_(v.normal_) {}

    const std::vector<GLuint> & Vertex() const { return vertex_; }
    const Vector3D & Normal() const { return normal_; }

    inline const std::vector<GLuint>::const_iterator Begin() const { return vertex_.cbegin(); }
    inline const std::vector<GLuint>::const_iterator End() const { return vertex_.cend(); }

    void CalculateNormalByNewell(const std::vector<Vector3D> & vertex) {
        normal_.Reset();
        for (unsigned int i = 0, len = vertex_.size(); i < len; ++i) {
            auto & vcur = vertex[vertex_[i]];
            auto & vnxt = vertex[vertex_[(i + 1) % len]];
            normal_.X(normal_.X() + (vcur.Y() - vnxt.Y()) * (vcur.Z() + vnxt.Z()));
            normal_.Y(normal_.Y() + (vcur.Z() - vnxt.Z()) * (vcur.X() + vnxt.X()));
            normal_.Z(normal_.Z() + (vcur.X() - vnxt.X()) * (vcur.Y() + vnxt.Y()));
        }
        normal_.Normalize();
    }
};

//----------------------------------------------------------------------------------------------------
// SimpleMesh3D
//----------------------------------------------------------------------------------------------------

class SimpleMesh3D : public IMesh3D  {
private:
    std::vector<Vector3D> vertex_;
    std::vector<FaceWithNormal3D> face_;
    GLfloat red_, green_, blue_;

public:
    SimpleMesh3D() : vertex_(), face_(), red_(1.0f), green_(1.0f), blue_(1.0f) {}
    SimpleMesh3D(const std::vector<Vector3D> & vertex, const std::vector<FaceWithNormal3D> & face) :
        vertex_(vertex), face_(face), red_(1.0f), green_(1.0f), blue_(1.0f) {}
    SimpleMesh3D(const SimpleMesh3D & v) : vertex_(v.vertex_), face_(v.face_), red_(v.red_),
        green_(v.green_), blue_(v.blue_) {}
    virtual ~SimpleMesh3D() {}

    void CalculateNormals() {
        std::for_each(std::begin(face_), std::end(face_), [&] (FaceWithNormal3D & face) {
            face.CalculateNormalByNewell(vertex_);
        });
    }

    void SetColor(GLfloat r, GLfloat g, GLfloat b) {
        red_ = r, green_ = g, blue_ = b;
    }

    virtual void Draw() const {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLineWidth(1.0f);
        glColor3f(red_, green_, blue_);
        auto vsize = vertex_.size();
        std::for_each(std::begin(face_), std::end(face_), [&] (const FaceWithNormal3D & face) {
            glBegin(GL_POLYGON);
            auto & n = face.Normal();
            glNormal3f(n.X(), n.Y(), n.Z());
            std::for_each(face.Begin(), face.End(), [&] (GLuint vertexIndex) {
                if (vertexIndex < vsize) {
                    auto & v = vertex_[vertexIndex];
                    glVertex3f(v.X(), v.Y(), v.Z());
                }
            });
            glEnd();
        });
        glPopMatrix();
    }
};

#endif
