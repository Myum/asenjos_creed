#include "EntityMesh.h"

EntityMesh::EntityMesh():Entity()
{
	this->className = "EntityMesh";
}

EntityMesh::EntityMesh( Mesh* mesh ):Entity()
{
	this->className = "EntityMesh";
	this->mesh = this->low_mesh = this->high_mesh = mesh;
	this->color = Vector3(1, 1, 1);
	this->centerEntity = mesh->center;
}

EntityMesh::EntityMesh( Mesh* mesh, Texture* texture ):Entity()
{
	this->className = "EntityMesh";
	this->mesh = this->low_mesh = this->high_mesh = mesh;
	this->texture = texture;
	this->color = Vector3(1, 1, 1);
	this->centerEntity = mesh->center;
}

EntityMesh::EntityMesh( Mesh* mesh, Texture* texture, Vector3 color ):Entity()
{
	this->className = "EntityMesh";
	this->mesh = this->low_mesh = this->high_mesh = mesh;
	this->texture = texture;
	this->color = color;
	this->centerEntity = mesh->center;
}

EntityMesh::~EntityMesh()
{
}

/*void EntityMesh::init()
{
//set flags
//DEPTH_TEST = true;

camera = new Camera();
//camera->lookAt(Vector3(0,50,-50),Vector3(0,0,0), Vector3(0,1,0)); //position the camera and point to 0,0,0
camera->lookAt(camera->eye,model*Vector3(),Vector3(0,1,0));
camera->setPerspective(70,WINDOW_WIDTH/(float)WINDOW_HEIGHT,0.1,10000); //set the projection, we want to be perspective
}*/

void EntityMesh::update( double elapsed_time )
{
	for(vecEntities::iterator it = childEntities.begin() ; it != childEntities.end(); ++it)
	{
		(*it)->update( elapsed_time );
	}

	//update center entity
	centerEntity = Vector3(model.m[12], model.m[13], model.m[14]);
	//position = ( parent ? ( parent->model * model ) * centerEntity : model * centerEntity );
	//position = model * centerEntity;

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
		//std::cou
	}
	mesh->render();
	this->texture->unbind();

	glPopMatrix();
}

bool EntityMesh::rayCollision(Vector3 ray_origin, Vector3 ray_direction){
	return this->mesh->collision_model->rayCollision(ray_origin.v, ray_direction.v,false);
}
