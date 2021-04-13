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

	void MeshObject::addTriangleMesh(TriangleMesh* Mesh);
	void MeshObject::loadAddTriangleMesh(const char* filename);
	void MeshObject::load(const char* filename);
	void MeshObject::load_tex(const char* filename);

	void MeshObject::draw();
	void MeshObject::setPosition(float x, float y, float z);

private:
	vector<TriangleMesh> triangleMeshes;
	Vec3f position;

};

