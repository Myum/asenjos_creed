#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "EntityMesh.h"
#include "EntityStatic.h"
#include "EntityPlayer.h"
#include "World.h"
#include <map>
#include <mmsystem.h>
#include <sstream>
#include "Bullet.h"


//some globals
//World will contain all Entities
//quick reference to World
World* world;
//Mesh* plane = NULL;
Texture* texture = NULL;
Shader* shader = NULL;
float angle = 0;
//EntityMesh* entityPlane = NULL;
SDL_Joystick* joystick = NULL;

EntityPlayer* plane;
EntityMesh* sky;
EntityMesh* box=NULL;

int x = 0, y = 0, z = 0;

Game::Game(SDL_Window* window)
{
	this->window = window;
	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT 
	// things create here cannot access opengl
	window_width = WINDOW_WIDTH;
	window_height = WINDOW_HEIGHT;

	keystate = NULL;
	mouse_locked = false;

	//set reference to World
	world = World::getInstance();
}

//Here we have already GL working, so we can create meshes and textures
void Game::init(void)
{
	std::cout << "initiating game..." << std::endl;

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	//instantiate Managers
	this->meshManager = new MeshManager();
	this->textureManager = new TextureManager();

	//create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0,50,-50),Vector3(0,0,0), Vector3(0,1,0)); //position the camera and point to 0,0,0
	//camera->lookAt(Vector3(0,-50,50),Vector3(0,0,0), Vector3(0,1,0)); //position the camera and point to 0,0,0
	camera->setPerspective(70,WINDOW_WIDTH/(float)WINDOW_HEIGHT,0.1,10000); //set the projection, we want to be perspective
	world->currentCamera = camera;

	//create sky
	/*sky = new EntityStatic( 
		meshManager->getMesh("assets/meshes/cubemap.ASE"),
		textureManager->getTexture("assets/textures/cielo.tga"));
	sky->name = "sky";
	world->addEntityChildren( sky );

	world->addEntityChildren(new EntityMesh(
		meshManager->getMesh("assets/meshes/island.ASE"),
		textureManager->getTexture("assets/textures/island_color_luz.tga")
		));*/

	/*world->addEntityChildren(new EntityMesh(
		meshManager->getMesh("assets/meshes/water_deep.ASE"),
		textureManager->getTexture("assets/textures/water_deep.tga")
		));*/

	/*box = new EntityMesh (
		meshManager->getMesh("assets/meshes/box.ASE"),
		textureManager->getTexture("assets/textures/bomber_axis.tga")
		);
	box->name = "box";
	world->addEntityChildren( (EntityMesh*)box );*/
	//create a plane mesh
	plane = new EntityPlayer(
		meshManager->getMesh("assets/meshes/bomber_axis.ASE"),
		textureManager->getTexture("assets/textures/bomber_axis.tga")
		);
	plane->name = "plane";
	world->addEntityChildren( (EntityMesh*)plane );

	//create a plane mesh
	for(int i = 0; i <100; i++)
	{
		EntityMesh* enemies = new EntityMesh(
			meshManager->getMesh("assets/meshes/bomber_axis.ASE"),
			textureManager->getTexture("assets/textures/spitfire_color_spec.tga")
			);
		enemies->low_mesh = meshManager->getMesh("assets/meshes/spitfire_low.ASE");
		enemies->model.setTranslation(rand()%100, rand()%100, rand()%100);

		std::ostringstream ostr;
		ostr << "plane" << i; 
		enemies->name = ostr.str();
		std::cout << enemies->name << std::endl;
		world->addEntityChildren( (EntityMesh*)enemies );
	}

	world->init();

	//shader = new Shader();
	//shader->load("assets/shaders/simple.vs","assets/shaders/simple.ps");

	//set Joystick
	joystick = openJoystick();
	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse

	//play background sound
	//PlaySound(TEXT("assets/sounds/war.WAV"), NULL, SND_ASYNC|SND_FILENAME|SND_LOOP);
}

//what to do when the image has to be draw
void Game::render(void)
{
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // bucle de render básico

	//Put the camera matrices on the stack of OpenGL (only for fixed rendering)
	//camera->set();
	world->currentCamera->set();

	world->render();
	drawGrid(500);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window); // el doble buffer de openGL
}

