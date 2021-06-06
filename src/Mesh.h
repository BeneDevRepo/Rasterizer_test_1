#pragma once

class Texture;

class Mesh {
public:
    struct vec3 {
        float x, y, z;
    };
    vec3* vertices;
    unsigned int* indices;
    unsigned int numTris;

    Mesh(vec3* vertices, unsigned int* indices, unsigned int numTris);
    Mesh(float* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numTris);
    ~Mesh();
    void drawTo(Texture* tex);
};