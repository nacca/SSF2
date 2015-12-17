#ifndef __MODULEPLAYER_H__
#define __MODULEPLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "Point.h"
#include "ModuleCollisions.h"
#include "Collider.h"

struct SDL_Texture;

class ModulePlayer : public Module
{
public:
	ModulePlayer(bool start_enabled = true);
	virtual ~ModulePlayer();

	virtual bool Start();
	virtual update_status Update();
	virtual bool CleanUp();

public:

	SDL_Texture* graphics = nullptr;
	Animation idle;
	Animation forward;
	Animation backward;
	Animation crouching;
	iPoint position;
	bool move_right;
	bool move_left;
	Collider collider;
};

#endif // __MODULEPLAYER_H__