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

#ifndef __GSYSTEM_H__
#define __GSYSTEM_H__

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/GL.h>
#include <vector>
#include <memory>

//****************************************************************************************************
//*************************************** General structures *****************************************
//****************************************************************************************************

const int ARRAY4_LENGTH = 4;
typedef GLfloat GLfloat4[ARRAY4_LENGTH];

//****************************************************************************************************
//***************************** Mathematical operations and structures *******************************
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
// General
//----------------------------------------------------------------------------------------------------

const GLfloat PI = 3.14159265359f;
inline GLfloat RadToDeg(GLfloat value) { return (value * 180.0f) / PI; }
inline GLfloat DegToRad(GLfloat value) { return (value * PI) / 180.0f; }

class Point3;
class Vector3;
class Quaternion;

//----------------------------------------------------------------------------------------------------
// Coordinates
//----------------------------------------------------------------------------------------------------

class Coordinates {
public:
    static const int X_IDX, Y_IDX, Z_IDX, W_IDX;
    static const GLfloat W_AS_VECTOR, W_AS_POINT;

protected:
    GLfloat4 data_;

    virtual void validateChanges() {}

public:
    Coordinates(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    Coordinates(const GLfloat * data);
    Coordinates(const Coordinates & victim);

    inline GLfloat X() const { return data_[X_IDX]; }
    inline GLfloat Y() const { return data_[Y_IDX]; }
    inline GLfloat Z() const { return data_[Z_IDX]; }
    inline GLfloat W() const { return data_[W_IDX]; }

    inline void X(GLfloat value) { data_[X_IDX] = value; validateChanges(); }
    inline void Y(GLfloat value) { data_[Y_IDX] = value; validateChanges(); }
    inline void Z(GLfloat value) { data_[Z_IDX] = value; validateChanges(); }
    inline void W(GLfloat value) { data_[W_IDX] = value; validateChanges(); }

    const GLfloat * Get() const;
    void Set(const Coordinates & value);
    void Set(const GLfloat * data);
    void Set(GLfloat x, GLfloat y, GLfloat z);
    void Set(GLfloat x, GLfloat y, GLfloat z, GLfloat w);

    Coordinates & operator =(const Coordinates & rhs);
    bool operator ==(const Coordinates & rhs) const;
    bool operator !=(const Coordinates & rhs) const;
};

//----------------------------------------------------------------------------------------------------
// Point3
//----------------------------------------------------------------------------------------------------

class Point3 : public Coordinates {
protected:
    virtual void validateChanges();

public:
    static const Point3 ZERO;

    Point3(GLfloat x = 0.0f, GLfloat y = 0.0f, GLfloat z = 0.0f);
    Point3(const GLfloat * data);
    Point3(const Point3 & victim);
    Point3(const Vector3 & victim);

    Point3 operator +(const Vector3 & rhs);
    Point3 operator -(const Vector3 & rhs);
    Point3 operator *(const Vector3 & rhs);
    Point3 operator /(const Vector3 & rhs);

    friend class Vector3;
};

//----------------------------------------------------------------------------------------------------
// Vector3
//----------------------------------------------------------------------------------------------------

class Vector3 : public Coordinates {
protected:
    virtual void validateChanges();

public:
    static const Vector3 ZERO;
    static const Vector3 IDENTITY;
    static const Vector3 UNIT_X;
    static const Vector3 UNIT_Y;
    static const Vector3 UNIT_Z;
    static const Vector3 LEFT;
    static const Vector3 RIGHT;
    static const Vector3 UP;
    static const Vector3 DOWN;
    static const Vector3 FORWARD;
    static const Vector3 BACKWARD;

    Vector3(GLfloat x = 0.0f, GLfloat y = 0.0f, GLfloat z = 0.0f);
    Vector3(const GLfloat * data);
    Vector3(const Vector3 & victim);
    Vector3(const Point3 & victim);

