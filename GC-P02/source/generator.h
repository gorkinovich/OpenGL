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

#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include "gmesh.h"
#include <cctype>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>

//----------------------------------------------------------------------------------------------------
// Outline3D
//----------------------------------------------------------------------------------------------------

class Outline3D {
private:
    std::vector<Vector3D> data_;

public:
    Outline3D() : data_() {}
    Outline3D(const std::vector<Vector3D> & data) : data_(data) {}
    Outline3D(const Outline3D & v) : data_(v.data_) {}

    const std::vector<Vector3D> & Data() const { return data_; }

    inline const std::vector<Vector3D>::const_iterator Begin() const { return data_.cbegin(); }
    inline const std::vector<Vector3D>::const_iterator End() const { return data_.cend(); }

    void LoadFromFile(const std::string & path) {
        data_.clear();
        std::string line;
        std::ifstream file(path);
        bool readCoords = false;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                if (line[0] == '*') {
                    std::transform(line.begin(), line.end(), line.begin(), std::tolower);
                    if (line.find("coordinates") != std::string::npos) {
                        readCoords = true;
                    } else {
                        readCoords = false;
                    }
                } else if (readCoords) {
                    int k = 0;
                    std::string item;
                    std::stringstream sline(line);
                    Vector3D victim;
                    while (std::getline(sline, item, '\t')) {
                        float number = static_cast<float>(std::atof(item.c_str()));
                        if (k == 0) {
                            victim.X(number);
                        } else if (k == 1) {
                            victim.Y(number);
                        } else if (k == 2) {
                            victim.Z(number);
                        }
                        ++k;
                    }
                    data_.push_back(victim);
                }
            }
        }
    }
};

//----------------------------------------------------------------------------------------------------
// Generator3D
//----------------------------------------------------------------------------------------------------

class Generator3D {
public:
    static SimpleMesh3D SimpleOutlineByRotation(const Outline3D & outline, GLuint slices, bool fillHoles = false) {
        // Prepare the partition angles:
        Matrix transform;
        GLfloat currentAngle = 0.0f, angleStep = 2.0f * PI / static_cast<GLfloat>(slices);

        // Calculate vertices and faces of the figure:
        std::vector<Vector3D> vertex;
        std::vector<FaceWithNormal3D> face;
        auto size = outline.Data().size();
        for (GLuint i = 0; i < slices; ++i) {
            // vertices of the figure:
            transform.SetAsRotateY(currentAngle);
            std::for_each(outline.Begin(), outline.End(), [&] (const Vector3D & item) {
                vertex.push_back(transform * item);
            });
            currentAngle += angleStep;
            // Faces of the figure:
            GLuint leftBase = i * size, rightBase = ((i + 1) % slices) * size;
            for (GLuint j = 0; j < size - 1; ++j) {
                std::vector<GLuint> idx;
                idx.push_back(leftBase + j + 1);
                idx.push_back(leftBase + j);
                idx.push_back(rightBase + j);
                idx.push_back(rightBase + j + 1);
                face.push_back(FaceWithNormal3D(idx));
            }
        }

        // Calculate faces of the top & down part:
        if (fillHoles) {
            std::vector<GLuint> holeDown, holeUp;
            for (GLuint i = 0; i < slices; ++i) {
                holeDown.push_back(i * size);
                holeUp.push_back(i * size + (size - 1));
            }
            std::reverse(std::begin(holeDown), std::end(holeDown));
            face.push_back(FaceWithNormal3D(holeDown));
            face.push_back(FaceWithNormal3D(holeUp));
        }

        // Configure & return the final model:
        SimpleMesh3D result(vertex, face);
        result.CalculateNormals();
        return result;
    }

    static SimpleMesh3D SimpleBox(GLfloat length) {
        // Calculate vertices and faces of the figure:
        GLfloat halfSide = length / 2.0f;
        std::vector<Vector3D> vertex;
        std::vector<FaceWithNormal3D> face;

        vertex.push_back(Vector3D(-halfSide, halfSide, halfSide));
        vertex.push_back(Vector3D(-halfSide, -halfSide, halfSide));
        vertex.push_back(Vector3D(halfSide, -halfSide, halfSide));
        vertex.push_back(Vector3D(halfSide, halfSide, halfSide));
        vertex.push_back(Vector3D(-halfSide, halfSide, -halfSide));
        vertex.push_back(Vector3D(-halfSide, -halfSide, -halfSide));
        vertex.push_back(Vector3D(halfSide, -halfSide, -halfSide));
        vertex.push_back(Vector3D(halfSide, halfSide, -halfSide));

        std::vector<GLuint> idx;
        idx.push_back(0); idx.push_back(1);
        idx.push_back(2); idx.push_back(3);
        face.push_back(FaceWithNormal3D(idx));

        idx.clear();
        idx.push_back(7); idx.push_back(6);
        idx.push_back(5); idx.push_back(4);
        face.push_back(FaceWithNormal3D(idx));

        idx.clear();
        idx.push_back(3); idx.push_back(2);
        idx.push_back(6); idx.push_back(7);
        face.push_back(FaceWithNormal3D(idx));

        idx.clear();
        idx.push_back(4); idx.push_back(5);
        idx.push_back(1); idx.push_back(0);
        face.push_back(FaceWithNormal3D(idx));

        idx.clear();
        idx.push_back(0); idx.push_back(3);
        idx.push_back(7); idx.push_back(4);
        face.push_back(FaceWithNormal3D(idx));

        idx.clear();
        idx.push_back(1); idx.push_back(5);
        idx.push_back(6); idx.push_back(2);
        face.push_back(FaceWithNormal3D(idx));

        // Configure & return the final model:
        SimpleMesh3D result(vertex, face);
        result.CalculateNormals();
        return result;
    }
};

#endif
