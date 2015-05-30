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
#include <cmath>

//----------------------------------------------------------------------------------------------------
// General
//----------------------------------------------------------------------------------------------------

const GLfloat PI = 3.14159265359f;

inline GLfloat RadToDeg(GLfloat value) { return (value * 180.0f) / PI; }
inline GLfloat DegToRad(GLfloat value) { return (value * PI) / 180.0f; }

//----------------------------------------------------------------------------------------------------
// Vector2D
//----------------------------------------------------------------------------------------------------

class Vector2D {
private:
    GLfloat x_, y_;

public:
    Vector2D(GLfloat x = 0.0f, GLfloat y = 0.0f) : x_(x), y_(y) {}
    Vector2D(const Vector2D & v) : x_(v.x_), y_(v.y_) {}

    // Properties:
    GLfloat X() const { return x_; }
    GLfloat Y() const { return y_; }

    void X(GLfloat value) { x_ = value; }
    void Y(GLfloat value) { y_ = value; }

    void Set(const Vector2D & v) { x_ = v.x_, y_ = v.y_; }
    void Set(GLfloat x, GLfloat y) { x_ = x, y_ = y; }

    void Reset() { x_ = 0.0f, y_ = 0.0f; }

    // Methods:
    GLfloat Length() const{ return std::sqrt(LengthSquared()); }
    GLfloat LengthSquared() const { return x_ * x_ + y_ * y_; }
    GLfloat Distance(const Vector2D & rhs) const { return std::sqrt(DistanceSquared(rhs)); }
    GLfloat DistanceSquared(const Vector2D & rhs) const { return ((*this) - rhs).LengthSquared(); }

    Vector2D Normalized() const {
        GLfloat norm = Length();
        return Vector2D(x_ / norm, y_ / norm);
    }

    void Normalize() {
        GLfloat norm = Length();
        x_ /= norm, y_ /= norm;
    }

    GLfloat Angle(const Vector2D & rhs) const {
        return RadToDeg(std::acos(Dot(rhs) / (Length() * rhs.Length())));
    }

    GLfloat Dot(const Vector2D & rhs) const {
        return x_ * rhs.x_ + y_ * rhs.y_;
    }

    Vector2D Rotate(float angle) const {
        auto vs = std::sin(DegToRad(angle)), vc = std::cos(DegToRad(angle));
        return Vector2D(x_ * vc - y_ * vs, x_ * vs + y_ * vc);
    }

    Vector2D Rotate(float angle, const Vector2D & center) const {
        auto vs = std::sin(DegToRad(angle)), vc = std::cos(DegToRad(angle));
        auto dx = (1.0f - vc) * center.x_ + vs * center.y_;
        auto dy = (1.0f - vc) * center.y_ - vs * center.x_;
        return Vector2D(x_ * vc - y_ * vs + dx, x_ * vs + y_ * vc + dy);
    }

    Vector2D LeftPerpendicular() const {
        return Vector2D(-y_, x_);
    }

    Vector2D RightPerpendicular() const {
        return Vector2D(y_, -x_);
    }

    GLfloat Projection(const Vector2D & vector) const {
        return Dot(vector) / vector.Dot(vector);
    }

    Vector2D Reflection(const Vector2D & normal) const {
        auto a = Projection(normal);
        return (*this) - ((2 * a) * normal);
    }

    // Operators:
    Vector2D & operator =(const Vector2D & rhs) {
        Set(rhs); return *this;
    }

    bool operator ==(const Vector2D & rhs) const {
        return x_ == rhs.x_ && y_ == rhs.y_;
    }

    bool operator !=(const Vector2D & rhs) const {
        return x_ != rhs.x_ || y_ != rhs.y_;
    }

    Vector2D operator -() const {
        return Vector2D(-x_, -y_);
    }

    Vector2D operator +(const Vector2D & rhs) const {
        return Vector2D(x_ + rhs.x_, y_ + rhs.y_);
    }

    Vector2D operator -(const Vector2D & rhs) const {
        return Vector2D(x_ - rhs.x_, y_ - rhs.y_);
    }

    friend Vector2D operator +(const Vector2D & lhs, GLfloat rhs);
    friend Vector2D operator +(GLfloat lhs, const Vector2D & rhs);
    friend Vector2D operator -(const Vector2D & lhs, GLfloat rhs);
    friend Vector2D operator -(GLfloat lhs, const Vector2D & rhs);
    friend Vector2D operator *(const Vector2D & lhs, GLfloat rhs);
    friend Vector2D operator *(GLfloat lhs, const Vector2D & rhs);
    friend Vector2D operator /(const Vector2D & lhs, GLfloat rhs);
    friend Vector2D operator /(GLfloat lhs, const Vector2D & rhs);
};

inline Vector2D operator +(const Vector2D & lhs, GLfloat rhs) {
    return Vector2D(lhs.x_ + rhs, lhs.y_ + rhs);
}

inline Vector2D operator +(GLfloat lhs, const Vector2D & rhs) {
    return Vector2D(rhs.x_ + lhs, rhs.y_ + lhs);
}

inline Vector2D operator -(const Vector2D & lhs, GLfloat rhs) {
    return Vector2D(lhs.x_ - rhs, lhs.y_ - rhs);
}

inline Vector2D operator -(GLfloat lhs, const Vector2D & rhs) {
    return Vector2D(rhs.x_ - lhs, rhs.y_ - lhs);
}

inline Vector2D operator *(const Vector2D & lhs, GLfloat rhs) {
    return Vector2D(lhs.x_ * rhs, lhs.y_ * rhs);
}

inline Vector2D operator *(GLfloat lhs, const Vector2D & rhs) {
    return Vector2D(rhs.x_ * lhs, rhs.y_ * lhs);
}

inline Vector2D operator /(const Vector2D & lhs, GLfloat rhs) {
    return Vector2D(lhs.x_ / rhs, lhs.y_ / rhs);
}

inline Vector2D operator /(GLfloat lhs, const Vector2D & rhs) {
    return Vector2D(rhs.x_ / lhs, rhs.y_ / lhs);
}

#endif
