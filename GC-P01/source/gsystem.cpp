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

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "gsystem.h"
#include <gl/GLU.h>
#include <algorithm>
#include <cstdio>
#include <cmath>

//****************************************************************************************************
//*************************************** General structures *****************************************
//****************************************************************************************************

static void GLfloat4Set(GLfloat * victim, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
    victim[0] = v0; victim[1] = v1;
    victim[2] = v2; victim[3] = v3;
}

//****************************************************************************************************
//***************************** Mathematical operations and structures *******************************
//****************************************************************************************************

// https://msdn.microsoft.com/en-us/library/microsoft.xna.framework.vector3_members.aspx
// https://msdn.microsoft.com/en-us/library/microsoft.xna.framework.quaternion_members.aspx
// https://msdn.microsoft.com/en-us/library/microsoft.xna.framework.matrix_members.aspx
// http://docs.unity3d.com/ScriptReference/Vector3.html
// http://docs.unity3d.com/ScriptReference/Quaternion.html
// http://docs.unity3d.com/ScriptReference/Matrix4x4.html

//====================================================================================================
// class Coordinates:
//====================================================================================================

const int Coordinates::X_IDX = 0;
const int Coordinates::Y_IDX = 1;
const int Coordinates::Z_IDX = 2;
const int Coordinates::W_IDX = 3;

const GLfloat Coordinates::W_AS_VECTOR = 0.0f;
const GLfloat Coordinates::W_AS_POINT = 1.0f;

//----------------------------------------------------------------------------------------------------
// Constructors:
//----------------------------------------------------------------------------------------------------

Coordinates::Coordinates(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
    Set(x, y, z, w);
}

Coordinates::Coordinates(const GLfloat * data) {
    Set(data);
}

Coordinates::Coordinates(const Coordinates & victim) {
    Set(victim);
}

//----------------------------------------------------------------------------------------------------
// Methods:
//----------------------------------------------------------------------------------------------------

const GLfloat * Coordinates::Get() const {
    return data_;
}

void Coordinates::Set(const Coordinates & value) {
    memcpy(data_, value.data_, ARRAY4_LENGTH * sizeof(GLfloat));
    validateChanges();
}

void Coordinates::Set(const GLfloat * data) {
    memcpy(data_, data, ARRAY4_LENGTH * sizeof(GLfloat));
    validateChanges();
}

void Coordinates::Set(GLfloat x, GLfloat y, GLfloat z) {
    data_[X_IDX] = x;
    data_[Y_IDX] = y;
    data_[Z_IDX] = z;
    validateChanges();
}

void Coordinates::Set(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
    data_[X_IDX] = x;
    data_[Y_IDX] = y;
    data_[Z_IDX] = z;
    data_[W_IDX] = w;
    validateChanges();
}

//----------------------------------------------------------------------------------------------------
// Operators:
//----------------------------------------------------------------------------------------------------

Coordinates & Coordinates::operator =(const Coordinates & rhs) {
    Set(rhs);
    return *this;
}

bool Coordinates::operator ==(const Coordinates & rhs) const {
    return data_[X_IDX] == rhs.data_[X_IDX] && data_[Y_IDX] == rhs.data_[Y_IDX] &&
           data_[Z_IDX] == rhs.data_[Z_IDX] && data_[W_IDX] == rhs.data_[W_IDX];
}

bool Coordinates::operator !=(const Coordinates & rhs) const {
    return data_[X_IDX] != rhs.data_[X_IDX] || data_[Y_IDX] != rhs.data_[Y_IDX] ||
           data_[Z_IDX] != rhs.data_[Z_IDX] || data_[W_IDX] != rhs.data_[W_IDX];
}

//====================================================================================================
// class Point3:
//====================================================================================================

const Point3 Point3::ZERO(0.0f, 0.0f, 0.0f);

//----------------------------------------------------------------------------------------------------
// Constructors:
//----------------------------------------------------------------------------------------------------

Point3::Point3 (GLfloat x, GLfloat y, GLfloat z) : Coordinates(x, y, z, W_AS_POINT) {}

Point3::Point3 (const GLfloat * data) : Coordinates(data) {}

