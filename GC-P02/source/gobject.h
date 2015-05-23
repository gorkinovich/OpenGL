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

#include "gmesh.h"
#include <memory>

//----------------------------------------------------------------------------------------------------
// Object3D
//----------------------------------------------------------------------------------------------------

class Object3D {
protected:
    std::shared_ptr<Matrix> transform_;

public:
    Object3D() : transform_(std::make_shared<Matrix>()) {}
    Object3D(const Object3D & v) : transform_(v.transform_) {}
    virtual ~Object3D() {}

    Matrix & Transform() { return *transform_; }
    void Transform(const Matrix & value) { *transform_ = value; }

    void Transform(Matrix * value) { transform_.reset(value); }
    void Transform(std::shared_ptr<Matrix> & value) { transform_ = value; }

    virtual void Draw() const = 0;
};

//----------------------------------------------------------------------------------------------------
// ObjectContainer3D
//----------------------------------------------------------------------------------------------------

class ObjectContainer3D : public Object3D {
public:
    typedef std::shared_ptr<Object3D> SharedObject3D;
    typedef std::vector<SharedObject3D> VectorObject3D;

protected:
    VectorObject3D childs_;

public:
    ObjectContainer3D() : Object3D(), childs_() {}
    ObjectContainer3D(const ObjectContainer3D & v) : Object3D(v), childs_(v.childs_) {}
    virtual ~ObjectContainer3D() {}

    virtual void Draw() const {
        glPushMatrix();
        if (transform_) {
            transform_->MultiplyInOpenGL();
        }
        std::for_each(std::begin(childs_), std::end(childs_), [] (const SharedObject3D & item) {
            item->Draw();
        });
        glPopMatrix();
    }

    void AddChild(SharedObject3D & victim) {
        if (!IsChild(victim)) {
            childs_.push_back(victim);
        }
    }

    void AddChild(Object3D * victim) {
        if (!IsChild(victim)) {
            childs_.push_back(SharedObject3D(victim));
        }
    }

    bool IsChild(const SharedObject3D & victim) const {
        return std::find(std::begin(childs_), std::end(childs_), victim) != childs_.cend();
    }

    bool IsChild(const Object3D * victim) const {
        auto result = std::find_if(std::begin(childs_), std::end(childs_),
            [&] (const SharedObject3D & item) { return item.get() == victim; });
        return result != childs_.cend();
    }

    SharedObject3D & GetChild(unsigned int index) {
        if (index < childs_.size()) {
            return childs_[index];
        } else {
            throw std::exception("ObjectContainer3D::GetChild -> Invalid index!");
        }
    }

    VectorObject3D::size_type NumberOfChilds() {
        return childs_.size();
    }

    void ClearChilds() {
        childs_.clear();
    }
};

//----------------------------------------------------------------------------------------------------
// MeshObject3D
//----------------------------------------------------------------------------------------------------

class MeshObject3D : public Object3D {
protected:
    std::shared_ptr<IMesh3D> mesh_;

public:
    MeshObject3D() : Object3D(), mesh_() {}
    MeshObject3D(IMesh3D * mesh) : Object3D(), mesh_(std::shared_ptr<IMesh3D>(mesh)) {}
    MeshObject3D(std::shared_ptr<IMesh3D> & mesh) : Object3D(), mesh_(mesh) {}
    MeshObject3D(const MeshObject3D & v) : Object3D(v), mesh_(v.mesh_) {}
    virtual ~MeshObject3D() {}

    IMesh3D * Mesh() { return mesh_.get(); }
    void Mesh(IMesh3D * value) { mesh_.reset(value); }
    void Mesh(std::shared_ptr<IMesh3D> & value) { mesh_ = value; }

    virtual void Draw() const {
        glPushMatrix();
        if (transform_) {
            transform_->MultiplyInOpenGL();
        }
        if (mesh_) {
            mesh_->Draw();
        }
        glPopMatrix();
    }
};

#endif
