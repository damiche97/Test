#pragma once

#include <vector>
#include <Vec3.h>
#include <GL/glut.h>
#include "TriangleMesh.h"

using namespace std;

class MeshObject
{
public:
	MeshObject();
	MeshObject(float x, float y, float z);
	~MeshObject();

	void addTriangleMesh(TriangleMesh* Mesh);
	void loadAddTriangleMesh(const char* filename);
	void load(const char* filename);
	void load_tex(const char* filename);

	void draw();
	void setPosition(float x, float y, float z);

private:
	vector<TriangleMesh> triangleMeshes;
	Vec3f position;

};