Point3::Point3 (const Point3 & victim) : Coordinates(victim) {}

Point3::Point3 (const Vector3 & victim) : Coordinates(victim.data_[X_IDX], victim.data_[Y_IDX],
    victim.data_[Z_IDX], W_AS_POINT) {}

//----------------------------------------------------------------------------------------------------
// Methods:
//----------------------------------------------------------------------------------------------------

void Point3::validateChanges() {
    data_[W_IDX] = W_AS_POINT;
}

//----------------------------------------------------------------------------------------------------
// Operators:
//----------------------------------------------------------------------------------------------------

Point3 Point3::operator +(const Vector3 & rhs) {
    return Point3(
        data_[X_IDX] + rhs.data_[X_IDX],
        data_[Y_IDX] + rhs.data_[Y_IDX],
        data_[Z_IDX] + rhs.data_[Z_IDX]
    );
}

Point3 Point3::operator -(const Vector3 & rhs) {
    return Point3(
        data_[X_IDX] - rhs.data_[X_IDX],
        data_[Y_IDX] - rhs.data_[Y_IDX],
        data_[Z_IDX] - rhs.data_[Z_IDX]
    );
}

Point3 Point3::operator *(const Vector3 & rhs) {
    return Point3(
        data_[X_IDX] * rhs.data_[X_IDX],
        data_[Y_IDX] * rhs.data_[Y_IDX],
        data_[Z_IDX] * rhs.data_[Z_IDX]
    );
}

Point3 Point3::operator /(const Vector3 & rhs) {
    return Point3(
        data_[X_IDX] / rhs.data_[X_IDX],
        data_[Y_IDX] / rhs.data_[Y_IDX],
        data_[Z_IDX] / rhs.data_[Z_IDX]
    );
}

//====================================================================================================
// class Vector3:
//====================================================================================================

