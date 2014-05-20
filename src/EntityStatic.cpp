#include "EntityStatic.h"

EntityStatic::EntityStatic():EntityMesh()
{
	className = "EntityStatic";
}

EntityStatic::EntityStatic( Mesh* mesh ):EntityMesh( mesh )
{
	className = "EntityStatic";
}

EntityStatic::EntityStatic( Mesh* mesh, Texture* texture ):EntityMesh( mesh, texture )
{
	className = "EntityStatic";
}

EntityStatic::EntityStatic( Mesh* mesh, Texture* texture, Vector3 color ):EntityMesh( mesh, texture, color )
{
	className = "EntityStatic";
}

EntityStatic::~EntityStatic()
{
}

void EntityStatic::render()
{
	glDisable( GL_DEPTH_TEST );

	glPushMatrix();
	model.set();

	this->texture->bind();
	glColor3f(color.x, color.y, color.z);

	mesh->render();

	glPopMatrix();

	glEnable( GL_DEPTH_TEST );
}