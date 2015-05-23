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

#ifndef __GMATH_H__
#define __GMATH_H__

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/GL.h>
#include <cstring>
#include <cmath>

class Matrix;

//----------------------------------------------------------------------------------------------------
// General
//----------------------------------------------------------------------------------------------------

const GLfloat PI = 3.14159265359f;
const GLfloat W_AS_VECTOR = 0.0f;
const GLfloat W_AS_POINT = 1.0f;
const GLfloat W_THRESHOLD = 0.01f;

inline GLfloat RadToDeg(GLfloat value) { return (value * 180.0f) / PI; }
inline GLfloat DegToRad(GLfloat value) { return (value * PI) / 180.0f; }

//----------------------------------------------------------------------------------------------------
// Vector3D
//----------------------------------------------------------------------------------------------------

class Vector3D {
private:
    GLfloat x_, y_, z_, w_;

public:
    Vector3D() : x_(0.0f), y_(0.0f), z_(0.0f), w_(W_AS_VECTOR) {}
    Vector3D(GLfloat x, GLfloat y, GLfloat z, GLfloat w) : x_(x), y_(y), z_(z), w_(w) {}
    Vector3D(GLfloat x, GLfloat y, GLfloat z, bool point = false) :
        x_(x), y_(y), z_(z), w_(point ? W_AS_POINT : W_AS_VECTOR) {}
    Vector3D(const Vector3D & v) : x_(v.x_), y_(v.y_), z_(v.z_), w_(v.w_) {}

    // Properties:
    GLfloat X() const { return x_; }
    GLfloat Y() const { return y_; }
    GLfloat Z() const { return z_; }
    GLfloat W() const { return w_; }

    void X(GLfloat value) { x_ = value; }
    void Y(GLfloat value) { y_ = value; }
    void Z(GLfloat value) { z_ = value; }
    void W(GLfloat value) { w_ = value; }

    bool IsVector() const { return (w_ - W_AS_VECTOR) <= W_THRESHOLD; }
    bool IsPoint() const { return (w_ - W_AS_POINT) <= W_THRESHOLD; }

    void Set(const Vector3D & v) { x_ = v.x_, y_ = v.y_, z_ = v.z_, w_ = v.w_; }
    void Set(GLfloat x, GLfloat y, GLfloat z) { x_ = x, y_ = y, z_ = z; }
    void Set(GLfloat x, GLfloat y, GLfloat z, GLfloat w) { x_ = x, y_ = y, z_ = z, w_ = w; }

    void Reset() { x_ = 0.0f, y_ = 0.0f, z_ = 0.0f, w_ = W_AS_VECTOR; }

    // Methods:
    GLfloat Length() const{ return std::sqrt(LengthSquared()); }
    GLfloat LengthSquared() const { return x_ * x_ + y_ * y_ + z_ * z_; }
    GLfloat Distance(const Vector3D & rhs) const { return std::sqrt(DistanceSquared(rhs)); }
    GLfloat DistanceSquared(const Vector3D & rhs) const { return ((*this) - rhs).LengthSquared(); }

    Vector3D Normalized() const {
        GLfloat norm = Length();
        return Vector3D(x_ / norm, y_ / norm, z_ / norm, W_AS_VECTOR);
    }

    void Normalize() {
        GLfloat norm = Length();
        x_ /= norm, y_ /= norm, z_ /= norm;
    }

    GLfloat Angle(const Vector3D & rhs) const {
        return RadToDeg(std::acos(Dot(rhs) / (Length() * rhs.Length())));
    }

    GLfloat Dot(const Vector3D & rhs) const {
        return x_ * rhs.x_ + y_ * rhs.y_ + z_ * rhs.z_;
    }

    Vector3D Cross(const Vector3D & rhs) const {
        return Vector3D(
            y_ * rhs.z_ - z_ * rhs.y_, z_ * rhs.x_ - x_ * rhs.z_,
            x_ * rhs.y_ - y_ * rhs.x_, W_AS_VECTOR
        );
    }

    // Operators:
    Vector3D & operator =(const Vector3D & rhs) {
        Set(rhs); return *this;
    }

    bool operator ==(const Vector3D & rhs) const {
        return x_ == rhs.x_ && y_ == rhs.y_ && z_ == rhs.z_ && w_ == rhs.w_;
    }

    bool operator !=(const Vector3D & rhs) const {
        return x_ != rhs.x_ || y_ != rhs.y_ || z_ != rhs.z_ || w_ != rhs.w_;
    }

    Vector3D operator -() const {
        return Vector3D(-x_, -y_, -z_, w_);
    }

