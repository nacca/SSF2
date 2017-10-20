#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "SDL/include/SDL_rect.h"

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

enum ComboTypes
{
	COMBO_YOGA_FIRE,
	COMBO_YOGA_FLAME,
	AERIAL_COMBO,
	AERIAL_COMBO_PUNCH,
	AERIAL_COMBO_KICK,
	COMBO_NOTHING
};

enum ColliderType
{
	COLLIDER_PLAYER_ONE,
	COLLIDER_BODY_PLAYER_ONE,
	COLLIDER_ATTACK_PLAYER_ONE,
	COLLIDER_PLAYER_TWO,
	COLLIDER_BODY_PLAYER_TWO,
	COLLIDER_ATTACK_PLAYER_TWO,
	COLLIDER_PARTICLES,
	COLLIDER_OTHERS
};

enum UpdateStatus
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

enum DirectionJumping
{
	JUMP_STATIC,
	JUMP_LEFT,
	JUMP_RIGHT
};

enum PlayerState
{
	PLAYER_IDLE,
	PLAYER_WALKING_FORWARD,
	PLAYER_WALKING_BACKWARD,
	PLAYER_JUMPING,
	PLAYER_CROUCHING,
	PLAYER_HIT,
	PLAYER_FACE_HIT,
	PLAYER_CROUCH_HIT,
	PLAYER_KO,
	PLAYER_TIME_OUT,
	PLAYER_WIN_1,
	PLAYER_WIN_2,
	PLAYER_LOW_PUNCH,
	PLAYER_LOW_KICK,
	PLAYER_MEDIUM_PUNCH,
	PLAYER_MEDIUM_KICK,
	PLAYER_HIGH_PUNCH,
	PLAYER_HIGH_KICK,
	PLAYER_CROUCH_PUNCH,
	PLAYER_CROUCH_KICK,
	PLAYER_JUMP_PUNCH,
	PLAYER_JUMP_KICK,
	PLAYER_FORWARD_LOW_PUNCH,
	PLAYER_FORWARD_LOW_KICK,
	PLAYER_FORWARD_MEDIUM_PUNCH,
	PLAYER_FORWARD_MEDIUM_KICK,
	PLAYER_FORWARD_HIGH_PUNCH,
	PLAYER_FORWARD_HIGH_KICK,
	PLAYER_YOGA_FIRE,
	PLAYER_YOGA_FLAME,
	PLAYER_YOGA_MUMMY,
	PLAYER_YOGA_SPEAR,
	PLAYER_BLOCKING,
	PLAYER_CROUCH_BLOCKING,
	PLAYER_BLOCKING_HITTED,
	PLAYER_CROUCH_BLOCKING_HITTED,
	PLAYER_AIR_HITTED
};

enum DamageType
{
	L_ATTACK,
	M_ATTACK,
	H_ATTACK,
	NONE_DAMAGE
};

enum InputType
{
	INPUT_UP,
	INPUT_DOWN,
	INPUT_LEFT,
	INPUT_RIGHT,
	INPUT_L_PUNCH,
	INPUT_L_KICK,
	INPUT_M_PUNCH,
	INPUT_M_KICK,
	INPUT_H_PUNCH,
	INPUT_H_KICK
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
#define SCREEN_SIZE 3
#define FULLSCREEN false
#define VSYNC true
#define TITLE "Super Street Fighter 2 - Tribute"

#endif //__GLOBALS_H__