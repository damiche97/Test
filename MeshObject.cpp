#include "MeshObject.h"
#include <vector>


MeshObject::MeshObject()
{
	position.x = 0;
	position.y = 0;
	position.z = 0;
}
MeshObject::MeshObject(float x, float y, float z) {
	position.x = x;
	position.y = y;
	position.z = z;
}

MeshObject::~MeshObject()
{
}

void MeshObject::addTriangleMesh(TriangleMesh* Mesh)
{
}

void MeshObject::loadAddTriangleMesh(const char* filename)
{
	TriangleMesh a = TriangleMesh();
	a.loadOBJ(filename);
	triangleMeshes.push_back(a);
	//triangleMeshes[triangleMeshes.size() - 1].loadOBJ(filename);
}

void MeshObject::load(const char* filename)
{

}

void MeshObject::load_tex(const char* filename)
{
}

void MeshObject::draw()
{
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	for (TriangleMesh& t : triangleMeshes) {
		t.draw();
	}
	glPopMatrix();
}

void MeshObject::setPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}
