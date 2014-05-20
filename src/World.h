#pragma once

#include "includes.h"
#include "utils.h"
#include "Entity.h"
#include "Camera.h"
#include "Bullet.h"
#include <map>

class World: public Entity
{
private:
	static World* instance;
	
	World();

public:
	static World* getInstance()
	{
		if(instance == NULL)
		{
			instance = new World();
		}
		return instance;
	}

	//new parameters
	Camera* currentCamera;

	//inherited functions
	void render();
	void update( double elapsed_time );


};