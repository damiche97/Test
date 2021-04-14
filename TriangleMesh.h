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
#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include <vector>
#include "Vec3.h"
#include <GL/glut.h>

#define M_PI 3.14159265358979f

using namespace std;

class TriangleMesh  {

private:

  // typedefs for data
  typedef Vec3i Triangle;
  typedef Vec3f Normal;
  typedef Vec3f Vertex;
  typedef vector<Triangle> Triangles;
  typedef vector<Normal> Normals;
  typedef vector<Vertex> Vertices;  
  //typedef vector<pair<float, float>> Textures;
  struct Tex2D { float u, v; };
  typedef vector<Tex2D> Textures;
  typedef vector<Vec3i> TriTextures;

  // data of TriangleMesh
  Vertices vertices;
  Normals normals;
  Triangles triangles;
  //avilable texture points
  Textures textures;
  // Texture indices used for each triangle
  TriTextures triTextures;
  unsigned int textureID;
  unsigned int drawMode;
  // Local Position translation of triangle mesh
  Vec3f position;

  vector<GLfloat> global_ambient; // = { 0.1f, 0.1f, 0.1f, 1.0f };
  vector<GLfloat> ambientLight; // = { 0.1f, 0.1f, 0.1f, 1.0f };
  vector<GLfloat> diffuseLight; // = { 1.0f, 1.0f, 1.0f, 1.0f };
  vector<GLfloat> specularLight; // = { 1.0f, 1.0f, 1.0f, 1.0f };
  vector<GLfloat> specularLightMaterial; // [] = { 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat shininessMaterial;
  GLfloat shininess;

  // private methods
  void calculateNormals();

public:

  // ===============================
  // === CONSTRUCTOR, DESTRUCTOR ===
  // ===============================

  TriangleMesh();
  ~TriangleMesh();

  // clears all data, sets defaults
  void clear();

  // ================
  // === RAW DATA ===
  // ================

  // get raw data references
  vector<Vec3f>& getPoints();
  vector<Vec3i>& getTriangles();
  vector<Vec3f>& getNormals();

  // flip all normals
  void flipNormals();

  void TriangleMesh::setPosition(float x, float y, float z);
  void switchDrawMode();

  // =================
  // === LOAD MESH ===
  // =================

  // read from an LSA file. also calculates normals.
  void loadLSA(const char* filename);

  // read from an OFF file. also calculates normals.
  void loadOFF(const char* filename);

  // read OBJ file
  void loadOBJ(const char* filename);

  void TriangleMesh::loadTexture(const char* filename);

  // ==============
  // === RENDER ===
  // ==============
  
  // draw mesh with set transformation
  void draw_settings();
  void draw();
  void drawImmediate();
  void drawArray();


};


#endif

