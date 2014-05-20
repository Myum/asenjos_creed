#include "utils.h"

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include "includes.h"

long getTime()
{
#ifdef WIN32
	return GetTickCount();
#else
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return (int)(tv.tv_sec*1000 + (tv.tv_usec / 1000));
#endif
}

//Draw the grid
void drawGrid(float dist)
{
	int num_lines = 20;
	glLineWidth(1);
	glColor3f(0.5,0.5,0.5);
	glBegin( GL_LINES );
	for (int i = 0; i <= num_lines * 0.5; ++i)
	{
		float a = dist * num_lines * 0.5;
		float b = i * dist;

		if (i == num_lines * 0.5)
			glColor3f(1,0.25,0.25);
		else if (i%2)
			glColor3f(0.25,0.25,0.25);
		else
			glColor3f(0.5,0.5,0.5);


		glVertex3f(a,b,-a);
		glVertex3f(-a,b,-a);
		glVertex3f(a,-b,-a);
		glVertex3f(-a,-b,-a);
		glVertex3f(b,a,-a);
		glVertex3f(b,-a,-a);
		glVertex3f(-b,a,-a);
		glVertex3f(-b,-a,-a);
		glVertex3f(a,b,a);
		glVertex3f(-a,b,a);
		glVertex3f(a,-b,a);
		glVertex3f(-a,-b,a);
		glVertex3f(b,a,a);
		glVertex3f(b,-a,a);
		glVertex3f(-b,a,a);
		glVertex3f(-b,-a,a);


		glVertex3f(a,-a,b);
		glVertex3f(-a,-a,b);
		glVertex3f(a,-a,-b);
		glVertex3f(-a,-a,-b);

		glVertex3f(b,-a,a);
		glVertex3f(b,-a,-a);
		glVertex3f(-b,-a,a);
		glVertex3f(-b,-a,-a);

		glVertex3f(-a, a,b);
		glVertex3f(-a, -a,b);
		glVertex3f(-a, a,-b);
		glVertex3f(-a, -a,-b);
		glVertex3f(-a, b,a);
		glVertex3f(-a, b,-a);
		glVertex3f(-a, -b,a);
		glVertex3f(-a, -b,-a);
		glVertex3f(a, a,b);
		glVertex3f(a, -a,b);
		glVertex3f(a, a,-b);
		glVertex3f(a, -a,-b);
		glVertex3f(a, b,a);
		glVertex3f(a, b,-a);
		glVertex3f(a, -b,a);
		glVertex3f(a, -b,-a);
	}
	glEnd();
}

//this function is used to access OpenGL Extensions (special features not supported by all cards)
void* getGLProcAddress(const char* name)
{
	return SDL_GL_GetProcAddress(name);
}


bool checkGLErrors()
{
	GLenum errCode;
	const GLubyte *errString;

	if ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL Error: " << errString << std::endl;
		return false;
	}

	return true;
}

//SDL_Joystick* openJoystick(int num_joystick)
SDL_Joystick* openJoystick()
{
	// Initialize the joystick subsystem
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	SDL_JoystickEventState(SDL_ENABLE);
	int joystick_index = -1;

	// Check for number of joysticks available
	if(SDL_NumJoysticks() <= 0)
		return NULL;

	std::cout << "# Joysticks connected " << SDL_NumJoysticks() << std::endl;

	// Open joystick and return it
	//return SDL_JoystickOpen(num_joystick);
	while(SDL_JoystickOpen(joystick_index) == NULL)
	{
		joystick_index++;
		return SDL_JoystickOpen(joystick_index);
	}

}

JoystickState getJoystickState(SDL_Joystick* joystick)
{
	JoystickState state;
	memset(&state,0,sizeof(JoystickState)); //clear

	if ( joystick == NULL )
	{
		std::cout << "Error: Joystick not opened" << std::endl;
		return state;
	}

	state.num_axis = SDL_JoystickNumAxes((SDL_Joystick*) joystick);
	state.num_buttons = SDL_JoystickNumButtons( (SDL_Joystick*)joystick);

	if (state.num_axis > 8) state.num_axis = 8;
	if (state.num_buttons > 16) state.num_buttons = 16;

	for (int i = 0; i < state.num_axis; ++i){ //axis
		state.axis[i] = SDL_JoystickGetAxis((::SDL_Joystick*) joystick,i) / 32768.0f; //range -32768 to 32768
		//std::cout << " -- " << state.axis[i] << " }}" <<std::endl;
	}
	//system("PAUSE");
	for (int i = 0; i < state.num_buttons; ++i) //buttons
		state.button[i] = SDL_JoystickGetButton((::SDL_Joystick*) joystick,i);
	state.hat = (HATState)(SDL_JoystickGetHat((::SDL_Joystick*) joystick,0) - SDL_HAT_CENTERED); //one hat is enough

	return state;
}