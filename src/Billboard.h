#pragma once
#include "Entity.h"
#include "texture.h"
#include "World.h"
#include "camera.h"
#include <algorithm>    // std::sort

class PointCloud
{
public: 
	Vector3 pos;
	float size;
	int id;
	PointCloud(int id, Vector3 pos, float size) { this->id = id; this->pos = pos; this->size = size; }
};


class Billboard :
	public Entity
{
public:
	Billboard(Texture* tex);
	~Billboard();

	std::vector<PointCloud*> points;

	float size;

	Vector3 top;
	Vector3 right;
	void render();
	Texture* texture;

};