    Vector3D operator +(const Vector3D & rhs) const {
        return Vector3D(x_ + rhs.x_, y_ + rhs.y_, z_ + rhs.z_, w_ >= rhs.w_ ? w_ : rhs.w_);
    }

    Vector3D operator -(const Vector3D & rhs) const {
        return Vector3D(x_ - rhs.x_, y_ - rhs.y_, z_ - rhs.z_, w_ >= rhs.w_ ? w_ : rhs.w_);
    }

    Vector3D operator *(const Vector3D & rhs) const {
        return Vector3D(x_ * rhs.x_, y_ * rhs.y_, z_ * rhs.z_, w_ >= rhs.w_ ? w_ : rhs.w_);
    }

    Vector3D operator /(const Vector3D & rhs) const {
        return Vector3D(x_ / rhs.x_, y_ / rhs.y_, z_ / rhs.z_, w_ >= rhs.w_ ? w_ : rhs.w_);
    }

    friend Vector3D operator *(const Vector3D & lhs, GLfloat rhs);
    friend Vector3D operator *(GLfloat lhs, const Vector3D & rhs);
    friend Vector3D operator /(const Vector3D & lhs, GLfloat rhs);
    friend Vector3D operator /(GLfloat lhs, const Vector3D & rhs);

    friend Matrix;
};

Vector3D operator *(const Vector3D & lhs, GLfloat rhs) {
    return Vector3D(lhs.x_ * rhs, lhs.y_ * rhs, lhs.z_ * rhs, lhs.w_);
}

Vector3D operator *(GLfloat lhs, const Vector3D & rhs) {
    return Vector3D(rhs.x_ * lhs, rhs.y_ * lhs, rhs.z_ * lhs, rhs.w_);
}

Vector3D operator /(const Vector3D & lhs, GLfloat rhs) {
    return Vector3D(lhs.x_ / rhs, lhs.y_ / rhs, lhs.z_ / rhs, lhs.w_);
}

Vector3D operator /(GLfloat lhs, const Vector3D & rhs) {
    return Vector3D(rhs.x_ / lhs, rhs.y_ / lhs, rhs.z_ / lhs, rhs.w_);
}

//----------------------------------------------------------------------------------------------------
// Matrix
//----------------------------------------------------------------------------------------------------

class Matrix {
private:
    static const int LEN4 = 4;
    static const int LEN = 16;
    GLfloat data_[LEN];

    inline int getIndex(int row, int col) const {
        return col * LEN4 + row;
    }

    inline GLfloat get(int i, int j) const {
        return data_[getIndex(i, j)];
    }

    inline void set(int i, int j, GLfloat v) {
        data_[getIndex(i, j)] = v;
    }

public:
    Matrix() {
        SetAsIdentity();
    }

    Matrix(const Matrix & v) {
        std::memcpy(data_, v.data_, LEN * sizeof(GLfloat));
    }

    // Properties:
    const GLfloat * Data() const { return data_; }

    // OpenGL Methods:
    void LoadFromOpenGL() {
        glGetFloatv(GL_MODELVIEW_MATRIX, data_);
    }

    void StoreInOpenGL() const {
        glLoadMatrixf(data_);
    }

    void MultiplyInOpenGL() const {
        glMultMatrixf(data_);
    }

    void LoadIdentity() {
        glPushMatrix();
        glLoadIdentity();
        LoadFromOpenGL();
        glPopMatrix();
    }

    void AddTranslate(GLfloat x, GLfloat y, GLfloat z) {
        glPushMatrix();
        StoreInOpenGL();
        glTranslatef(x, y, z);
        LoadFromOpenGL();
        glPopMatrix();
    }

    void AddScale(GLfloat x, GLfloat y, GLfloat z) {
        glPushMatrix();
        StoreInOpenGL();
        glScalef(x, y, z);
        LoadFromOpenGL();
        glPopMatrix();
    }

