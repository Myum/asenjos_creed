#include "mesh.h"
#include "includes.h"
#include "text.h"
#include <fstream>

Mesh::Mesh()
{
	primitive = GL_TRIANGLES;
	centerBB = Vector3(0,0,0);
	halfsize = Vector3(0,0,0);
}

void Mesh::clear()
{
	vertices.clear();
	normals.clear();
	uvs.clear();
}

void Mesh::render()
{
	assert(vertices.size() && "No vertices in this mesh");
	/*glBegin(GL_LINES);
		//glColor4f(1,1,0,1);
		glVertex3f(centerBB.x, centerBB.y, centerBB.z);
		//std::cout<<halfsize.x<<std::endl;
		glVertex3f( halfsize.x, halfsize.y, halfsize.z);
	glEnd();*/

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0] );

	if (normals.size())
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, &normals[0] );
	}

	if (uvs.size())
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2,GL_FLOAT, 0, &uvs[0] );
	}

	glDrawArrays(primitive, 0, vertices.size() );
	glDisableClientState(GL_VERTEX_ARRAY);

	if (normals.size())
		glDisableClientState(GL_NORMAL_ARRAY);
	if (uvs.size())
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

}

void Mesh::createPlane(float size)
{
	vertices.clear();
	normals.clear();
	uvs.clear();

	//create six vertices (3 for upperleft triangle and 3 for lowerright)

	vertices.push_back( Vector3(size,0,size) );
	vertices.push_back( Vector3(size,0,-size) );
	vertices.push_back( Vector3(-size,0,-size) );
	vertices.push_back( Vector3(-size,0,size) );
	vertices.push_back( Vector3(size,0,size) );
	vertices.push_back( Vector3(-size,0,-size) );

	//all of them have the same normal
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );

	//texture coordinates
	uvs.push_back( Vector2(1,1) );
	uvs.push_back( Vector2(1,0) );
	uvs.push_back( Vector2(0,0) );
	uvs.push_back( Vector2(0,1) );
	uvs.push_back( Vector2(1,1) );
	uvs.push_back( Vector2(0,0) );
}

