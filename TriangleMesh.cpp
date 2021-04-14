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
// #include <GL/glut.h>
#include "TriangleMesh.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <cassert>


// ===============================
// === CONSTRUCTOR, DESTRUCTOR ===
// ===============================


TriangleMesh::TriangleMesh() {
    clear();
    // set lighting and material
    /*
    GLfloat global_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat shininess;
    */
    position = { 0.f,0.f,0.f };
    global_ambient = { 0.1f, 0.1f, 0.1f, 0.1f };
    ambientLight = { 0.1f, 0.1f, 0.1f, 0.1f };
    diffuseLight = { 1.0f, 1.0f, 1.0f, 1.0f };
    specularLight = { 1.0f, 1.0f, 1.0f, 1.0f };
    shininess = 128.0f;
    specularLightMaterial = { 1.0f, 1.0f, 1.0f, 1.0f };
    shininessMaterial = 128.0f;
    drawMode = 1;
}

TriangleMesh::~TriangleMesh() {
  clear();
}

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

void TriangleMesh::setPosition(float x, float y, float z) {
    position.x = x;
    position.y = y;
    position.z = z;
}

void TriangleMesh::switchDrawMode()
{
    drawMode += 1;
    if (drawMode >= 2)
    {
        drawMode = 0;
    }
    std::cout << "drawMode switched to " << drawMode << std::endl;
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
    // Storing the local possible texture coordinates
    /*struct Tex2D
    {
        float u, v;
    };*/
    vector<Tex2D> localTexCoords;
    vector<Vec3f> localVertices;
    vector<Vec3f> localNormals;
	
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
            localVertices.push_back(Vertex{ vx,vy,vz });
            //localVertices.emplace_back(Vertex{ vx,vy,vz });
            
            //std::cout << "vertex detected" << endl;
        }
        else if (block == "vn") {
            float nx, ny, nz;
            in >> nx;
            in >> ny;
            in >> nz;
            localNormals.push_back(Vertex{ nx,ny,nz });
            //vertices.push_back(Vertex{ nx,ny,nz });
            // muss überarbeitet werden
        }
        else if (block == "vt") {
            float u, v;
            in >> u;
            in >> v;
            localTexCoords.push_back(Tex2D{ u,v });
            //textures.push_back({ u, v });
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
        	
            vertices.push_back(localVertices[std::stoi(v1) - 1]);
            vertices.push_back(localVertices[std::stoi(v2) - 1]);
            vertices.push_back(localVertices[std::stoi(v3) - 1]);
            normals.push_back(localNormals[std::stoi(vn1) - 1]);
            normals.push_back(localNormals[std::stoi(vn2) - 1]);
            normals.push_back(localNormals[std::stoi(vn3) - 1]);
        	
            textures.push_back(localTexCoords[std::stoi(vt1) - 1]);
            textures.push_back(localTexCoords[std::stoi(vt2) - 1]);
            textures.push_back(localTexCoords[std::stoi(vt3) - 1]);
        	
            //triangles.push_back(Triangle{ std::stoi(v1) - 1,std::stoi(v2) - 1,std::stoi(v3) - 1 });
            triangles.push_back(Triangle{ (int)vertices.size() - 3, (int)vertices.size() - 2, (int)vertices.size() - 1 });

        	//triTextures.push_back(Vec3i{ std::stoi(vt1) - 1,std::stoi(vt2) - 1,std::stoi(vt3) - 1 });
            
            //triangles.push_back(Triangle{ std::stoi(v3) - 1,std::stoi(v4) - 1,std::stoi(v1) - 1 });
            //in >> s;
            
        }
    }
    calculateNormals();
}

void TriangleMesh::loadTexture(const char* filename) {
    //unsigned int texture;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        //glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}
// ==============
// === RENDER ===
// ==============

void TriangleMesh::draw_settings() {
    // enable depth buffer
    glEnable(GL_DEPTH_TEST);
    // set shading model
    glShadeModel(GL_SMOOTH);
    // Lightning settings
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, &global_ambient[0]);
    glLightfv(GL_LIGHT0, GL_AMBIENT, &ambientLight[0]);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, &diffuseLight[0]);
    glLightfv(GL_LIGHT0, GL_SPECULAR, &specularLight[0]);
    glLightf(GL_LIGHT0, GL_SHININESS, shininess);

    glEnable(GL_LIGHT0);
    // enable use of glColor instead of glMaterial for ambient and diffuse property
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    // white shiny specular highlights
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininessMaterial);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &specularLightMaterial[0]);
}

void TriangleMesh::draw() {
    draw_settings();
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    switch (drawMode)
    {
    case 0:
        drawImmediate();
    case 1:
        drawArray();
    default:
        break;
    }
    glPopMatrix();
}

void TriangleMesh::drawImmediate() {
  if (triangles.size() == 0) return;
  // Enable Texture
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glBegin(GL_TRIANGLES);
  for (std::size_t i = 0; i < triangles.size(); i++) {
      glNormal3f(normals[3*i].x, normals[3*i].y, normals[3*i].z);
      glTexCoord2f(textures[3*i].u, textures[3*i].v);
      glVertex3f(vertices[triangles[i].x].x, vertices[triangles[i].x].y, vertices[triangles[i].x].z);
     
      glNormal3f(normals[3*i+1].x, normals[3*i+1].y, normals[3*i+1].z);
  	  glTexCoord2f(textures[3 * i + 1].u, textures[3 * i + 1].v);
      glVertex3f(vertices[triangles[i].y].x, vertices[triangles[i].y].y, vertices[triangles[i].y].z);
     
      glNormal3f(normals[3*i + 2].x, normals[3*i + 2].y, normals[3*i + 2].z);
  	  glTexCoord2f(textures[3 * i + 2].u, textures[3 * i + 2].v);
      glVertex3f(vertices[triangles[i].z].x, vertices[triangles[i].z].y, vertices[triangles[i].z].z);
  }
  glEnd();
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);
}

void TriangleMesh::drawArray() {
    // Enabling Drawing Arrays
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // Pointers to the vertices and normals data
    /*glVertexPointer(3, GL_FLOAT, sizeof(Vertex), vertices.data());
    glNormalPointer(GL_FLOAT, sizeof(Normal), normals.data());
    glTexCoordPointer(2, GL_FLOAT, sizeof(Tex2D), textures.data());*/
    glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
    glNormalPointer(GL_FLOAT, 0, &normals[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &textures[0]); 
    // drawing the elements
    glDrawElements(GL_TRIANGLES, triangles.size()*3, GL_UNSIGNED_INT, &triangles[0]);

    // We disable normal and vertex arrays again
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisable(GL_TEXTURE_2D);
}