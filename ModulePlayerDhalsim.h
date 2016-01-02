#ifndef __MODULEPLAYERDHALSIM_H__
#define __MODULEPLAYERDHALSIM_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "Point.h"
#include "ModuleCollisions.h"
#include "Collider.h"

struct SDL_Texture;

class ModulePlayerDhalsim : public Module
{
public:
	ModulePlayerDhalsim(bool start_enabled = true);
	virtual ~ModulePlayerDhalsim();

	virtual bool Start();
	virtual update_status PreUpdate();
	virtual update_status Update();
	virtual update_status PostUpdate();
	virtual bool CleanUp();

	virtual iPoint getPosition();
	virtual void OnCollision(Collider* c1, Collider* c2);

	virtual bool playerInCameraLimit() const;

public:
	int life;

protected:
	SDL_Texture* graphics = nullptr;
	iPoint position;
	bool move_right;
	bool move_left;
	Collider* collider_head;
	Collider* collider_body;
	Collider* collider_legs;
	Collider* collider_attack;

	bool jumping;
	player_state playerState;
	Animation particula;

	bool looking_right;
	bool colliding_players;

	Animation idle;
	Animation forward;
	Animation backward;
	Animation crouching;
	Animation L_punch;
	Animation L_kick;
	Animation hit;
	Animation face_hit;
	Animation crouch_hit;
	Animation ko;

	bool hitted;
	bool head_hitted;

};

#endif //Dhalsim