bool Mesh::loadASE(const char *meshFile)
{
	assert(meshFile);

	char meshFileBin[100];
	strcpy_s(meshFileBin, meshFile);
	strcat_s(meshFileBin, ".bin");

	if(loadASEbin(meshFileBin)){
		return true;
	}

	//clear all vectors
	clear();

	text parser;
	if(parser.create(meshFile)){
		std::cout << "file loaded" << std::endl;
	}else{
		std::cout << "couldn't load file!" << std::endl;
		return false;
	}

	parser.seek("*MESH_NUMVERTEX");
	int numVertices = parser.getint();
	parser.seek("*MESH_NUMFACES");
	int numCaras = parser.getint();

	std::vector< Vector3 > vert_temp(numVertices);

	for(int i=0; i < numVertices; i++){
		parser.seek("*MESH_VERTEX"); //*MESH_VERTEX
		parser.getint(); //index
		Vector3 v;
		v.x = parser.getfloat();
		v.z = -parser.getfloat();
		v.y = parser.getfloat();
		vert_temp[i] = v;
		//calculate center of the mesh
		center.x += v.x;
		center.y += v.y;
		center.z += v.z;

		if(v.x < minBB.x )	// 
			minBB.x = v.x;
		if(v.y < minBB.y )
			minBB.y = v.y;
		if(v.z < minBB.z )
			minBB.z = v.z;

		if(v.x > maxBB.x )
			maxBB.x = v.x;
		if(v.y > maxBB.y )
			maxBB.y = v.y;
		if(v.z > maxBB.z )
			maxBB.z = v.z;

	}

	//average all values in center vector
	center.x /= numVertices;
	center.y /= numVertices;
	center.z /= numVertices;

	//center for the bounding box
	centerBB.x = (maxBB.x + minBB.x) / 2;
	centerBB.y = (maxBB.y + minBB.y) / 2;
	centerBB.z = (maxBB.z + minBB.z) / 2;

	halfsize = maxBB - centerBB;

	//calculate maximum radius
	radius = -1;
	for(int i=0; i < vert_temp.size(); i++)
	{
		if(center.distance( vert_temp[i] ) > radius)
		{
			radius = center.distance( vert_temp[i] );
		}
	}

	vertices.resize(numCaras * 3);
	for(int face = 0; face < numCaras; ++face){
		parser.seek("*MESH_FACE"); 
		parser.getword();
		for(int vxFce = 0; vxFce < 3; ++vxFce){
			parser.getword();
			int v = parser.getint();
			vertices[face*3 + vxFce] = vert_temp[v];
		}
	}
	vert_temp.clear();

	parser.seek("*MESH_NUMTVERTEX");
	int tex_vx = parser.getint();
	std::vector< Vector2 > uv_temp(tex_vx); // no farem servir la 3a coord d textura
	for(int i = 0; i < tex_vx; ++i){
		parser.seek("*MESH_TVERT");
		parser.getint();
		float u = parser.getfloat();
		float v = parser.getfloat();
		uv_temp[i] = Vector2(u, v); 
	}

	uvs.resize(numCaras * 3);
	for(int face = 0; face < numCaras; ++face){
		parser.seek("*MESH_TFACE");
		parser.getint();
		for(int vxTxtr = 0; vxTxtr < 3; ++vxTxtr){
			uvs[face*3 + vxTxtr] = uv_temp[parser.getint()];
		}
	}

	normals.resize(numCaras * 3);
	for(int i = 0; i < numCaras * 3; ++i){
		parser.seek("*MESH_VERTEXNORMAL"); 
		parser.getint();
		Vector3 v;
		v.x = parser.getfloat();
		v.z = -parser.getfloat();
		v.y = parser.getfloat();
		normals[i] = v;
	}

	createBin(meshFileBin);
	clear();
	loadASEbin(meshFileBin);
	return true;

}
bool Mesh::createBin(const char* meshFileBin){

	std::ofstream binFile;
	binFile.open(meshFileBin, std::ios::out | std::ios::binary);
	if(!binFile.is_open()){
		std::cout << "No se puedo generar el fichero " << meshFileBin << std::endl;
		return false;
	}
	int numVertices = vertices.size();
	binFile.write((const char*)&numVertices, sizeof(int));

	binFile.write((char*)&vertices[0], sizeof(Vector3)*numVertices);
	binFile.write((char*)&normals[0], sizeof(Vector3)*numVertices);
	binFile.write((char*)&uvs[0], sizeof(Vector2)*numVertices);

	binFile.write((char*)&center.x, sizeof(float));
	binFile.write((char*)&center.y, sizeof(float));
	binFile.write((char*)&center.z, sizeof(float));

	binFile.write((char*)&radius, sizeof(float));

	binFile.write((char*)&centerBB.x, sizeof(float));
	binFile.write((char*)&centerBB.y, sizeof(float));
	binFile.write((char*)&centerBB.z, sizeof(float));
	
	binFile.write((char*)&halfsize.x, sizeof(float));
	binFile.write((char*)&halfsize.y, sizeof(float));
	binFile.write((char*)&halfsize.z, sizeof(float));

	binFile.close();
	return true;

}

bool Mesh::loadASEbin(const char* meshFileBin){

	std::ifstream binFile;
	binFile.open(meshFileBin, std::ios::in | std::ios::binary);
	if(!binFile.is_open()){
		std::cout << "No se puede abrir el fichero " << meshFileBin << std::endl;
		return false;
	}
	int numVertices;
	binFile.read((char*)&numVertices, sizeof(int));

	vertices.resize(numVertices);
	binFile.read((char*)&vertices[0], sizeof(Vector3)*numVertices);

	normals.resize(numVertices);
	binFile.read((char*)&normals[0], sizeof(Vector3)*numVertices);

	uvs.resize(numVertices);
	binFile.read((char*)&uvs[0], sizeof(Vector2)*numVertices);

	binFile.read((char*)&center.x, sizeof(float));
	binFile.read((char*)&center.y, sizeof(float));
	binFile.read((char*)&center.z, sizeof(float));

	binFile.read((char*)&radius, sizeof(float));

	binFile.read((char*)&centerBB.x, sizeof(float));
	binFile.read((char*)&centerBB.y, sizeof(float));
	binFile.read((char*)&centerBB.z, sizeof(float));
	
	binFile.read((char*)&halfsize.x, sizeof(float));
	binFile.read((char*)&halfsize.y, sizeof(float));
	binFile.read((char*)&halfsize.z, sizeof(float));

	binFile.close();

	this->coldetInit();

	return true;
}

void Mesh::coldetInit()
{
	collision_model = newCollisionModel3D();
	collision_model->setTriangleNumber(vertices.size()/3);

	for(unsigned int i = 0; i < vertices.size()/3; ++i){
		collision_model->addTriangle(
			vertices[i*3].x,vertices[i*3].y,vertices[i*3].z,
			vertices[i*3+1].x,vertices[i*3+1].y,vertices[i*3+1].z, 
			vertices[i*3+2].x,vertices[i*3+2].y,vertices[i*3+2].z
			);
	}

	collision_model->finalize();
}