const Vector3 Vector3::ZERO(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::IDENTITY(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::UNIT_X(1.0f, 1.0f, 0.0f);
const Vector3 Vector3::UNIT_Y(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::UNIT_Z(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::LEFT(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::RIGHT(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::UP(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::DOWN(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::FORWARD(0.0f, 0.0f, -1.0f);
const Vector3 Vector3::BACKWARD(0.0f, 0.0f, 1.0f);

//----------------------------------------------------------------------------------------------------
// Constructors:
//----------------------------------------------------------------------------------------------------

Vector3::Vector3 (GLfloat x, GLfloat y, GLfloat z) : Coordinates(x, y, z, W_AS_VECTOR) {}

Vector3::Vector3 (const GLfloat * data) : Coordinates(data) {}

Vector3::Vector3 (const Vector3 & victim) : Coordinates(victim) {}

Vector3::Vector3 (const Point3 & victim) : Coordinates(victim.data_[X_IDX], victim.data_[Y_IDX],
    victim.data_[Z_IDX], W_AS_VECTOR) {}

//----------------------------------------------------------------------------------------------------
// Methods:
//----------------------------------------------------------------------------------------------------

void Vector3::validateChanges() {
    data_[W_IDX] = W_AS_VECTOR;
}

GLfloat Vector3::Length() const {
    return std::sqrt(LengthSquared());
}

GLfloat Vector3::LengthSquared() const {
    return data_[X_IDX] * data_[X_IDX] +
           data_[Y_IDX] * data_[Y_IDX] +
           data_[Z_IDX] * data_[Z_IDX];
}

GLfloat Vector3::Distance(const Vector3 & rhs) const {
    return std::sqrt(DistanceSquared(rhs));
}

GLfloat Vector3::DistanceSquared(const Vector3 & rhs) const {
    Vector3 delta = (*this) - rhs;
    return delta.LengthSquared();
}

Vector3 Vector3::Normalized() const {
    GLfloat norm = Length();
    return Vector3(
        data_[X_IDX] / norm,
        data_[Y_IDX] / norm,
        data_[Z_IDX] / norm
    );
}

void Vector3::Normalize() {
    GLfloat norm = Length();
    data_[X_IDX] /= norm;
    data_[Y_IDX] /= norm;
    data_[Z_IDX] /= norm;
}

GLfloat Vector3::Angle(const Vector3 & rhs) const {
    return RadToDeg(std::acos(Dot(rhs) / (Length() * rhs.Length())));
}

GLfloat Vector3::Dot(const Vector3 & rhs) const {
    return data_[X_IDX] * rhs.data_[X_IDX] +
           data_[Y_IDX] * rhs.data_[Y_IDX] +
           data_[Z_IDX] * rhs.data_[Z_IDX];
}

Vector3 Vector3::Cross(const Vector3 & rhs) const {
    return Vector3(
        data_[Y_IDX] * rhs.data_[Z_IDX] - data_[Z_IDX] * rhs.data_[Y_IDX],
        data_[Z_IDX] * rhs.data_[X_IDX] - data_[X_IDX] * rhs.data_[Z_IDX],
        data_[X_IDX] * rhs.data_[Y_IDX] - data_[Y_IDX] * rhs.data_[X_IDX]
    );
}

//----------------------------------------------------------------------------------------------------
// Operators:
//----------------------------------------------------------------------------------------------------

Vector3 Vector3::operator -() const {
    return Vector3(-data_[X_IDX], -data_[Y_IDX], -data_[Z_IDX]);
}

Vector3 Vector3::operator +(const Vector3 & rhs) const {
    return Vector3(
        data_[X_IDX] + rhs.data_[X_IDX],
        data_[Y_IDX] + rhs.data_[Y_IDX],
        data_[Z_IDX] + rhs.data_[Z_IDX]
    );
}

Vector3 Vector3::operator -(const Vector3 & rhs) const {
    return Vector3(
        data_[X_IDX] - rhs.data_[X_IDX],
        data_[Y_IDX] - rhs.data_[Y_IDX],
        data_[Z_IDX] - rhs.data_[Z_IDX]
    );
}

Vector3 Vector3::operator *(const Vector3 & rhs) const {
    return Vector3(
        data_[X_IDX] * rhs.data_[X_IDX],
        data_[Y_IDX] * rhs.data_[Y_IDX],
        data_[Z_IDX] * rhs.data_[Z_IDX]
    );
}

Vector3 Vector3::operator /(const Vector3 & rhs) const {
    return Vector3(
        data_[X_IDX] / rhs.data_[X_IDX],
        data_[Y_IDX] / rhs.data_[Y_IDX],
        data_[Z_IDX] / rhs.data_[Z_IDX]
    );
}

Vector3 operator *(const Vector3 & lhs, GLfloat rhs) {
    return Vector3(
        lhs.data_[Coordinates::X_IDX] * rhs,
        lhs.data_[Coordinates::Y_IDX] * rhs,
        lhs.data_[Coordinates::Z_IDX] * rhs
    );
}

Vector3 operator *(GLfloat lhs, const Vector3 & rhs) {
    return Vector3(
        rhs.data_[Coordinates::X_IDX] * lhs,
        rhs.data_[Coordinates::Y_IDX] * lhs,
        rhs.data_[Coordinates::Z_IDX] * lhs
    );
}

Vector3 operator /(const Vector3 & lhs, GLfloat rhs) {
    return Vector3(
        lhs.data_[Coordinates::X_IDX] / rhs,
        lhs.data_[Coordinates::Y_IDX] / rhs,
        lhs.data_[Coordinates::Z_IDX] / rhs
    );
}

Vector3 operator /(GLfloat lhs, const Vector3 & rhs) {
    return Vector3(
        rhs.data_[Coordinates::X_IDX] / lhs,
        rhs.data_[Coordinates::Y_IDX] / lhs,
        rhs.data_[Coordinates::Z_IDX] / lhs
    );
}

//====================================================================================================
// class Quaternion:
//====================================================================================================

//----------------------------------------------------------------------------------------------------
// Constructors:
//----------------------------------------------------------------------------------------------------

Quaternion::Quaternion (GLfloat x, GLfloat y, GLfloat z, GLfloat w) : Coordinates(x, y, z, w) {}

Quaternion::Quaternion (const GLfloat * data) : Coordinates(data) {}

Quaternion::Quaternion (const Quaternion & victim) : Coordinates(victim) {}

//----------------------------------------------------------------------------------------------------
// Methods:
//----------------------------------------------------------------------------------------------------

GLfloat Quaternion::Length() const {
    return std::sqrt(LengthSquared());
}

GLfloat Quaternion::LengthSquared() const {
    return data_[X_IDX] * data_[X_IDX] +
           data_[Y_IDX] * data_[Y_IDX] +
           data_[Z_IDX] * data_[Z_IDX] +
           data_[W_IDX] * data_[W_IDX];
}

Quaternion Quaternion::Normalized() const {
    GLfloat norm = Length();
    return Quaternion(
        data_[X_IDX] / norm,
        data_[Y_IDX] / norm,
        data_[Z_IDX] / norm,
        data_[W_IDX] / norm
    );
}

void Quaternion::Normalize() {
    GLfloat norm = Length();
    data_[X_IDX] /= norm;
    data_[Y_IDX] /= norm;
    data_[Z_IDX] /= norm;
    data_[W_IDX] /= norm;
}

Quaternion Quaternion::Conjugate () const {
    return Quaternion(-data_[X_IDX], -data_[Y_IDX], -data_[Z_IDX], data_[W_IDX]);
}

//----------------------------------------------------------------------------------------------------
// Operators:
//----------------------------------------------------------------------------------------------------

Quaternion Quaternion::operator * (Quaternion rhs) const {
    return Quaternion(
        data_[W_IDX] * rhs.data_[X_IDX] + data_[X_IDX] * rhs.data_[W_IDX] +
        data_[Y_IDX] * rhs.data_[Z_IDX] - data_[Z_IDX] * rhs.data_[Y_IDX],
        data_[W_IDX] * rhs.data_[Y_IDX] - data_[X_IDX] * rhs.data_[Z_IDX] +
        data_[Y_IDX] * rhs.data_[W_IDX] + data_[Z_IDX] * rhs.data_[X_IDX],
        data_[W_IDX] * rhs.data_[Z_IDX] + data_[X_IDX] * rhs.data_[Y_IDX] -
        data_[Y_IDX] * rhs.data_[X_IDX] + data_[Z_IDX] * rhs.data_[W_IDX],
        data_[W_IDX] * rhs.data_[W_IDX] - data_[X_IDX] * rhs.data_[X_IDX] -
        data_[Y_IDX] * rhs.data_[Y_IDX] - data_[Z_IDX] * rhs.data_[Z_IDX]
    );
}

//****************************************************************************************************
//******************************************** Materials *********************************************
//****************************************************************************************************

//====================================================================================================
// Types:
//====================================================================================================

struct TargaFileHeader {
    BYTE  imageIDLength;     // ID field length (=0)
    BYTE  colorMapType;      // = 0;
    BYTE  imageTypeCode;     // = 2; (RGB uncompressed)
                             // = 3; (Grayscale uncompressed)
    INT16 colorMapOrigin;    // = 0;
    INT16 colorMapLength;    // = 0;
    INT16 colorMapEntrySize; // = 0;
    INT16 imageXOrigin;      // = 0;
    INT16 imageYOrigin;      // = 0;
    INT16 imageWidth;        // Width in pixels of the image.
    INT16 imageHeight;       // Height in pixels of the image.
    BYTE  bitCount;          // Color depth (16, 24, 32)
    BYTE  imageDescriptor;   // 24bits = 0x00, 32bits = 0x08
};

struct TargaFile {
    BYTE   imageTypeCode;
    INT16  imageWidth;
    INT16  imageHeight;
    BYTE   bitCount;
    BYTE * imageData;
};

//====================================================================================================
// Functions:
//====================================================================================================

unsigned char * LoadBitmapFile (const char * name, BITMAPINFOHEADER * info) {
    // Open the file:
    FILE * file = fopen(name, "rb");
    if(file == nullptr) return nullptr;

    // Read the header and check we have a bmp:
    BITMAPFILEHEADER header;
    fread(&header, sizeof(BITMAPFILEHEADER), 1, file);
    if(header.bfType != 0x4D42) {
        fclose(file);
        return nullptr;
    }

    // Read another the header and check that the bmp is uncompressed:
    fread(info, sizeof(BITMAPINFOHEADER), 1, file);
    if(info->biCompression != BI_RGB) {
        fclose(file);
        return nullptr;
    }

    // Move the pointer to the section with the pixels:
    fseek(file, header.bfOffBits, SEEK_SET);

    // Ask memory for the image's buffer:
    BYTE  colormode = info->biBitCount / 8;
    DWORD imagesize = info->biWidth * info->biHeight * colormode;
    unsigned char * buffer = new unsigned char[imagesize];
    if(buffer == nullptr) {
        fclose(file);
        return nullptr;
    }

    // Read the contents of the image:
    fread(buffer, 1, imagesize, file);

    // Convert pixels from BGR to RGB:
    unsigned char temprgb;
    for(DWORD imgidx = 0; imgidx < imagesize; imgidx += 3) {
        temprgb = buffer[imgidx];
        buffer[imgidx] = buffer[imgidx + 2];
        buffer[imgidx + 2] = temprgb;
    }

    // Close the file and return the buffer:
    fclose(file);
    return buffer;
}

//----------------------------------------------------------------------------------------------------

bool LoadTargaFile (const char * name, TargaFile * targaFile) {
    // Open the file:
    FILE * file = fopen(name, "rb");
    if (file == nullptr) return false;

    // Read the type of file:
    TargaFileHeader header;
    fread(&(header.imageIDLength), 1, 1, file);
    fread(&(header.colorMapType),  1, 1, file);
    fread(&(header.imageTypeCode), 1, 1, file);

    // Validate the type:
    if ((header.imageTypeCode != 2) && (header.imageTypeCode != 3)) {
        fclose(file);
        return false;
    }

    // Read the rest of the header:
    fread(&(header.colorMapOrigin),    2, 1, file);
    fread(&(header.colorMapLength),    2, 1, file);
    fread(&(header.colorMapEntrySize), 1, 1, file);
    fread(&(header.imageXOrigin),      2, 1, file);
    fread(&(header.imageYOrigin),      2, 1, file);
    fread(&(header.imageWidth),        2, 1, file);
    fread(&(header.imageHeight),       2, 1, file);
    fread(&(header.bitCount),          1, 1, file);
    fread(&(header.imageDescriptor),   1, 1, file);

    // Save the necessary information on the TGAFILE structure:
    targaFile->imageTypeCode = header.imageTypeCode;
    targaFile->imageWidth    = header.imageWidth;
    targaFile->imageHeight   = header.imageHeight;
    targaFile->bitCount      = header.bitCount;

    // Calculate the size of the image and the pixel in bytes:
    BYTE  colormode = targaFile->bitCount / 8;
    DWORD imagesize = targaFile->imageWidth * targaFile->imageHeight * colormode;

    // Ask memory for the image's buffer:
    targaFile->imageData = new BYTE[imagesize];
    if(targaFile->imageData == nullptr) {
        fclose(file);
        return false;
    }

    // Read the contents of the image:
    fread(targaFile->imageData, sizeof(BYTE), imagesize, file);

    // Convert pixels from BGR to RGB:
    BYTE temprgb;
    for(DWORD imgidx = 0; imgidx < imagesize; imgidx += colormode) {
        temprgb = targaFile->imageData[imgidx];
        targaFile->imageData[imgidx] = targaFile->imageData[imgidx + 2];
        targaFile->imageData[imgidx + 2] = temprgb;
    }

    // Close the file and return that everything is ok:
    fclose(file);
    return true;
}

//====================================================================================================
// class Texture:
//====================================================================================================

//----------------------------------------------------------------------------------------------------
// Constructors:
//----------------------------------------------------------------------------------------------------

Texture::Texture() : width_(0), height_(0), bpp_(0), buffer_(nullptr), name_(0) {}

Texture::~Texture() {
    Release();
}

//----------------------------------------------------------------------------------------------------
// Methods:
//----------------------------------------------------------------------------------------------------

void Texture::Release() {
    width_ = height_ = bpp_ = 0;
    if (buffer_.unique()) {
        ReleaseFromCard();
    }
    buffer_ = nullptr;
}

void Texture::ReleaseFromCard() {
    if (name_) {
        if (glIsTexture(name_)) {
            glDeleteTextures(1, &name_);
        }
        name_ = 0;
    }
}

bool Texture::LoadBMP(const char * path) {
    if (buffer_) Release();

    BITMAPINFOHEADER info;
    buffer_.reset(LoadBitmapFile(path, &info));
    if (!buffer_) return false;

    width_  = info.biWidth;
    height_ = info.biHeight;
    bpp_    = info.biBitCount;

    return LoadIntoCard();
}

bool Texture::LoadTGA(const char * path) {
    if (buffer_) Release();

    TargaFile info;
    if (!LoadTargaFile(path, &info)) return false;
    buffer_.reset(info.imageData);

    width_  = info.imageWidth;
    height_ = info.imageHeight;
    bpp_    = info.bitCount;

    return LoadIntoCard();
}

bool Texture::LoadIntoCard() {
    GLenum format = GL_RGB;
    switch (bpp_) {
    case 24:
        format = GL_RGB;
        break;
    case 32:
        format = GL_RGBA;
        break;
    default:
        Release();
        return false;
    }
    glGenTextures(1, &name_);
    glBindTexture(GL_TEXTURE_2D, name_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width_, height_, format, GL_UNSIGNED_BYTE, buffer_.get());
    return true;
}

//====================================================================================================
// class Material:
//====================================================================================================

const int Material::R_IDX = 0;
const int Material::G_IDX = 1;
const int Material::B_IDX = 2;
const int Material::A_IDX = 3;

const GLfloat4 Material::NO_COLOR   = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat4 Material::FULL_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };

//----------------------------------------------------------------------------------------------------
// Constructors:
//----------------------------------------------------------------------------------------------------

Material::Material() : shininess_(0.0f), texture_(0) {
    memcpy(color_, FULL_COLOR, ARRAY4_LENGTH * sizeof(GLfloat));
    memcpy(ambient_, FULL_COLOR, ARRAY4_LENGTH * sizeof(GLfloat));
    memcpy(diffuse_, FULL_COLOR, ARRAY4_LENGTH * sizeof(GLfloat));
    memcpy(specular_, NO_COLOR, ARRAY4_LENGTH * sizeof(GLfloat));
    memcpy(emission_, NO_COLOR, ARRAY4_LENGTH * sizeof(GLfloat));
}

//----------------------------------------------------------------------------------------------------
// Methods:
//----------------------------------------------------------------------------------------------------

void Material::SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    GLfloat4Set(color_, r, g, b, a);
}

void Material::SetAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    GLfloat4Set(ambient_, r, g, b, a);
}

void Material::SetDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    GLfloat4Set(diffuse_, r, g, b, a);
}

void Material::SetSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    GLfloat4Set(specular_, r, g, b, a);
}

void Material::SetEmission(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    GLfloat4Set(emission_, r, g, b, a);
}

void Material::SetShininess(GLfloat value) {
    shininess_ = value;
}

void Material::SetTexture(GLuint value) {
    texture_ = value;
}

void Material::Apply() {
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_);
    glMaterialfv(GL_FRONT, GL_EMISSION, emission_);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess_);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glColor4fv(color_);
}

//****************************************************************************************************
//******************************************** Materials *********************************************
//****************************************************************************************************

//====================================================================================================
// class NodeObject:
//====================================================================================================

//----------------------------------------------------------------------------------------------------
// Constructors:
//----------------------------------------------------------------------------------------------------

NodeObject::NodeObject() {}

NodeObject::~NodeObject() {}

//----------------------------------------------------------------------------------------------------
// Methods:
//----------------------------------------------------------------------------------------------------

Material & NodeObject::GetMaterial() {
    return material_;
}

void NodeObject::AddChildren(NodeObject * victim) {
    childrens_.push_back(victim);
}

void NodeObject::ClearChildrens() {
    childrens_.clear();
}

void NodeObject::Initialize() {
}

void NodeObject::Draw() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        material_.Apply();
        drawChildrens();
    glPopMatrix();
}

void NodeObject::drawChildrens() {
    std::for_each(std::begin(childrens_), std::end(childrens_),
        [] (NodeObject * victim) {
            victim->Draw();
        }
    );
}
