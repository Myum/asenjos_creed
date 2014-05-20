#pragma once

#include "Entity.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "Camera.h"

class EntityMesh : public Entity
{
public:
	EntityMesh();
	EntityMesh( Mesh* mesh );
	EntityMesh( Mesh* mesh, Texture* texture );
	EntityMesh( Mesh* mesh, Texture* texture, Vector3 color );
	~EntityMesh();

	Mesh* low_mesh;
	Mesh* high_mesh;
	Mesh* mesh;
	Texture* texture;
	Vector3 color;

	//inherited functions
	void render();
	void update( double elapsed_time );

	//new parameters
	Camera* camera;
	//center
	Vector3 centerEntity;
	Vector3 position;

	bool intercepted;

	bool rayCollision(Vector3 ray_origin, Vector3 ray_direction);
};

