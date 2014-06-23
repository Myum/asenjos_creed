#include "AIController.h"
//#include "EntityMesh.h"
#include "Spitfire.h"

AIController::AIController()
{
	this->controlled = NULL;
	this->pitch=this->roll=0;
}


AIController::~AIController()
{
}

void AIController::update(double dt)
{
	if(pitch < -10) pitch = -10;
	if(pitch > 10 ) pitch = 10;
	if(this->controlled != NULL ){

		Spitfire* enemy = (Spitfire*)this->controlled;

		Matrix44 mod = enemy->getGlobalMatrix();
		Vector3 pos = Vector3(mod.m[12], mod.m[13], mod.m[14]);
		Vector3 front = enemy->getGlobalMatrix().frontVector();
		Vector3 right = enemy->getGlobalMatrix().rightVector();

		Vector3 to_target = (targetPos - pos);

		if(to_target.length() < 250 )
		{
			if(to_target.length() < 130 )
			{
				if(to_target.length() < 35 )
					enemy->reduce(3);
				else
					enemy->reduce(2);
			}
			else
				enemy->reduce(1);
		}
		to_target.normalize();

		float angle = 1 - front.dot(to_target);
		float dir = right.dot(to_target);

		Vector3 axis = front.cross( to_target );



		/*	if(dir < -0.02) dir = -1;
		else if(dir>0.02) dir= 1;
		else dir=0;
		std::cout<<"angle: "<<angle<<std::endl;
		if(angle < 0.2 && angle > 0) angle = 0.3;
		//if(angle > -0.2 && angle < 0) angle = - 0.2;



		this->controlled->model.rotateLocal(dt*dir, Vector3(0,-1,0));
		/*this->controlled->model.rotateLocal(dt*angle,Vector3(axis.x,-axis.y,axis.z));
		this->controlled->model.rotateLocal(dt*angle,Vector3(axis.x,-axis.y,axis.z));
		this->controlled->model.rotateLocal(dt*angle,Vector3(axis.x,-axis.y,axis.z));
		this->controlled->model.rotateLocal(dt*angle,Vector3(axis.x,-axis.y,axis.z));*/
		//if(angle>0.02)this->controlled->model.rotateLocal(dt*angle,Vector3(axis.x,-axis.y,axis.z));

		enemy->model.rotateLocal(dt*dir, Vector3(0,-1,0));
		enemy->model.rotateLocal(dt*angle, Vector3(1,0,0));
		//	enemy->model.traslateLocal(0,0,dt*5);

	}
}