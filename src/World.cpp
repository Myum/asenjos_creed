#include "World.h"
#include "EntityMesh.h"

World* World::instance = NULL;

World::World():Entity()
{
	model.set();
	className = "World";
	parent = NULL;
}

void World::render()
{
	glPushMatrix();
	model.set();

	BulletManager::getInstance()->render();
	//glDisable( GL_CULL_FACE );

	for(vecEntities::iterator it = childEntities.begin() ; it != childEntities.end(); ++it)
	{
		if((*it)->className == "EntityMesh")
		{
			EntityMesh* child = (EntityMesh*)(*it);
			//clipping
			if( !currentCamera->clipper.SphereInFrustum( child->centerEntity.x, child->centerEntity.y, child->centerEntity.z, child->mesh->radius ) )
			{
				continue;
			}
			//check wich mesh we need to show
			if( currentCamera->center.distance( child->centerEntity ) > 50 )
			{
				child->mesh = child->low_mesh;
			}else
			{
				child->mesh = child->high_mesh;
			}
		}
		(*it)->render();
	}

	//glEnable( GL_CULL_FACE );

	glPopMatrix();
}

void World::update( double elapsed_time )
{
	for(vecEntities::iterator it = childEntities.begin() ; it != childEntities.end(); ++it)
	{
		(*it)->update( elapsed_time );
		//check sphere collisions
		/*if((*it)->className == "EntityMesh" )
		{
			for(vecEntities::iterator itColl = it+1 ; itColl != childEntities.end(); ++itColl)
			{
				if((*itColl)->className == "EntityMesh")
				{
					//std::cout << ((EntityMesh*)(*it))->name << " radius: " << ((EntityMesh*)(*it))->mesh->radius << " " << ((EntityMesh*)(*itColl))->name << " radius: " << ((EntityMesh*)(*itColl))->mesh->radius << std::endl;
					//std::cout << "Distance between them: " << ( (EntityMesh*)(*it) )->centerEntity.distance( ( (EntityMesh*)(*itColl) )->centerEntity ) << std::endl;
					if( ( ( (EntityMesh*)(*it) )->mesh->radius + ( (EntityMesh*)(*itColl) )->mesh->radius ) > ( (EntityMesh*)(*it) )->centerEntity.distance( ( (EntityMesh*)(*itColl) )->centerEntity ) )
					{
						//std::cout << "Collision between " << ((EntityMesh*)(*it))->name << " and " << ((EntityMesh*)(*itColl))->name << std::endl;
					}
				}
			}
		}*/
	}
	BulletManager::getInstance()->update(elapsed_time);
}