void Game::update(double seconds_elapsed)
{

	//to navigate with the mouse fixed in the middle
	if (mouse_locked)   
	{
		SDL_WarpMouseInWindow(this->window, window_width*0.5, window_height*0.5); //put the mouse back in the middle of the screen
		this->mouse_position.x = window_width*0.5;
		this->mouse_position.y = window_height*0.5;
	}

	double speed = seconds_elapsed * 1000; //the speed is defined by the seconds_elapsed so it goes constant

	//mouse input to rotate the cam
	if ((mouse_state & SDL_BUTTON_LEFT) || mouse_locked ) //is left button pressed?
	{
		camera->rotate(speed * mouse_delta.x * 0.0005, Vector3(0,0,-1));
		camera->rotate(speed * mouse_delta.y * 0.0005, camera->getLocalVector( Vector3(-1,0,0)));
	}

	//async input to move the camera around
	if(keystate[SDL_SCANCODE_LSHIFT]) speed *= 10; //move faster with left shift
	if (keystate[SDL_SCANCODE_W]) camera->move(Vector3(0,0,1) * speed);
	if (keystate[SDL_SCANCODE_S]) camera->move(Vector3(0,0,-1) * speed);
	if (keystate[SDL_SCANCODE_A]) camera->move(Vector3(1,0,0) * speed);
	if (keystate[SDL_SCANCODE_D]) camera->move(Vector3(-1,0,0) * speed);

	//if(keystate[SDL_SCANCODE_1]) world->currentCamera = plane->camera;

	if (keystate[SDL_SCANCODE_UP]) plane->model.traslateLocal(0, 0, 2);
	if (keystate[SDL_SCANCODE_DOWN]) plane->model.traslateLocal(0, 0, -2);
	if (keystate[SDL_SCANCODE_LEFT]) plane->model.traslateLocal(2, 0, 0);
	if (keystate[SDL_SCANCODE_RIGHT]) plane->model.traslateLocal(-2, 0, 0);


	float avanzar = 0;
	float virar = 0;
	if(joystick)
	{
		JoystickState state = getJoystickState( joystick );
		
		if(state.button[6] != NULL)
			speed *= 100;

		avanzar = state.axis[LEFT_ANALOG_Y];
		if (avanzar>-0.2 && avanzar < 0.2) avanzar = 0;
		plane->model.traslateLocal(0,0,-avanzar*seconds_elapsed*speed );

		virar = state.axis[LEFT_ANALOG_X];
		if (virar>-0.2 && virar < 0.2) virar = 0;
		plane->model.traslateLocal(-virar*seconds_elapsed*speed,0,0);

		float inclinarVert = state.axis[2];
		//std::cout<<state.axis[2]<<std::endl;
		if (inclinarVert>-0.4 && inclinarVert< 0.4) inclinarVert = 0;
		plane->model.rotateLocal(inclinarVert*seconds_elapsed*3,Vector3(1,0,0));
		float inclinarHor = state.axis[RIGHT_ANALOG_Y];
		if (inclinarHor>-0.4 && inclinarHor< 0.4) inclinarHor = 0;
		plane->model.rotateLocal(inclinarHor*seconds_elapsed*3,Vector3(0,0,-1));

		if(state.button[7] != NULL)
		{
			plane->fire = true;
			Vector3 dir = world->currentCamera->getLocalVector(Vector3(0,0,-1));
			for(size_t j = 0; j<world->childEntities.size();++j)
			{
				EntityMesh* eM = (EntityMesh*)world->childEntities[j];
				Vector3 coll;
				eM->mesh->collision_model->setTransform(eM->getGlobalMatrix().m);
				if(eM->rayCollision(world->currentCamera->eye,dir))
				{
					// target intercepted
					eM->intercepted = true;
				}
			}
		}
	}


	angle += seconds_elapsed * 1000;
	/*if(avanzar< -0.7)
	{
	camera->lookAt(plane->model * Vector3(0,2,-7*((avanzar*(-1))+0.4)),plane->model*Vector3(0,0,100),plane->model.frontVector());
	}
	else 
	{*/
	camera->lookAt(plane->model * Vector3(0,4,-10), plane->model*Vector3(0, 0, 150),plane->model.frontVector());	
	//}

	//sky = (EntityMesh*)world->getChildren( "sky" );
//	sky->model.setTranslation( camera->eye.x, camera->eye.y, camera->eye.z );
	//camera->lookAt(camera->eye,entityPlane->model*Vector3(),Vector3(0,1,0));

		if(keystate[SDL_SCANCODE_SPACE])
	{
		std::cout << "bullet sent" << std::endl;
		/*Vector3 start_pos = world->currentCamera->eye;
		start_pos.z -= 1.19;
		start_pos.x -= 0.02;
		BulletManager::getInstance()->createBullet(start_pos,world->currentCamera->getLocalVector(Vector3(0,-1,-500)),"plane");
		//start_pos.z -= 1.09;
		start_pos.x += 0.04;
		BulletManager::getInstance()->createBullet(start_pos,world->currentCamera->getLocalVector(Vector3(0,-1,-500)),"plane");*/
		//Vector3 dir = camera->getLocalVector(Vector3(0,0,-1));
		plane->fire = true;
	}

			//update world
	world->update( seconds_elapsed );

	plane = (EntityPlayer*)world->getChildren( "plane" );
}

//Keyboard event handler (sync input)
void Game::onKeyPressed( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
	case SDLK_ESCAPE: exit(0); //ESC key, kill the app
	}
}


void Game::onMouseButton( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Game::setWindowSize(int width, int height)
{
	glViewport( 0,0, width, height );
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}

