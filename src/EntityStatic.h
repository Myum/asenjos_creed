#pragma once

#include "EntityMesh.h"

class EntityStatic : public EntityMesh
{
public:
	EntityStatic();
	EntityStatic( Mesh* mesh );
	EntityStatic( Mesh* mesh, Texture* texture );
	EntityStatic( Mesh* mesh, Texture* texture, Vector3 color );
	~EntityStatic();
	
	//inherited functions
	void render();

};