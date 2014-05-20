#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include <map>
#include "mesh.h"
#include "includes.h"

class MeshManager
{
	std::map< char*, Mesh*> meshes;

public:
	MeshManager();
	~MeshManager();

	Mesh* getMesh( char* );
};

#endif