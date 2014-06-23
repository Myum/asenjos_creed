#include "EntityMesh.h"

EntityMesh::EntityMesh():Entity()
{
	this->className = "EntityMesh";
	this->fixedMesh = 0;
	this->has_particula = 0;
	this->intercepted = 0;
}

EntityMesh::EntityMesh( Mesh* mesh ):Entity()
{
	this->className = "EntityMesh";
	this->mesh = this->low_mesh = this->high_mesh = mesh;
	this->color = Vector3(1, 1, 1);
	this->centerEntity = mesh->center;
	this->fixedMesh = 0;
	this->has_particula = 0;
	this->intercepted = 0;
}

EntityMesh::EntityMesh( Mesh* mesh, Texture* texture ):Entity()
{
	this->className = "EntityMesh";
	this->mesh = this->low_mesh = this->high_mesh = mesh;
	this->texture = texture;
	this->color = Vector3(1, 1, 1);
	this->centerEntity = mesh->center;
	this->fixedMesh = 0;
	this->has_particula = 0;
	this->intercepted = 0;
}

EntityMesh::EntityMesh( Mesh* mesh, Texture* texture, Vector3 color ):Entity()
{
	this->className = "EntityMesh";
	this->mesh = this->low_mesh = this->high_mesh = mesh;
	this->texture = texture;
	this->color = color;
	this->centerEntity = mesh->center;
	this->fixedMesh = 0;
	this->has_particula = 0;
	this->intercepted = 0;
}

EntityMesh::~EntityMesh()
{
	if(has_particula)
		//EmisorParticulas::getInstance()->killParticula(particula);
		EmisorParticulas::getInstance()->getParticula(particula)->TTL = 0;
}




void EntityMesh::update( double elapsed_time )
{

	for(vecEntities::iterator it = childEntities.begin() ; it != childEntities.end(); ++it)
	{
		(*it)->update( elapsed_time );
	}

	//update center entity
	centerEntity = Vector3(model.m[12], model.m[13], model.m[14]);
	//front = this->getGlobalMatrix() * 
	//position = ( parent ? ( parent->model * model ) * centerEntity : model * centerEntity );
	//position = model * centerEntity;
	if(has_particula)
	{
		Particula* p = EmisorParticulas::getInstance()->getParticula(particula);
		if(p != NULL)
		{
			front = this->getGlobalMatrix().frontVector();
			p->pos = this->centerEntity;
			p->vel = this->front;
		
		}
		else has_particula = 0;
	}
	this->mesh->collision_model->setTransform(getGlobalMatrix().m);
}

void EntityMesh::render()
{
	glPushMatrix();
	model.set();

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
}

bool EntityMesh::rayCollision(Vector3 ray_origin, Vector3 ray_direction){
	return this->mesh->collision_model->rayCollision(ray_origin.v, ray_direction.v,true);
}
Vector3 EntityMesh::rayCollisionPoint(Vector3 ray_origin, Vector3 ray_direction){
	this->mesh->collision_model->rayCollision(ray_origin.v, ray_direction.v,true);
	float p[3];
	this->mesh->collision_model->getCollisionPoint(p,true);// point on local position
	Vector3 vec;
	vec.x = p[0];
	vec.y = p[1];
	vec.z = p[2];
	vec = this->getGlobalMatrix() * vec; // point on world position
	return vec;
}
bool EntityMesh::meshCollision(EntityMesh* eM)
{
	return this->mesh->collision_model->collision(eM->mesh->collision_model,-1,0,eM->getGlobalMatrix().m);
}

float EntityMesh::distanceOfCollision(Vector3 ray_origin, Vector3 ray_direction)
{
	Vector3 vec = this->rayCollisionPoint(ray_origin, ray_direction);
	return vec.distance(ray_origin);
}

bool EntityMesh::boundingBoxCollision(EntityMesh* eM)
{
	Vector3 halfsize = this->getGlobalMatrix() * this->mesh->halfsize;
	Vector3 halfsize2 = eM->getGlobalMatrix() * eM->mesh->halfsize;
//	std::cout<<this->name<<"  :: "<<halfsize.x - (this->mesh->halfsize.x * 2)<<"  -  "<<halfsize.x << std::endl;
	//std::cout<<eM->name<<"  :: "<<halfsize2.x-(eM->mesh->centerBB.x * 2)<< "  -  " << halfsize2.x << std::endl;
	if( (halfsize.x > halfsize2.x) && ( halfsize.x - (this->mesh->halfsize.x * 2)) < halfsize2.x )
		if( (halfsize.y > halfsize2.y) && ( halfsize.y - (this->mesh->halfsize.y * 2)) < halfsize2.y )  
			if( (halfsize.z > halfsize2.z) && ( halfsize.z - (this->mesh->halfsize.z * 2)) < halfsize2.z )  
				return true;
	return false;
}
