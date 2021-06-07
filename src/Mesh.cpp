#include "Mesh.h"

#include "Texture.h"
#include <cmath>
#include <iostream>

#include <chrono>

#include "math.hh"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

std::vector<std::string> split(const std::string& s, char seperator) {
   std::vector<std::string> output;
    std::string::size_type prev_pos = 0, pos = 0;
    while((pos = s.find(seperator, pos)) != std::string::npos) {
        std::string substring( s.substr(prev_pos, pos-prev_pos) );
        output.push_back(substring);
        prev_pos = ++pos;
    }
    output.push_back(s.substr(prev_pos, pos-prev_pos)); // Last word
    return output;
}

unsigned int int32(unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4) {
    return b4<<24 | b3<<16 | b2<<8 | b1<<0;
}

float float32(byte b1, byte b2, byte b3, byte b4) {
    float out = 0;
    // unsigned int* handle = ((unsigned int*)&out);
    *((unsigned int*)&out) = int32(b1, b2, b3, b4);
    return out;
    // return java.nio.ByteBuffer.wrap(new byte[]{b4, b3, b2, b1}).getFloat();
}

constexpr const int stl_header_length = 80;
Mesh loadSTL(const char* filename) {
    unsigned char* file;
    uint32_t fileLength;
    {
        std::ifstream infile(filename);
        infile.seekg(0, std::ios::end);//get length of file
        fileLength = infile.tellg();
        infile.seekg(0, std::ios::beg);
        file = new unsigned char[fileLength];
        infile.read((char*)file, fileLength);//read file
        infile.close();
    }

    unsigned int triNum = int32(file[stl_header_length], file[stl_header_length+1], file[stl_header_length+2], file[stl_header_length+3]);
    
    std::cout << "Loading binary STL file with <" << triNum << "> faces and <" << fileLength/100/10. << "> kb\n";
    
    // PVector[][] triangles = new PVector[triNum][3];
    Mesh::vec3* vertices = new Mesh::vec3[triNum*3];
    unsigned int* indices = new unsigned int[triNum*3];

    float mult = .9;
    for (unsigned int i=0; i<triNum; i++) {
        int off = stl_header_length+4 + i*(3*4 + 3*3*4 + 1*2) + 3*4;
        
        vertices[i*3+0].x = float32(file[off],   file[off+1],   file[off+2],   file[off+3])*mult;
        vertices[i*3+0].y = float32(file[off+4], file[off+4+1], file[off+4+2], file[off+4+3])*mult;
        vertices[i*3+0].z = float32(file[off+8], file[off+8+1], file[off+8+2], file[off+8+3])*mult;
        off += 3*4;
        vertices[i*3+2].x = float32(file[off],   file[off+1],   file[off+2],   file[off+3])*mult;
        vertices[i*3+2].y = float32(file[off+4], file[off+4+1], file[off+4+2], file[off+4+3])*mult;
        vertices[i*3+2].z = float32(file[off+8], file[off+8+1], file[off+8+2], file[off+8+3])*mult;
        off += 3*4;
        vertices[i*3+1].x = float32(file[off],   file[off+1],   file[off+2],   file[off+3])*mult;
        vertices[i*3+1].y = float32(file[off+4], file[off+4+1], file[off+4+2], file[off+4+3])*mult;
        vertices[i*3+1].z = float32(file[off+8], file[off+8+1], file[off+8+2], file[off+8+3])*mult;

        indices[i*3] = i*3;
        indices[i*3+1] = i*3+1;
        indices[i*3+2] = i*3+2;
    }
    
   return {vertices, indices, triNum};
//    return {nullptr, nullptr, 0};
}

Mesh loadOBJ(const char* filename) {
    std::ifstream file;
    std::vector<std::string> verts, faces;
    file.open(filename);
    if (file.is_open()) {
        std::string line;
        while (getline(file,line)) {
            if(line.starts_with("v "))
                verts.push_back(line);
            if(line.starts_with("f "))
                faces.push_back(line);
        }
        file.close();

        int vi = 0;
        Mesh::vec3* vertices = new Mesh::vec3[verts.size()];
        for(std::string vert_s : verts) {
            Mesh::vec3 vert = {0, 0, 0};
            vert.x = std::stof(split(vert_s, ' ')[1]) * 50;
            vert.y = std::stof(split(vert_s, ' ')[2]) * 50;
            vert.z = std::stof(split(vert_s, ' ')[3]) * 50;
            vertices[vi++] = vert;
        }

        unsigned int numTris = faces.size();

        int fi = 0;
        unsigned int* indices = new unsigned int[numTris*3];
        for(std::string face_s : faces) {
            indices[fi++] = std::stoi(split(split(face_s, ' ')[1], '/')[0]);
            indices[fi++] = std::stoi(split(split(face_s, ' ')[3], '/')[0]);
            indices[fi++] = std::stoi(split(split(face_s, ' ')[2], '/')[0]);
        }
        return {vertices, indices, numTris};
    }
    return {nullptr, nullptr, 0};
}

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

    int64_t m = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    a += m/1000.;
    v.x = cos(a) * mag;
    v.z = sin(a) * mag;
}

void rotX(Mesh::vec3& v) {
    double a = atan2(v.z, v.y);
    float mag = sqrt(v.z*v.z + v.y*v.y);

    // int64_t m = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    // a -= m/1000.*2;
    a += 3.1415926535;
    v.z = cos(a) * mag;
    v.y = sin(a) * mag;
}


#define PI 3.1415926535
#define TWO_PI (PI*2)
boolean cw(const Mesh::vec3& p1, const Mesh::vec3& p2, const Mesh::vec3& p3) {
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
            rotX(tri[j]);
            rotY(tri[j]);
            tri[j].y += 70;
            tri[j].z += 150;
            tri[j].x = tri[j].x / tri[j].z * 300;
            tri[j].y = tri[j].y / tri[j].z * 300;
        }

        if(cw(tri[0], tri[1], tri[2]))
            continue;
        
        for(unsigned int line=0; line<3; line++) {
            vec3 v1 = tri[line];
            vec3 v2 = tri[(line+1)%3];
            
            tex->line(tex->width /2-v1.x,
                      tex->height/2-v1.y,
                      tex->width /2-v2.x,
                      tex->height/2-v2.y,
                      0xffff0000);
        }
    }
}