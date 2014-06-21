#include "Billboard.h"
#include "mesh.h"



Billboard::Billboard(Texture *tex)
{
	size = 30;
	has_blend = true;
	this->texture = tex;

}


Billboard::~Billboard()
{
}

bool isCloserToCamera(PointCloud* a, PointCloud* b)
{
	return ( World::getInstance()->currentCamera->eye.distance(a->pos) > World::getInstance()->currentCamera->eye.distance(b->pos) );
}

void Billboard::render()
{
	glPushMatrix();
	model.set();

	Camera* camera = World::getInstance()->currentCamera;

	top = camera->getLocalVector(Vector3(0,1,0));
	right = camera->getLocalVector(Vector3(1,0,0));
	//std::cout<<"top: "<< top.x <<std::endl;



	/*top = top * size;
	right = right * size;*/

	Mesh* plane = new Mesh();

	plane->vertices.reserve(points.size()*4);
	plane->uvs.reserve(points.size()*4);

	std::sort( points.begin(), points.end(), isCloserToCamera );

	//float sise = 30;
	//glDisable(GL_CULL_FACE);

	/*plane->vertices.push_back( Vector3(-size, -size, 0) );
	plane->vertices.push_back( Vector3(-size, size, 0) );
	plane->vertices.push_back( Vector3(size, size, 0) );
	plane->vertices.push_back( Vector3(size, -size, 0) );
	*/
		/*plane->vertices.push_back(top - right );
	plane->vertices.push_back(top*-1 - right );
	plane->vertices.push_back( top*-1 + right  );
	plane->vertices.push_back( top + right  );


	plane->uvs.push_back( Vector2(0,1) );
	plane->uvs.push_back( Vector2(0,0) );
	plane->uvs.push_back( Vector2(1,0) );
	plane->uvs.push_back( Vector2(1,1) );*/

	for(int i = 0; i<points.size(); ++i)
	{
		PointCloud* point = points[i];
		Vector3 pos = point->pos;

		
		/*top = top * 30;
		right = right * 30;*/
		
		plane->vertices.push_back(pos + (top*point->size) - (right*point->size) );
		plane->vertices.push_back(pos + (top*point->size)*-1 - (right*point->size) );
		plane->vertices.push_back(pos +  (top*point->size)*-1 + (right*point->size)  );
		plane->vertices.push_back(pos +  (top*point->size) + (right*point->size)  );

		Vector2 offset;
		if(point->id%4 == 0)
			offset.set(0,0);
		else if(point->id%4 == 1) 
			offset.set(0.5,0);
		else if(point->id%4 == 2) 
			offset.set(0,0.5);
		else if(point->id%4 == 3) 
			offset.set(0.5,0.5);


		plane->uvs.push_back( offset + Vector2(0,0.5) );
		plane->uvs.push_back(  offset +Vector2(0,0) );
		plane->uvs.push_back(  offset +Vector2(0.5,0) );
		plane->uvs.push_back(  offset +Vector2(0.5,0.5) );
		

		/*plane->uvs.push_back( Vector2(0,1) ); // cloud.tga
		plane->uvs.push_back( Vector2(0,0) );
		plane->uvs.push_back( Vector2(1,0) );
		plane->uvs.push_back( Vector2(1,1) );*/
	
	}

	plane->radius = 100;


		plane->primitive = GL_QUADS;
	glPushAttrib( GL_ALL_ATTRIB_BITS );

	glAlphaFunc( GL_GREATER, 0.3);
	glEnable( GL_ALPHA_TEST );

	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	// glBlendFunc( GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA );


	this->texture->bind();

	plane->render();
	this->texture->unbind();

	//glEnable(GL_CULL_FACE);

	glPopAttrib();

	glPopMatrix();
}