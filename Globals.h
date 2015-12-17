#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "SDL/include/SDL_rect.h"

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

enum collider_type
{
	PLAYER,
	PARTICLES,
	OTHERS
};

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

enum player_state
{
	PLAYER_IDLE,
	PLAYER_WALKING_FORWARD,
	PLAYER_WALKING_BACKWARD,
	PLAYER_JUMPING_STATIC,
	PLAYER_CROUCHING
};

// Deletes a buffer
#define RELEASE( x ) \
    {\
       if( x != nullptr )\
       {\
         delete x;\
	     x = nullptr;\
       }\
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
		 }\
	 }

// Configuration -----------
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 224
#define SCREEN_SIZE 4
#define FULLSCREEN false
#define VSYNC true
#define TITLE "Street Fighter 2 - Tribute"

#endif //__GLOBALS_H__