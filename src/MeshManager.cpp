#include "MeshManager.h"

MeshManager::MeshManager()
{

}

MeshManager::~MeshManager()
{

}

Mesh* MeshManager::getMesh(char* meshFile)
{
	//return NULL;
	std::map< char*, Mesh* >::iterator it = this->meshes.find(meshFile);

	if(it == this->meshes.end())
	{
		Mesh* mesh = new Mesh();
		mesh->loadASE(meshFile);
		this->meshes.insert( std::pair< char* , Mesh* >(meshFile, mesh) );

		return mesh;
	}
	else
	{
		return it->second;
	}
}