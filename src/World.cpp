#include "World.h"
#include "EntityMesh.h"
#include "EntityStatic.h"

World* World::instance = NULL;

World::World():Entity()
{
	model.set();
	className = "World";
	parent = NULL;
}

void World::addIA(Entity* toControll)
{
	AIController* ia = new AIController();
	ia->controlled = toControll;

	IA.push_back(ia);
}

void World::render()
{
	glPushMatrix();
	model.set();


	//glDisable( GL_CULL_FACE );

	for(vecEntities::iterator it = childEntities.begin() ; it != childEntities.end(); ++it)
	{
		if((*it)->className == "EntityMesh")
		{
			EntityMesh* child = (EntityMesh*)(*it);
			//clipping
			if( currentCamera->clipper.SphereInFrustum( child->centerEntity.x, child->centerEntity.y, child->centerEntity.z, child->mesh->radius ) )
			{
				//std::cout << child->name << std::endl;//std::cout << abs(currentCamera->center.distance( child->centerEntity )) << std::endl;
				//check wich mesh we need to show
				if( abs(currentCamera->center.distance( child->centerEntity )) < 160 )
				{
					child->mesh = child->high_mesh;

				}else
				{
					child->mesh = child->low_mesh;
				}
				child->render();
			}
			/*if((*it)->name == "box" ){
			//std::cout<< (child->centerEntity).x<<"  "<<(child->centerEntity).y<<"  "<<(child->centerEntity).z<<std::endl;
			//std::cout<<"       "<< ((*it)->getGlobalMatrix().m)[12]<<"  "<<((*it)->getGlobalMatrix().m)[13]<<"  "<<((*it)->getGlobalMatrix().m)[14]<<std::endl;
			}*/

		}
		else if((*it)->className == "EntityStatic")
		{
			(*it)->render();

		}															// cal checkejar contra clipper
		else{ // billboard
			(*it)->render();
		}
	}

	//glEnable( GL_CULL_FACE );
	BulletManager::getInstance()->render();
	//EmisorParticulas::getInstance()->render();

	glPopMatrix();
}

void World::update( double elapsed_time )
{
		for(size_t i = 0; i < IA.size(); ++i)
	{
		Vector3 posPlayer = Vector3(player->model.m[12], player->model.m[13], player->model.m[14]);
		IA[i]->targetPos = posPlayer;
		IA[i]->update(elapsed_time);
	}


	for(vecEntities::iterator it = childEntities.begin() ; it != childEntities.end(); ++it)
	{
		(*it)->update( elapsed_time );
		//check sphere collisions

		if((*it)->className == "EntityMesh" )
		{
			for(vecEntities::iterator itColl = it+1 ; itColl != childEntities.end(); ++itColl)
			{
				if((*itColl)->className == "EntityMesh")
				{
					//std::cout << ((EntityMesh*)(*it))->name << " radius: " << ((EntityMesh*)(*it))->mesh->radius << " " << ((EntityMesh*)(*itColl))->name << " radius: " << ((EntityMesh*)(*itColl))->mesh->radius << std::endl;
					//std::cout << "Distance between them: " << ( (EntityMesh*)(*it) )->centerEntity.distance( ( (EntityMesh*)(*itColl) )->centerEntity ) << std::endl;
					if( !( (EntityMesh*)(*it) )->fixedMesh )	// for moving spitfires
						//if((*it)->name != "isle" && (*it)->name != "isle2" && (*it)->name != "isle0" )
					{
						if( ( ( (EntityMesh*)(*it) )->mesh->radius + ( (EntityMesh*)(*itColl) )->mesh->radius ) > ( (EntityMesh*)(*it) )->centerEntity.distance( ( (EntityMesh*)(*itColl) )->centerEntity ) )
						{
							//	std::cout <<(*it)->name<< "  sphere  "<<(*itColl)->name <<std::endl;
							//((EntityMesh*)(*itColl))->intercepted = true;
							if( ((EntityMesh*)(*it))->meshCollision( ((EntityMesh*)(*itColl)) ) )
							{
							//	std::cout << "Collision between " << ((EntityMesh*)(*it))->name << " and " << ((EntityMesh*)(*itColl))->name << std::endl;
								((EntityMesh*)(*itColl))->intercepted = true;
								((EntityMesh*)(*it))->intercepted = true;
							}
						}
					}
					else if (  ( (EntityMesh*)(*it) )->fixedMesh ) // for fixed terrains
					{
						if ( ( (EntityMesh*)(*it) )->boundingBoxCollision( ( (EntityMesh*)(*itColl) ))  )
						{
						//	std::cout << "ddd: Collision between " << ((EntityMesh*)(*it))->name << " and " << ((EntityMesh*)(*itColl))->name << std::endl;
							if( ((EntityMesh*)(*it))->meshCollision( ((EntityMesh*)(*itColl)) ) )
							{
						//		std::cout << "TERRAIN: Collision between " << ((EntityMesh*)(*it))->name << " and " << ((EntityMesh*)(*itColl))->name << std::endl;
								((EntityMesh*)(*itColl))->intercepted = true;
								((EntityMesh*)(*it))->intercepted = true;
							}
						}
					}
				}
			}
		}
	}
	BulletManager::getInstance()->update(elapsed_time);
	EmisorParticulas::getInstance()->update(elapsed_time);
}



void World::killEntity(std::string name)
{
	vecEntities::iterator it = childEntities.begin();
	for(it; it!=childEntities.end();++it)
	{
		if((*it)->name == name)
			break;
	}
	
	std::cout<< (*it)->className << " borrado!!!!"<<std::endl;

	bool has_ia = false;
	std::vector<AIController*>::iterator itIA = IA.begin();
	for(itIA; itIA!=IA.end();++itIA)
	{
		if( (*itIA)->controlled->name == name )
		{
			(*itIA)->controlled = NULL;
			has_ia = true;
			break;
		}
	}
	if(has_ia)
		IA.erase(itIA);

	delete (*it);
	childEntities.erase(it);

	mapEntities::iterator it2 = childNamedEntities.find( name );
		childNamedEntities.erase(it2);
}