    void AddRotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
        glPushMatrix();
        StoreInOpenGL();
        glRotatef(angle, x, y, z);
        LoadFromOpenGL();
        glPopMatrix();
    }

    void AddMultiply(const Matrix & rhs) {
        glPushMatrix();
        StoreInOpenGL();
        rhs.MultiplyInOpenGL();
        LoadFromOpenGL();
        glPopMatrix();
    }

    Matrix Multiply(const Matrix & rhs) {
        Matrix result;
        glPushMatrix();
        StoreInOpenGL();
        rhs.MultiplyInOpenGL();
        result.LoadFromOpenGL();
        glPopMatrix();
        return result;
    }

    inline void AddTranslate(const Vector3D & v) {
        AddTranslate(v.X(), v.Y(), v.Z());
    }

    inline void AddScale(const Vector3D & v) {
        AddScale(v.X(), v.Y(), v.Z());
    }

    inline void AddRotate(GLfloat angle, const Vector3D & v) {
        AddRotate(angle, v.X(), v.Y(), v.Z());
    }

    // Methods:
    void SetAsIdentity() {
        for (int i = 0, k = 0; i < LEN4; ++i) {
            for (int j = 0; j < LEN4; ++j, ++k) {
                data_[k] = i == j ? 1.0f : 0.0f;
            }
        }
    }

    void SetAsTranslate(GLfloat x, GLfloat y, GLfloat z) {
        SetAsIdentity();
        set(0, 3, x);
        set(1, 3, y);
        set(2, 3, z);
    }

    void SetAsScale(GLfloat x, GLfloat y, GLfloat z) {
        SetAsIdentity();
        set(0, 0, x);
        set(1, 1, y);
        set(2, 2, z);
    }

    void SetAsRotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
        GLfloat c = std::cosf(angle);
        GLfloat s = std::sinf(angle);
        GLfloat t = 1.0f - std::cosf(angle);

        set(0, 0, t * x * x + c);
        set(0, 1, t * x * y + s * z);
        set(0, 2, t * x * z - s * y);
        set(0, 3, 0.0f);

        set(1, 0, t * x * y - s * z);
        set(1, 1, t * y * y + c);
        set(1, 2, t * y * z + s * x);
        set(1, 3, 0.0f);

        set(2, 0, t * x * y + s * y);
        set(2, 1, t * y * z - s * x);
        set(2, 2, t * z * z + c);
        set(2, 3, 0.0f);

        set(3, 0, 0.0f);
        set(3, 1, 0.0f);
        set(3, 2, 0.0f);
        set(3, 3, 1.0f);
    }

    void SetAsRotateX(GLfloat angle) {
        SetAsIdentity();
        set(1, 1, std::cosf(angle));
        set(1, 2, -std::sinf(angle));
        set(2, 1, std::sinf(angle));
        set(2, 2, std::cosf(angle));
    }

    void SetAsRotateY(GLfloat angle) {
        SetAsIdentity();
        set(0, 0, std::cosf(angle));
        set(0, 2, std::sinf(angle));
        set(2, 0, -std::sinf(angle));
        set(2, 2, std::cosf(angle));
    }

    void SetAsRotateZ(GLfloat angle) {
        SetAsIdentity();
        set(0, 0, std::cosf(angle));
        set(0, 1, -std::sinf(angle));
        set(1, 0, std::sinf(angle));
        set(1, 1, std::cosf(angle));
    }

    inline void SetAsTranslate(const Vector3D & v) {
        SetAsTranslate(v.X(), v.Y(), v.Z());
    }

    inline void SetAsScale(const Vector3D & v) {
        SetAsScale(v.X(), v.Y(), v.Z());
    }

    inline void SetAsRotate(GLfloat angle, const Vector3D & v) {
        SetAsRotate(angle, v.X(), v.Y(), v.Z());
    }

    // Operators:
    Matrix & operator =(const Matrix & rhs) {
        std::memcpy(data_, rhs.data_, LEN * sizeof(GLfloat));
        return *this;
    }

    bool operator ==(const Matrix & rhs) const {
        for (int i = 0; i < LEN; ++i) {
            if (data_[i] != rhs.data_[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator !=(const Matrix & rhs) const {
        for (int i = 0; i < LEN; ++i) {
            if (data_[i] != rhs.data_[i]) {
                return true;
            }
        }
        return false;
    }

    Matrix operator *(const Matrix & rhs) const {
        Matrix result;
        for (int i = 0; i < LEN4; ++i) {
            for (int j = 0; j < LEN4; ++j) {
                // Row x Column:
                GLfloat v = 0.0f;
                for (int k = 0; k < LEN4; ++k) {
                    v += get(i, k) * rhs.get(k, j);
                }
                result.set(i, j, v);
            }
        }
        return result;
    }

    Vector3D operator *(const Vector3D & rhs) const {
        return Vector3D(
            get(0, 0) * rhs.x_ + get(0, 1) * rhs.y_ + get(0, 2) * rhs.z_ + get(0, 3) * rhs.w_,
            get(1, 0) * rhs.x_ + get(1, 1) * rhs.y_ + get(1, 2) * rhs.z_ + get(1, 3) * rhs.w_,
            get(2, 0) * rhs.x_ + get(2, 1) * rhs.y_ + get(2, 2) * rhs.z_ + get(2, 3) * rhs.w_,
            get(3, 0) * rhs.x_ + get(3, 1) * rhs.y_ + get(3, 2) * rhs.z_ + get(3, 3) * rhs.w_
        );
    }
};

#endif
