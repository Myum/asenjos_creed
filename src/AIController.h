#pragma once
#include "Entity.h"

class AIController
{
public:
	AIController();
	~AIController();

	Entity* controlled;
	Vector3 targetPos;

	void update(double dt);

	float pitch,roll;
};

