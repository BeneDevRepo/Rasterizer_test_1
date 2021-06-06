#include "Mesh.h"

#include "Texture.h"
#include <cmath>
#include <iostream>

#include <chrono>

#include "math.hh"

Mesh::Mesh(vec3* vertices, unsigned int* indices, unsigned int numTris): vertices(vertices), indices(indices), numTris(numTris) {
}

Mesh::Mesh(float* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numTris): vertices(new vec3[numVertices]()), indices(indices), numTris(numTris) {
    for(unsigned int i=0; i<numVertices; i++)
        this->vertices[i] = {vertices[i*3]*100, vertices[i*3+1]*100, vertices[i*3+2]*100};
}

Mesh::~Mesh() {
}

void rotY(Mesh::vec3& v) {
    double a = atan2(v.z, v.x);
    float mag = sqrt(v.z*v.z + v.x*v.x);

    using namespace std::chrono;
    int64_t m = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    a += m/1000.;
    v.x = cos(a) * mag;
    v.z = sin(a) * mag;
}


#define PI 3.1415926535
#define TWO_PI (PI*2)
boolean cw(const Mesh::vec3& p1, const Mesh::vec3& p2, const Mesh::vec3& p3) {
//   float a1 = atan2(p2.y-p1.y, p2.x-p1.x);
//   float a2 = atan2(p3.y-p1.y, p3.x-p1.x);
  float a1 = atan2(p2.y-p1.y, p2.x-p1.x);
  float a2 = atan2(p3.y-p1.y, p3.x-p1.x);
  float ad = a2-a1;
  while (ad<0)
    ad += TWO_PI;
  return (ad>0 && ad<PI);
}

void Mesh::drawTo(Texture* tex) {
    for(unsigned int i=0; i<this->numTris; i++) {
        vec3 tri[] {
            this->vertices[this->indices[i*3+0]],
            this->vertices[this->indices[i*3+1]],
            this->vertices[this->indices[i*3+2]]
        };

        for(int j=0; j<3; j++) {
            rotY(tri[j]);
            tri[j].z += 150;
            // v2.z += 150;
            tri[j].x = tri[j].x / tri[j].z * 300;
            tri[j].y = tri[j].y / tri[j].z * 300;
        }

        

        if(!cw( tri[0], tri[1], tri[2] ))
            continue;

        
        for(unsigned int line=0; line<3; line++) {
            // int i1 = this->indices[i*3+line];
            // int i2 = this->indices[i*3+(line+1)%3];
            // vec3 v1 = this->vertices[i1];
            // vec3 v2 = this->vertices[i2];

            vec3 v1 = tri[line];
            vec3 v2 = tri[(line+1)%3];

            // rotY(v1);
            // rotY(v2);

            // v1.z += 150;
            // v2.z += 150;
            
            tex->line(tex->width /2-v1.x,
                      tex->height/2-v1.y,
                      tex->width /2-v2.x,
                      tex->height/2-v2.y,
                      0xffff0000);
        }
    }
        // tex->line(tex->width /2-this->vertices[this->indices[i*3+0]].x,
        //           tex->height/2-this->vertices[this->indices[i*3+0]].y,
        //           tex->width /2-this->vertices[this->indices[i*3+1]].x,
        //           tex->height/2-this->vertices[this->indices[i*3+1]].y,
        //           0xffff0000);
        // tex->line(tex->width /2-this->vertices[this->indices[i*3+1]].x,
        //           tex->height/2-this->vertices[this->indices[i*3+1]].y,
        //           tex->width /2-this->vertices[this->indices[i*3+2]].x,
        //           tex->height/2-this->vertices[this->indices[i*3+2]].y,
        //           0xffff0000);
        // tex->line(tex->width /2-this->vertices[this->indices[i*3+2]].x,
        //           tex->height/2-this->vertices[this->indices[i*3+2]].y,
        //           tex->width /2-this->vertices[this->indices[i*3+0]].x,
        //           tex->height/2-this->vertices[this->indices[i*3+0]].y,
        //           0xffff0000);
    
}