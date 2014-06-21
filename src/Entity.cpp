#include "Entity.h"

Entity::Entity()
{
	className = "Entity";
	has_alpha = false;
	has_blend = false;
}

Entity::~Entity()
{
}

void Entity::init()
{
	for(vecEntities::iterator it = childEntities.begin() ; it != childEntities.end(); ++it)
	{
		(*it)->init();
	}
}

void Entity::render()
{
	glPushMatrix();
	model.set();

	for(vecEntities::iterator it = childEntities.begin() ; it != childEntities.end(); ++it)
	{
		(*it)->render();
	}

	glPopMatrix();
}

void Entity::update( double elapsed_time )
{
	//std::cout << "entity: "<<std::endl;
	for(vecEntities::iterator it = childEntities.begin() ; it != childEntities.end(); ++it)
	{
		(*it)->update( elapsed_time );
	}
}

void Entity::addEntityChildren( Entity* childEntity )
{
	//set parent of entity child
	childEntity->parent = this;
	//add children to vector
	childEntities.push_back( childEntity );
	//only add children to map if it's name is defined
	if( childEntity->name != "" )
	{
		childNamedEntities.insert( itemMapEntity( childEntity->name, childEntity ) );
	}
}

Entity* Entity::getChildren( std::string name )
{
	mapEntities::iterator it = childNamedEntities.find( name );
	if( it != childNamedEntities.end() )
	{
		return it->second;
	}
	//if element isn't in the map
	return NULL;
}

Entity* Entity::getParent()
{ 
	return parent; 
}

Matrix44 Entity::getGlobalMatrix(){
	if(parent){
		return model * parent->getGlobalMatrix();
	}
	return model;
}

float Entity::distanceToCamera(Vector3 eye)
{
	//Matrix44 m = getGlobalMatrix();
	Vector3 v;
	v.x = (this->getGlobalMatrix().m)[12];
	v.y = (this->getGlobalMatrix().m)[13];
	v.z = (this->getGlobalMatrix().m)[14];
	return  eye.distance( v );
}

void Entity::killEntity(std::string name)
{
	vecEntities::iterator it = childEntities.begin();
	for(it; it!=childEntities.end();++it)
	{
		if((*it)->name == name)
			break;
	}
	childEntities.erase(it);
	mapEntities::iterator it2 = childNamedEntities.find( name );
	if( it2 != childNamedEntities.end() )
	{
		it2->second = NULL;
		std::cout<< name << " borrado!!!!"<<std::endl;
		std::cout<< childEntities.size() << " redimensionado"<<std::endl;
	}

}