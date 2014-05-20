#include "Entity.h"

Entity::Entity()
{
	className = "Entity";
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