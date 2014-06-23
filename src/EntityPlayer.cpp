#include "EntityPlayer.h"

EntityPlayer::EntityPlayer():EntityMesh()
{
	fire = false;
}

EntityPlayer::EntityPlayer( Mesh* mesh ):EntityMesh( mesh )
{
	fire = false;
}

EntityPlayer::EntityPlayer( Mesh* mesh, Texture* texture ):EntityMesh( mesh, texture )
{
	fire = false;
}

EntityPlayer::EntityPlayer( Mesh* mesh, Texture* texture, Vector3 color ):EntityMesh( mesh, texture, color )
{
	fire = false;
}

EntityPlayer::~EntityPlayer()
{
}

void EntityPlayer::init()
{
	fire = false;
	life = 10000;
	//startPostition = Vector3(this->model.m[12],this->model.m[12],this->model.m[12]
	startPosition = this->centerEntity;
}
void EntityPlayer::reinit()
{
	/*std::cout<<"startPosition "<<startPosition.x<<" : "<<startPosition.y<<" : "<<startPosition.z<<"  "<<std::endl;
	std::cout<<"this->centerEntity "<<this->centerEntity.x<<" : "<<this->centerEntity.y<<" : "<<this->centerEntity.z<<""<<std::endl;*/

	fire = false;
	life = 10000;
	this->model.setTranslation(startPosition.x,startPosition.y,startPosition.z);
	intercepted = false;
	/*std::cout<<"final "<<restartPostition.x<<" : "<<restartPostition.y<<" : "<<restartPostition.z<<"  "<<std::endl;
	std::cout<<"model "<<(this->model.m)[12]<<" : "<<(this->model.m)[13]<<" : "<<(this->model.m)[14]<<"  "<<std::endl;*/
}


void EntityPlayer::render()
{
	glDisable( GL_CULL_FACE );

	glPushMatrix();
	model.set();

	this->texture->bind();
		if(intercepted)
	{
		glColor3f(1,0,0);
		//intercepted = false;
	}
	else glColor3f(color.x, color.y, color.z);

	mesh->render();

	glPopMatrix();

	glEnable( GL_CULL_FACE );
}

void EntityPlayer::update( double elapsed_time)
{

	if(intercepted)
	{
		this->reinit();	
	}
	for(vecEntities::iterator it = childEntities.begin() ; it != childEntities.end(); ++it)
	{
		(*it)->update( elapsed_time );
	}
	centerEntity = Vector3(model.m[12], model.m[13], model.m[14]);
	if(fire)
	{
		Matrix44 start_poss = this->model;

		start_poss.traslateLocal(-2,0,-2);
		Vector3 start_pos = start_poss * Vector3();
		//start_pos.z += 2.00;
		//start_pos.y += 1.09;
		//start_pos.x -= 2.50;
		BulletManager::getInstance()->createBullet(start_pos,World::getInstance()->currentCamera->getLocalVector(Vector3(0,-1,-2000)),this->name);
		///start_pos.z -= 1.09;
		//start_pos.x += 5.00;
		start_poss.traslateLocal(+4,0,0);
		start_pos = start_poss * Vector3();
		BulletManager::getInstance()->createBullet(start_pos,World::getInstance()->currentCamera->getLocalVector(Vector3(0,-1,-2000)),this->name);
		fire = false;
	}
	mesh->collision_model->setTransform(getGlobalMatrix().m);

	if(has_particula)
	{
		Particula* p = EmisorParticulas::getInstance()->getParticula(particula);
		if(p != NULL){
			front = this->getGlobalMatrix().frontVector();
			//std::cout<<this->name<<"  :: "<<this->getGlobalMatrix().frontVector().x<<"  :: "<<this->getGlobalMatrix().frontVector().y<<"  :: "<<this->getGlobalMatrix().frontVector().z << std::endl;
			p->pos = this->centerEntity;
			p->vel = this->front;
		}
		else has_particula = 0;
	}

}

