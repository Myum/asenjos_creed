
#include "EmisorParticulas.h"


EmisorParticulas* EmisorParticulas::instance = NULL;
int Particula::sharedId = 0;

Particula::Particula(Vector3 pos, Vector3 vel, int TTL, float size, float alpha, Texture* texture)
{
	this->pos = pos;this->vel = vel;this->TTL = TTL;this->size = size;this->alpha = alpha; this->texture = texture;
	this->fixedTTL = TTL;
	id = sharedId;
	sharedId = sharedId+1;
}
void Particula::addParticulaChildren(Particula* p)
{
	surtidor.push_back(p);
}


EmisorParticulas::EmisorParticulas()
{
	numParticulasPorFuente = 30;
}


EmisorParticulas::~EmisorParticulas()
{
}

bool EmisorParticulas::addParticula(Particula* p)
{
	std::map<std::string, int>::iterator it = textures.find( p->texture->filename );
	if( it != textures.end() )
	{
		int number = (it)->second;
		(it)->second = number+1;
		p->parent = NULL;
		for(int i = 0; i < numParticulasPorFuente; ++i)
		{
			Particula * child = new Particula(p->pos, p->vel,( (p->TTL) /100000) / (1 + rand()%100),20,p->alpha, p->texture);
			/*child->vel.x = child->vel.x * (rand()%155);
			child->vel.y = child->vel.y * (rand()%150);
			child->vel.z = child->vel.z * (rand()%150);*/
			child->parent = p;
			p->addParticulaChildren(child);
		}

		particulas.push_back(p);
	}
	else
	{
		textures.insert(std::pair<std::string, int>(p->texture->filename,1));
		p->parent = NULL;
		for(int i = 0; i < numParticulasPorFuente; ++i)
		{
			Particula * child = new Particula(p->pos, p->vel,( (p->TTL) /100000) / (1 + rand()%100),20,p->alpha, p->texture);
			/*child->vel.x = child->vel.x * (rand()%55);
			child->vel.y = child->vel.y * (rand()%50);
			child->vel.z = child->vel.z * (rand()%50);*/
			child->parent = p;
			p->addParticulaChildren(child);
		}
		particulas.push_back(p);
	}
	return true;
}

Particula* EmisorParticulas::getParticula(int id)
{
	for(std::vector<Particula*>::iterator it = particulas.begin(); it != particulas.end(); ++it)
	{
		if((*it)->id == id)
			return (*it);
	}
	//if element isn't in the map
	return NULL;
}


bool isCloserToCamera(Particula* a, Particula* b)
{
	return ( World::getInstance()->currentCamera->eye.distance(a->pos) > World::getInstance()->currentCamera->eye.distance(b->pos) );
}

void EmisorParticulas::killParticula()
{
	for(int i = 0; i < this->particulasToErase.size(); ++i)
	{
		int eraseId = this->particulasToErase[i];

		for(std::vector<Particula*>::iterator it = particulas.begin(); it != particulas.end(); ++it)
		{
			if( (*it)->id == eraseId ){
				particulas.erase(it);
				break;
			}
		}
	}
}
void EmisorParticulas::killParticula(int id)
{
	for(std::vector<Particula*>::iterator it = particulas.begin(); it != particulas.end(); ++it)
	{
		if( (*it)->id == id ){
			particulas.erase(it);
			break;
		}
	}
}
void EmisorParticulas::update(double dt)
{
	killParticula();

	for(std::vector<Particula*>::iterator it = particulas.begin(); it != particulas.end(); ++it)
	{
		if((*it)->TTL > 0 )
		{
			(*it)->TTL = (*it)->TTL - dt;
			std::sort( (*it)->surtidor.begin(), (*it)->surtidor.end(), isCloserToCamera );
			for(std::vector<Particula*>::iterator itSurtidor = (*it)->surtidor.begin(); itSurtidor != (*it)->surtidor.end(); ++itSurtidor)
			{
				if((*itSurtidor)->TTL > 0 )
				{
					Vector3 pos,vel;
					pos = (*itSurtidor)->pos;
					vel = (*itSurtidor)->vel;
					(*itSurtidor)->pos = pos + vel * dt * 0.01;

					float size,alpha,ttl;
					size =  (*itSurtidor)->size;
					(*itSurtidor)->size = size + (1.5 * dt);
					/*alpha = (*itSurtidor)->alpha;
					(*itSurtidor)->alpha = alpha - (0.01 * dt) ;*/
					ttl = (*itSurtidor)->TTL;
					(*itSurtidor)->TTL = ttl - dt;
				}
				else
				{
					(*itSurtidor)->pos = (*itSurtidor)->parent->pos;
					(*itSurtidor)->size = (*itSurtidor)->parent->size;
					//(*itSurtidor)->alpha = (*itSurtidor)->parent->alpha;
					(*itSurtidor)->vel = (*itSurtidor)->parent->vel * -1000;

					(*itSurtidor)->vel.x = (*itSurtidor)->vel.x * (1 + rand()%5);
					(*itSurtidor)->vel.y = (*itSurtidor)->vel.y * (1 + rand()%5);
					(*itSurtidor)->vel.z = (*itSurtidor)->vel.z * (1 + rand()%5);

					(*itSurtidor)->TTL = 2 + rand()%5;/*(rand()%(((*itSurtidor)->parent->fixedTTL)/10));*/
				}
			}
		}
		else
		{
			int particulasDead = 0;
			std::sort( (*it)->surtidor.begin(), (*it)->surtidor.end(), isCloserToCamera );
			for(std::vector<Particula*>::iterator itSurtidor = (*it)->surtidor.begin(); itSurtidor != (*it)->surtidor.end(); ++itSurtidor)
			{
				if((*itSurtidor)->TTL > 0 )
				{
					Vector3 pos,vel;
					pos = (*itSurtidor)->pos;
					vel = (*itSurtidor)->vel;
					(*itSurtidor)->pos = pos + vel * dt * 0.01;

					float size,alpha,ttl;
					size =  (*itSurtidor)->size;
					(*itSurtidor)->size = size + (1.5 * dt);
					ttl = (*itSurtidor)->TTL;
					(*itSurtidor)->TTL = ttl - dt;
				}
				else
				{
					if((*itSurtidor)->size > 0.5){
					(*itSurtidor)->size = (*itSurtidor)->size - 0.5;
					}
					++particulasDead;

				}
			}

			if(particulasDead == numParticulasPorFuente)
				particulasToErase.push_back((*it)->id);
			//(*it)->TTL = (*it)->fixedTTL;
		}
	}
}


