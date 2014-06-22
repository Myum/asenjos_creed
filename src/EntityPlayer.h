#pragma once

#include "EntityMesh.h"
#include "Bullet.h"
#include "World.h"

class EntityPlayer : public EntityMesh
{
public:
	EntityPlayer();
	EntityPlayer( Mesh* mesh );
	EntityPlayer( Mesh* mesh, Texture* texture );
	EntityPlayer( Mesh* mesh, Texture* texture, Vector3 color );
	~EntityPlayer();
	
	//inherited functions
	void update( double elapsed_time );
	void render();
	bool fire;



};