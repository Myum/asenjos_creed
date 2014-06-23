#pragma once

#include "includes.h"
#include "utils.h"
#include "Entity.h"
#include "Camera.h"
#include "Bullet.h"
#include "EmisorParticulas.h"
#include <map>
#include "AIController.h"

class World: public Entity
{
private:
	static World* instance;
	
	std::vector<AIController*> IA;
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

	void addIA(Entity* toControll);
	void killEntity(std::string name);


	Entity* player;
};