/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	The Mesh contains the info about how to render a mesh and also how to parse it from a file.
*/

#ifndef MESH_H
#define MESH_H

#include <vector>
#include "framework.h"
#include "coldet\coldet.h"

class Mesh
{
public:
	std::vector< Vector3 > vertices; //here we store the vertices
	std::vector< Vector3 > normals;	 //here we store the normals
	std::vector< Vector2 > uvs;	 //here we store the texture coordinates


	int primitive; //used to tell which primitive to use when rendering (quad, points, lines)
	Vector3 center;
	float radius;

	Mesh();
	void clear();
	void render();

	void createPlane(float size);
	bool loadASE(const char *meshFile);
	bool createBin(const char* meshFileBin);
	bool loadASEbin(const char* meshFileBin);

	Vector3 centerBB,halfsize,maxBB,minBB;

	CollisionModel3D* collision_model;
	void coldetInit();

};

#endif