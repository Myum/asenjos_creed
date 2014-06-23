#pragma once

#include "EntityMesh.h"
#include "TextureManager.h"

class Spitfire
	: public EntityMesh
{
public:
	Spitfire(Mesh* mesh, Texture* texture);
	~Spitfire();
	void update(double dt);
	void render();

	int speed;
	void reduce(int lvl);

	float life;
	TextureManager* tM;
};

