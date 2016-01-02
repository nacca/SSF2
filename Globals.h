#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "SDL/include/SDL_rect.h"

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

enum collider_type
{
	COLLIDER_PLAYER_ONE,
	COLLIDER_ATTACK_PLAYER_ONE,
	COLLIDER_PLAYER_TWO,
	COLLIDER_ATTACK_PLAYER_TWO,
	COLLIDER_PARTICLES,
	COLLIDER_OTHERS
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
	PLAYER_CROUCHING,
	PLAYER_LEFT_PUNCH,
	PLAYER_LEFT_KICK,
	PLAYER_HIT,
	PLAYER_FACE_HIT,
	PLAYER_CROUCH_HIT,
	PLAYER_KO
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
#define SCREEN_SIZE 2
#define FULLSCREEN false
#define VSYNC true
#define TITLE "Street Fighter 2 - Tribute"

#endif //__GLOBALS_H__