    GLfloat Length() const;
    GLfloat LengthSquared() const;
    GLfloat Distance(const Vector3 & rhs) const;
    GLfloat DistanceSquared(const Vector3 & rhs) const;
    Vector3 Normalized() const;
    void Normalize();

    GLfloat Angle(const Vector3 & rhs) const;
    GLfloat Dot(const Vector3 & rhs) const;
    Vector3 Cross(const Vector3 & rhs) const;

    Vector3 operator -() const;
    Vector3 operator +(const Vector3 & rhs) const;
    Vector3 operator -(const Vector3 & rhs) const;
    Vector3 operator *(const Vector3 & rhs) const;
    Vector3 operator /(const Vector3 & rhs) const;

    friend Vector3 operator *(const Vector3 & lhs, GLfloat rhs);
    friend Vector3 operator *(GLfloat lhs, const Vector3 & rhs);
    friend Vector3 operator /(const Vector3 & lhs, GLfloat rhs);
    friend Vector3 operator /(GLfloat lhs, const Vector3 & rhs);

    friend class Point3;
};

Vector3 operator *(const Vector3 & lhs, GLfloat rhs);
Vector3 operator *(GLfloat lhs, const Vector3 & rhs);
Vector3 operator /(const Vector3 & lhs, GLfloat rhs);
Vector3 operator /(GLfloat lhs, const Vector3 & rhs);

//----------------------------------------------------------------------------------------------------
// Quaternion
//----------------------------------------------------------------------------------------------------

class Quaternion : public Coordinates {
public:
    Quaternion (GLfloat x = 0.0f, GLfloat y = 0.0f, GLfloat z = 0.0f, GLfloat w = 0.0f);
    Quaternion (const GLfloat * data);
    Quaternion (const Quaternion & victim);

    GLfloat Length() const;
    GLfloat LengthSquared() const;
    Quaternion Normalized() const;
    void Normalize();

    Quaternion Conjugate () const;

    Quaternion operator * (Quaternion rhs) const;
};

//****************************************************************************************************
//******************************************** Materials *********************************************
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
// Texture
//----------------------------------------------------------------------------------------------------

class Texture {
private:
    GLuint width_, height_, bpp_;
    std::shared_ptr<GLubyte> buffer_;
    GLuint name_;

public:
    Texture();
    ~Texture();

    inline GLuint Width() const { return width_; }
    inline GLuint Height() const { return height_; }
    inline GLuint BPP() const { return bpp_; }
    inline GLuint Name() const { return name_; }

    void Release();
    void ReleaseFromCard();

    bool LoadBMP(const char * path);
    bool LoadTGA(const char * path);
    bool LoadIntoCard();
};

//----------------------------------------------------------------------------------------------------
// Material
//----------------------------------------------------------------------------------------------------

class Material {
public:
    static const int R_IDX, G_IDX, B_IDX, A_IDX;
    static const GLfloat4 NO_COLOR;
    static const GLfloat4 FULL_COLOR;

private:
    GLfloat4 color_;
    GLfloat4 ambient_;
    GLfloat4 diffuse_;
    GLfloat4 specular_;
    GLfloat4 emission_;
    GLfloat shininess_;
    GLuint texture_;

public:
    Material();

    void SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);
    void SetAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);
    void SetDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);
    void SetSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);
    void SetEmission(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);
    void SetShininess(GLfloat value);
    void SetTexture(GLuint value);

    void Apply();
};

//****************************************************************************************************
//******************************************* Scene graph ********************************************
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
// NodeObject
//----------------------------------------------------------------------------------------------------

class NodeObject {
protected:
    Material material_;
    std::vector<NodeObject *> childrens_;
    void drawChildrens();
public:
    NodeObject();
    virtual ~NodeObject();
    Material & GetMaterial();
    void AddChildren(NodeObject * victim);
    void ClearChildrens();
    virtual void Initialize();
    virtual void Draw();
};

#endif
