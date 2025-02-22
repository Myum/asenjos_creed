/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class wraps the behaviour of a camera. A Camera helps to set the point of view from where we will render the scene.
	The most important attributes are  eye and center which say where is the camera and where is it pointing.
	This class also stores the matrices used to do the transformation and projection of the scene.
*/

#ifndef CAMERA_H
#define CAMERA_H

#include "framework.h"
#include "clipper.h"

class Camera
{
private:
	static Camera* instance;
public:

	enum { PERSPECTIVE, ORTHOGRAPHIC }; //types of cameras available

	char type; //camera type

	//get instance
	static Camera* getInstance()
	{
		if(instance == NULL)
		{
			instance = new Camera();
		}
		return	instance;
	}

	//vectors to define the orientation of the camera
	Vector3 eye; //where is the camera
	Vector3 center; //where is it pointing
	Vector3 up; //the up pointing up

	//properties of the projection of the camera
	float fov;			//view angle in degrees (1/zoom)
	float aspect;		//aspect ratio (width/height)
	float near_plane;	//near plane
	float far_plane;	//far plane

	//for orthogonal projection
	float left,right,top,bottom;

	//matrices
	Matrix44 view_matrix;
	Matrix44 projection_matrix;
	Matrix44 viewprojection_matrix;

	Camera();
	void set();

	//translate and rotate the camera
	void move(Vector3 delta);
	void rotate(float angle, const Vector3& axis);

	//transform a local camera vector to world coordinates
	Vector3 getLocalVector(const Vector3& v);

	//set the info
	void setPerspective(float fov, float aspect, float near_plane, float far_plane);
	void setOrthographic(float left, float right, float top, float bottom, float near_plane, float far_plane);
	void lookAt(const Vector3& eye, const Vector3& center, const Vector3& up);

	//compute the matrices
	void updateViewMatrix();
	void updateProjectionMatrix();

	//clipper
	Clipper clipper;
};


#endif