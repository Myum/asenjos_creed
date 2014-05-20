#pragma once

#include <map>
#include "mesh.h"
#include "includes.h"

class MeshManager
{
	static std::map <std::string, Mesh*> meshes;

public:
	MeshManager();
	~MeshManager();


};

