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

#ifndef __GTEXTURE_H__
#define __GTEXTURE_H__

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <memory>

//----------------------------------------------------------------------------------------------------
// Texture
//----------------------------------------------------------------------------------------------------

class Texture {
private:
    GLuint width_, height_, bpp_;
    std::shared_ptr<GLubyte> buffer_;
    GLuint name_;

    unsigned char * loadBitmapFile(const char * name, BITMAPINFOHEADER * info) {
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

public:
    Texture() : width_(0), height_(0), bpp_(0), buffer_(nullptr), name_(0) {}
    ~Texture() { Release(); }

    inline GLuint Width() const { return width_; }
    inline GLuint Height() const { return height_; }
    inline GLuint BPP() const { return bpp_; }
    inline GLuint Name() const { return name_; }

    void Release() {
        width_ = height_ = bpp_ = 0;
        if (buffer_.unique()) {
            ReleaseFromCard();
        }
        buffer_ = nullptr;
    }

    void ReleaseFromCard() {
        if (name_) {
            if (glIsTexture(name_)) {
                glDeleteTextures(1, &name_);
            }
            name_ = 0;
        }
    }

    bool LoadBMP(const char * path) {
        if (buffer_) Release();

        BITMAPINFOHEADER info;
        buffer_.reset(loadBitmapFile(path, &info));
        if (!buffer_) return false;

        width_  = info.biWidth;
        height_ = info.biHeight;
        bpp_    = info.biBitCount;

        return LoadIntoCard();
    }

    bool LoadIntoCard() {
        GLenum format = GL_RGB;
        switch (bpp_) {
        case 24: format = GL_RGB; break;
        case 32: format = GL_RGBA; break;
        default: Release(); return false;
        }
        glGenTextures(1, &name_);
        glBindTexture(GL_TEXTURE_2D, name_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width_, height_, format, GL_UNSIGNED_BYTE, buffer_.get());
        return true;
    }

    void Bind() const {
        glBindTexture(GL_TEXTURE_2D, name_);
    }

    void Unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};

#endif
