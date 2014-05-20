/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef GAME_H
#define GAME_H

#include "includes.h"
#include "camera.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "Entity.h"

class Game
{
public:
	//window
	SDL_Window* window;
	float window_width;
	float window_height;
	//Managers
	MeshManager* meshManager;
	TextureManager* textureManager;
	//SoundManager
	//Don't instantiate World because it's Singleton

	//keyboard state
	const Uint8* keystate;

	//mouse state
	int mouse_state; //tells which buttons are pressed
	Vector2 mouse_position; //last mouse position
	Vector2 mouse_delta; //mouse movement in the last frame
	bool mouse_locked; //tells if the mouse is locked (not seen)
	
	//In a future, inside World as EntityCamera
	Camera* camera; //our global camera

	Game(SDL_Window* window);
	void init( void );
	void render( void );
	void update( double dt );

	void onKeyPressed( SDL_KeyboardEvent event );
	void onMouseButton( SDL_MouseButtonEvent event );

	void setWindowSize(int width, int height);

};


#endif 