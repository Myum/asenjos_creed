#include "Spitfire.h"


Spitfire::Spitfire( Mesh* mesh, Texture* texture):EntityMesh( mesh, texture)
{
	speed = /*85*/5;
	life = 10000;
	has_particula= 0;
}

Spitfire::~Spitfire()
{
}

void Spitfire::update(double dt)
{
	if(intercepted)
	{
		life -= 10;
		if(!has_particula)
		{
			Particula * p = new Particula(centerEntity,front,11,5,0.3,tM->getTexture("assets/textures/smoke_alpha.tga"));
			EmisorParticulas::getInstance()->addParticula(p);

			this->particula = p->id;
			has_particula = 1;
		}

	}
	if(life > 0 )
	{
		for(vecEntities::iterator it = childEntities.begin() ; it != childEntities.end(); ++it)
		{
			(*it)->update( dt );
		}

		Vector3 frontt = this->getGlobalMatrix().frontVector();

		centerEntity = Vector3(getGlobalMatrix().m[12], getGlobalMatrix().m[13], getGlobalMatrix().m[14]);

		if(has_particula)
		{
			Particula* p = EmisorParticulas::getInstance()->getParticula(particula);
			if(p != NULL)
			{
				
				//front = this->getGlobalMatrix().frontVector();

				//std::cout<<"                "<<front.y<<std::endl;
				p->pos = this->centerEntity;
				p->vel = frontt;
			}
			else has_particula = 0;
		}
		this->front = frontt * dt * speed;
		this->model.traslate(front.x,front.y,front.z);
		this->mesh->collision_model->setTransform(getGlobalMatrix().m);
	}
}

void Spitfire::render()
{
	glPushMatrix();
	model.set();

	/*glBegin(GL_LINES);
	glColor4f(1,1,0,0);
	Vector3 pos = Vector3(this->getGlobalMatrix().m[12], this->getGlobalMatrix().m[13], this->getGlobalMatrix().m[14]);
	Vector3 end = this->getGlobalMatrix().frontVector();
	glVertex3f(pos.x, pos.y, pos.z);
	end = (end - pos).normalize()*5;
	glVertex3f( end.x, end.y, end.z);
	glEnd();*/

	this->texture->bind();
	glColor3f(color.x, color.y, color.z);
	if(intercepted)
	{
		glColor3f(1,0,0);
		intercepted = false;
	}

	mesh->render();
	this->texture->unbind();

	glPopMatrix();
	if(has_particula)EmisorParticulas::getInstance()->render();
}

void Spitfire::reduce(int lvl)
{
	//std::cout<<"REDUCED"<<std::endl;
	if(lvl == 1)speed = 75;
	if(lvl == 2)speed = 55;
	if(lvl == 3)speed = 30;

}