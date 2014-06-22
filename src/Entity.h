#pragma once

#include "framework.h"
#include "includes.h"
#include "utils.h"
#include <string>
#include <map>
#include "camera.h"

class Entity
{
public:
	typedef std::vector<Entity*> vecEntities;
	typedef std::map<std::string, Entity*> mapEntities;
	typedef std::pair<std::string, Entity*> itemMapEntity;

	//constructors
	Entity();
	Entity( std::string name );
	virtual ~Entity();

	//local variables
	std::string className;
	std::string name;
	Matrix44 model;

	//sets of children entities
	vecEntities childEntities;
	mapEntities childNamedEntities;
	
	void killEntity(std::string name);

	//graphic flags
	bool has_alpha;
	bool has_blend;

	//parent entity
	Entity* parent;

	//functions
	virtual void init();
	virtual void render();
	virtual void update( double elapsed_time );
	virtual void addEntityChildren( Entity* );
	virtual Entity* getChildren( std::string name );
	virtual Entity* getParent();

	float distanceToCamera(Vector3 eye);

	Matrix44 getGlobalMatrix();

};

