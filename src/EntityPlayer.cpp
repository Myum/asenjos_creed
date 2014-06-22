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

void EntityPlayer::render()
{
	glDisable( GL_CULL_FACE );

	glPushMatrix();
	model.set();

	this->texture->bind();
	glColor3f(color.x, color.y, color.z);

	mesh->render();

	glPopMatrix();

	glEnable( GL_CULL_FACE );
}

void EntityPlayer::update( double elapsed_time)
{
	//std::cout << "entityplayer: "<<std::endl;

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

