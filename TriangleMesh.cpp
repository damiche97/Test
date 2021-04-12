// ========================================================================= //
// Authors: Roman Getto, Matthias Bein                                       //
// mailto:roman.getto@gris.informatik.tu-darmstadt.de                        //
//                                                                           //
// GRIS - Graphisch Interaktive Systeme                                      //
// Technische Universität Darmstadt                                          //
// Fraunhoferstrasse 5                                                       //
// D-64283 Darmstadt, Germany                                                //
//                                                                           //
// Content: Simple class for reading and rendering triangle meshes            //
// ========================================================================== //

#include <iostream>
#include <fstream>
#include <float.h>
#include <GL/glut.h>
#include "TriangleMesh.h"

#include <cassert>

void TriangleMesh::calculateNormals() {
  normals.resize(vertices.size());
  // TODO: calculate normals for each vertex
  for (Triangle const& t : triangles) {
      // retrieve immutable references to each vertex of the triangle
      Vec3f const& p1 = vertices[t[0]];
      Vec3f const& p2 = vertices[t[1]];
      Vec3f const& p3 = vertices[t[2]];

      const Vec3f edge1 = p1 - p2;
      const Vec3f edge2 = p1 - p3;

      const Vec3f normal = edge1 ^ edge2;
      normals[t[0]] += normal;
      normals[t[1]] += normal;
      normals[t[2]] += normal;
  }

  // normalize normals
  for (Normals::iterator nit=normals.begin(); nit!=normals.end(); ++nit) {
     //the normalize() function returns a boolean which can be used if you want to check for erroneous normals
	 (*nit).normalize();
   }
}

// ===============================
// === CONSTRUCTOR, DESTRUCTOR ===
// ===============================

TriangleMesh::TriangleMesh() {
  clear();
}

TriangleMesh::~TriangleMesh() {
  clear();
}

void TriangleMesh::clear() {
  // clear mesh data
  vertices.clear();
  triangles.clear();
  normals.clear();
}

// ================
// === RAW DATA ===
// ================

vector<Vec3f>& TriangleMesh::getPoints() {
  return vertices;
}
vector<Vec3i>& TriangleMesh::getTriangles() {
	return triangles;
}

vector<Vec3f>& TriangleMesh::getNormals() {
  return normals;
}

void TriangleMesh::flipNormals() {
  for (Normals::iterator it = normals.begin(); it != normals.end(); ++it) {
    (*it) *= -1.0;
  }
}

// =================
// === LOAD MESH ===
// =================

void TriangleMesh::loadLSA(const char* filename) {  
  std::ifstream in(filename);
  if (!in.is_open()) {
    cout << "loadLSA: can not open " << filename << endl;
    return;
  }
  char s[256];
  in >> s;
  // first word: LSA
  if (!(s[0] == 'L' && s[1] == 'S' && s[2] == 'A')) return;
  // get number of vertices nv, faces nf, edges ne and baseline distance
  int nv, nf, ne;
  float baseline;
  in >> nv;
  in >> nf;
  in >> ne;
  in >> baseline;
  if (nv <= 0 || nf <= 0) return;
  // clear any existing mesh
  clear();
  // read vertices  
  vertices.resize(nv);
  // TODO: read alpha, beta, gamma for each vertex and calculate verticex coordinates

// read triangles
triangles.resize(nf);
// TODO: read all triangles from the file

// calculate normals
calculateNormals();
}

void TriangleMesh::loadOFF(const char* filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        cout << "loadOFF: can not find " << filename << endl;
        return;
    }
    char s[256];
    in >> s;
    // first word: OFF
    if (!(s[0] == 'O' && s[1] == 'F' && s[2] == 'F')) return;
    // get number of vertices nv, faces nf, edges ne and baseline distance
    int nv, nf, ne;
    in >> nv;
    in >> nf;
    in >> ne;
    if (nv <= 0 || nf <= 0) return;
    // clear any existing mesh
    clear();
    // read vertices  
    vertices.resize(nv);
    // TODO: read all vertices from the file
    float vx, vy, vz;
    for (std::size_t i = 0; i < nv; i++) {
        in >> vx;
        in >> vy;
        in >> vz;
        vertices[i] = Vec3f(vx, vy, vz);
        //vertices.push_back(Vertex{ vx, vy, vz }); // store them in vector
    }

    // read triangles
    triangles.resize(nf);
    // TODO: read triangles from the file
    int num_vert;
    for (std::size_t i = 0; i < nf; i++) {
        in >> num_vert;
        assert(num_vert == 3);

        in >> vx;
        in >> vy;
        in >> vz;
        triangles[i] = Vec3i(vx, vy, vz);
    }

    // calculate normals
    calculateNormals();
}

void TriangleMesh::loadOBJ(const char* filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cout << "loadOBJ: can not find " << filename << endl;
        return;
    }
    while (in) {
        string block;
        in >> block;
        if (block == "v") {
            // vertices.resize(vertices.size() + 1);
            float vx, vy, vz;
            in >> vx;
            in >> vy;
            in >> vz;
            vertices.push_back(Vertex{ vx,vy,vz });
            //std::cout << "vertex detected" << endl;
        }
        else if (block == "vn") {
            float nx, ny, nz;
            in >> nx;
            in >> ny;
            in >> nz;
            //vertices.push_back(Vertex{ nx,ny,nz });
            // muss überarbeitet werden
        }
        else if (block == "f") {
            //string v1, v2, v3, vt1, vt2, vt3, vn1, vn2, vn3;
            string v1, v2, v3, vt1, vt2, vt3, vn1, vn2, vn3;// v4, vt4, vn4;
            //in >> block;
            //char  s[256];
            //std::stringstream ss(block);
            std::getline(in, v1, '/');
            std::getline(in, vt1, '/');
            std::getline(in, vn1, ' ');

            std::getline(in, v2, '/');
            std::getline(in, vt2, '/');
            std::getline(in, vn2, ' ');

            std::getline(in, v3, '/');
            std::getline(in, vt3, '/');
            std::getline(in, vn3);

            //std::getline(in, v4, '/');
            //std::getline(in, vt4, '/');
            //std::getline(in, vn4);
            
            triangles.push_back(Triangle{ std::stoi(v1) - 1,std::stoi(v2) - 1,std::stoi(v3) - 1 });
            //triangles.push_back(Triangle{ std::stoi(v3) - 1,std::stoi(v4) - 1,std::stoi(v1) - 1 });
            //in >> s;
            
        }
    }
    calculateNormals();
}

// ==============
// === RENDER ===
// ==============

void TriangleMesh::draw() {
  if (triangles.size() == 0) return;
  // TODO: draw triangles with immediate mode
  glBegin(GL_TRIANGLES);
  for (std::size_t i = 0; i < triangles.size(); i++) {
      glNormal3f(normals[triangles[i].x].x, normals[triangles[i].x].y, normals[triangles[i].x].z);
      glVertex3f(vertices[triangles[i].x].x, vertices[triangles[i].x].y, vertices[triangles[i].x].z);
     
      glNormal3f(normals[triangles[i].y].x, normals[triangles[i].y].y, normals[triangles[i].y].z);
      glVertex3f(vertices[triangles[i].y].x, vertices[triangles[i].y].y, vertices[triangles[i].y].z);
     
      glNormal3f(normals[triangles[i].z].x, normals[triangles[i].z].y, normals[triangles[i].z].z);
      glVertex3f(vertices[triangles[i].z].x, vertices[triangles[i].z].y, vertices[triangles[i].z].z);
  }
  glEnd();
}