void EmisorParticulas::render()
{
	if(particulas.size() > 0 )
	{
		Camera* camera = World::getInstance()->currentCamera;

		top = camera->getLocalVector(Vector3(0,1,0));
		right = camera->getLocalVector(Vector3(1,0,0));
		std::sort( particulas.begin(), particulas.end(), isCloserToCamera );

		for(std::map<std::string, int>::iterator it = textures.begin(); it != textures.end(); ++it)
		{
			glPushMatrix();
			Mesh* plane = new Mesh();

			plane->vertices.reserve((it)->second*4*numParticulasPorFuente); // numParticulasPorFuente pq cada particula ha de tenir numParticulasPorFuente fills
			plane->uvs.reserve((it)->second*4*numParticulasPorFuente);

			int indexOfTexture = 0;

			for(int i = 0; i<particulas.size(); ++i)
			{
				/*if(particulas[i]->texture->filename == ((*it).first))
				{
				Particula* particula = particulas[i];
				Vector3 pos = particula->pos;

				plane->vertices.push_back(pos + (top*particula->size) - (right*particula->size) );
				plane->vertices.push_back(pos + (top*particula->size)*-1 - (right*particula->size) );
				plane->vertices.push_back(pos +  (top*particula->size)*-1 + (right*particula->size)  );
				plane->vertices.push_back(pos +  (top*particula->size) + (right*particula->size)  );		

				plane->uvs.push_back( Vector2(0,1) ); 
				plane->uvs.push_back( Vector2(0,0) );
				plane->uvs.push_back( Vector2(1,0) );
				plane->uvs.push_back( Vector2(1,1) );

				indexOfTexture = i;
				}*/
				if(particulas[i]->texture->filename == ((*it).first))
				{
					for(std::vector<Particula*>::iterator itSurtidor = particulas[i]->surtidor.begin(); itSurtidor != particulas[i]->surtidor.end(); ++itSurtidor)
					{
						Particula* particula = (*itSurtidor);
						Vector3 pos = particula->pos;

						plane->vertices.push_back(pos + (top*particula->size) - (right*particula->size) );
						plane->vertices.push_back(pos + (top*particula->size)*-1 - (right*particula->size) );
						plane->vertices.push_back(pos +  (top*particula->size)*-1 + (right*particula->size)  );
						plane->vertices.push_back(pos +  (top*particula->size) + (right*particula->size)  );		

						plane->uvs.push_back( Vector2(0,1) ); 
						plane->uvs.push_back( Vector2(0,0) );
						plane->uvs.push_back( Vector2(1,0) );
						plane->uvs.push_back( Vector2(1,1) );

						indexOfTexture = i;
					}
				}

			}
			plane->primitive = GL_QUADS;
			glPushAttrib( GL_ALL_ATTRIB_BITS );

			glAlphaFunc( GL_GREATER, particulas[indexOfTexture]/*->surtidor[0]*/->alpha);
			glEnable( GL_ALPHA_TEST );

			glEnable(GL_BLEND);
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );



			particulas[indexOfTexture]->texture->bind();
			glColor3f(0.19,0.19,0.19);
			plane->render();
			particulas[indexOfTexture]->texture->unbind();


			glPopAttrib();


			glPopMatrix();
		}
	}
}
