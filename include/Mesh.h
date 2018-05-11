#ifndef MESH_H
#define MESH_H

/**
 * Based on: https://gitlab.com/wikibooks-opengl/modern-tutorials/blob/master/obj-viewer/obj-viewer.cpp
 **/

#include <Vector3.h>
#include <Vector4.h>
#include <vector>
#include <GL/glew.h>
#include <SOIL/SOIL.h>
#include <GL/freeglut.h>
using namespace std;

class Face {
public:
    GLushort vertex, texture, normal;

    void decrease() {
        vertex--;
        texture--;
        normal--;
    }
};

class Triangle {
public:
    Face faces[3];
};

class Mesh {
    public:
        Mesh() : verticesVBO(0), normalsVBO(0), elementsIBO(0) {};
        virtual ~Mesh();

        void loadFromFile(const char* fileName);
        void draw();
    protected:

    private:
        void upload();
        // Buffers de v�rtices e de �ndices
        GLuint verticesVBO, normalsVBO, elementsIBO;

        vector<Vector4d> vertices;
        vector<Vector3d> normals;
        vector<Vector3d> textures;
        vector<GLushort> eNormals;
        vector<GLushort> eTextures;
        vector<Triangle> elements;
};

#endif // MESH_H
